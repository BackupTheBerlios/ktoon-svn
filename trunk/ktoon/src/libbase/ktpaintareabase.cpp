/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ktpaintareabase.h"


#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>
#include <QApplication>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QClipboard>
#include <QMenu>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"
#include "kttextitem.h"
#include "ktpaintarearotator.h"

#include "ktgraphicsscene.h"
#include "ktgraphicalgorithm.h"

#include <dcore/dconfig.h>
#include <dcore/ddebug.h>
#include <dgui/dapplication.h>
#include <dgui/dosd.h>

#include <cmath>


#ifdef QT_OPENGL_LIB

#include <QGLWidget>
#include <QGLFramebufferObject>

class GLDevice : public QGLWidget
{
	public:
		GLDevice() : QGLWidget()
		{
			makeCurrent();
		}
		~GLDevice() {};
		
	protected:
		void initializeGL()
		{
			glDisable(GL_DEPTH_TEST);
		}
};

#endif

struct KTPaintAreaBase::Private
{
	QGraphicsRectItem *grid;
	
	QRectF drawingRect;
	
	bool drawGrid;
	double angle;
	
	KTPaintAreaRotator *rotator;
	
	QStringList copiesXml;
	
	KTGraphicsScene *scene;
};



KTPaintAreaBase::KTPaintAreaBase(QWidget * parent) : QGraphicsView(parent), d(new Private)
{
	d->scene = new KTGraphicsScene();
	
	d->grid = 0;
	
	d->drawGrid = false;
	d->angle = 0;
	
	d->rotator = new KTPaintAreaRotator(this, this);
	
	d->drawingRect = QRectF(QPointF(0,0), QSizeF( 500, 400 ) ); // FIXME: configurable
	
	d->scene->setSceneRect(d->drawingRect);
	setScene(d->scene);
	
	centerDrawingArea();
	
	setUseOpenGL( false );
	setInteractive ( true );
	
	restoreState();
}

void KTPaintAreaBase::saveState()
{
	DConfig *config = dApp->config( "PaintArea" );
	config->setValue("RenderHints", int(renderHints()));
	
}

void KTPaintAreaBase::restoreState()
{
	DConfig *config = dApp->config( "PaintArea" );
	
	int renderHints = config->value("RenderHints", int(this->renderHints())).toInt();
	setRenderHints(QPainter::RenderHints(renderHints));
}

KTPaintAreaBase::~KTPaintAreaBase()
{
	saveState();
	delete d;
}

void KTPaintAreaBase::setAntialiasing(bool use)
{
#ifdef QT_OPENGL_LIB
	if ( QGLWidget *gl = dynamic_cast<QGLWidget *>(viewport() ) )
	{
		gl->setUpdatesEnabled(false); // works better
		gl->setFormat(QGLFormat(QGL::SampleBuffers | QGL::HasOverlay /*| QGL::DirectRendering | QGL::AccumBuffer | QGL::Rgba */));
		gl->setUpdatesEnabled(true);
	}
#endif
	
	setRenderHint(QPainter::Antialiasing, use);
	setRenderHint(QPainter::TextAntialiasing, use);
}

void KTPaintAreaBase::setUseOpenGL(bool opengl)
{
	D_FUNCINFO << opengl;
	
	QCursor cursor(Qt::ArrowCursor);
	if ( viewport() )
	{
		cursor = viewport()->cursor();
	}
	
#ifdef QT_OPENGL_LIB
	if ( opengl )
	{
		setViewport(new GLDevice());
	}
	else
	{
// 		setViewport( new KTImageDevice() );
	}
#else
	Q_UNUSED(opengl);
	dWarning() << tr("OpenGL isn't supported");
#endif
	
	// to restore the cursor.
	if ( viewport() )
	{
		viewport()->setCursor(cursor);
		viewport()->setAcceptDrops(true);
	}
}

void KTPaintAreaBase::setDrawGrid(bool draw)
{
	d->drawGrid = draw;
// 	resetCachedContent();
	viewport()->update();
}

void KTPaintAreaBase::setTool(KTToolPlugin *tool )
{
	if ( !scene() ) return;
	
	if ( tool )
	{
		disconnect(tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
	}
	
	d->scene->setTool(tool);
	
	connect(tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
}

bool KTPaintAreaBase::drawGrid() const
{
	return d->drawGrid;
}

void KTPaintAreaBase::mousePressEvent ( QMouseEvent * event )
{
	if ( !canPaint() ) return;
	
	if ( event->buttons() == Qt::LeftButton )
	{
		QGraphicsView::mousePressEvent(event);
	}
	else if ( event->buttons() == Qt::RightButton )
	{
		QMenu *menu = new QMenu(tr("Drawing area"));
		
		menu->addAction(dApp->findGlobalAction("undo"));
		menu->addAction(dApp->findGlobalAction("redo"));
		
		menu->addSeparator();
		
		QAction *cut = menu->addAction(tr("Cut"), this, SLOT(cutItems()));
		QAction *copy = menu->addAction(tr("Copy"), this, SLOT(copyItems()));
		QAction *paste = menu->addAction(tr("Paste"), this, SLOT(pasteItems()));
		QAction *del = menu->addAction(tr("Delete"), this, SLOT(deleteItems()));
		
		menu->addSeparator();
		
		QMenu *order = new QMenu(tr("Order"));
		
		connect(order, SIGNAL(triggered( QAction* )), this, SLOT(requestMoveSelectedItems( QAction *)));
		order->addAction(tr("Send to back"))->setData(MoveBack);
		order->addAction(tr("Bring to front"))->setData(MoveFront);
		order->addAction(tr("Send backwards"))->setData(MoveBackwards);
		order->addAction(tr("Brind forwards"))->setData(MoveForwards);
		menu->addMenu ( order );
		menu->addSeparator();
		
		menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
		
		menu->addSeparator();
		
		
		
		if(scene()->selectedItems().isEmpty())
		{
			del->setEnabled(false);
			cut->setEnabled(false);
			copy->setEnabled(false);
		}
		
		if ( d->copiesXml.isEmpty() )
		{
			paste->setEnabled(false);
		}
		
		if ( QMenu *toolMenu = d->scene->currentTool()->menu() )
		{
			menu->addSeparator();
			menu->addMenu(toolMenu);
		}
		menu->exec(event->globalPos());
	}
}

void KTPaintAreaBase::mouseMoveEvent( QMouseEvent * event )
{
	if ( !canPaint()) return;
	
	// Rotate
	if( !d->scene->isDrawing() && event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		setUpdatesEnabled(false);
		
		setDragMode(QGraphicsView::NoDrag);
		
		QPointF p1 = event->pos();
		QPointF p2 = d->drawingRect.center();
		
		d->rotator->rotateTo( (int)(-(180*KTGraphicalAlgorithm::angleForPos(p1,p2))/M_PI) );
		setUpdatesEnabled(true);
	}
	else
	{
		QGraphicsView::mouseMoveEvent(event);
		
		if( !d->scene->mouseGrabberItem() && d->scene->isDrawing() ) // HACK
		{
			QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
			mouseEvent.setWidget(viewport());
			mouseEvent.setScenePos(mapToScene(event->pos()));
			mouseEvent.setScreenPos(event->globalPos());
			mouseEvent.setButtons(event->buttons());
			mouseEvent.setButton(event->button());
			mouseEvent.setModifiers(event->modifiers());
			mouseEvent.setAccepted(false);
// 			QApplication::sendEvent(d->scene, &mouseEvent);
			d->scene->mouseMoved(&mouseEvent);
		}
	}
	
	emit cursorPosition( mapToScene( event->pos() ) );
}

void KTPaintAreaBase::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
	
	if( ! d->scene->mouseGrabberItem() && d->scene->isDrawing() ) // HACK
	{
		QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
		mouseEvent.setWidget(viewport());
		mouseEvent.setScenePos(mapToScene(event->pos()));
		mouseEvent.setScreenPos(event->globalPos());
		mouseEvent.setButtons(event->buttons());
		mouseEvent.setButton(event->button());
		mouseEvent.setModifiers(event->modifiers());
		mouseEvent.setAccepted(false);
// 		QApplication::sendEvent(d->scene, &mouseEvent);
		d->scene->mouseReleased(&mouseEvent);
	}
}

void KTPaintAreaBase::tabletEvent( QTabletEvent * event )
{
	QGraphicsView::tabletEvent(event );
}

void KTPaintAreaBase::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawBackground(painter, rect);
	
	painter->save();
	
	bool hasAntialiasing = painter->renderHints() & QPainter::Antialiasing;
	
	painter->setRenderHint(QPainter::Antialiasing, false);
	
	painter->setPen( QPen(QColor(0,0,0,180), 3) );
	
	painter->fillRect( d->drawingRect, Qt::white );
	painter->drawRect( d->drawingRect );
	
	if ( d->drawGrid )
	{
		int sx = (int)painter->matrix().m11();
		int sy = (int)painter->matrix().m22();
		
		painter->resetMatrix();
		painter->scale(sx, sy);
		painter->setPen( QPen(QColor(0,0,180, 50), 1) );
		
		for(int i = 0; i < qMax(width(), height()); i+= 10 )
		{
			painter->drawLine(i, 0, i, height() );
			painter->drawLine(0, i, width(), i );
		}
	}
	
	painter->setRenderHint(QPainter::Antialiasing, hasAntialiasing);
	
	emit changedZero(painter->matrix().map(QPointF(0,0)));
	
	painter->restore();
}

void KTPaintAreaBase::drawForeground( QPainter *painter, const QRectF &rect )
{
	if ( KTFrame *frame = d->scene->currentFrame() )
	{
		if ( frame->isLocked() )
		{
			painter->fillRect(rect, QColor(201,201,201, 200));
			
			painter->setFont(QFont("Arial", 30) );
			QFontMetricsF fm(painter->font());
			QString text = tr("Locked");
			
			painter->drawText(QPointF(d->scene->sceneRect().topRight().x() - fm.width(text), (d->scene->sceneRect().topRight().y() + fm.height()) / 2), text);
		}
	}
}

bool KTPaintAreaBase::canPaint() const
{
	if( d->scene )
	{
		KTFrame *frame = d->scene->currentFrame();
		
		if( frame )
		{
			return !frame->isLocked();
		}
	}
	
	return false;
}

void KTPaintAreaBase::centerDrawingArea()
{
	centerOn(d->drawingRect.center());
}

void KTPaintAreaBase::wheelEvent(QWheelEvent *event)
{
	if(event->modifiers () == Qt::ControlModifier)
	{
		scaleView(pow(( double)2, -event->delta() / 240.0));
	}
	else
	{
		QGraphicsView::wheelEvent(event);
	}
}

bool KTPaintAreaBase::viewportEvent(QEvent *e)
{
	bool ret = QGraphicsView::viewportEvent(e);
	
	if( e->type() == QEvent::Show )
	{
		if( d->scene->items().isEmpty() )
		{
			d->scene->drawCurrentPhotogram();
		}
	}
	
	return ret;
}

void KTPaintAreaBase::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
	
	emit scaled(scaleFactor);
}

void KTPaintAreaBase::setRotationAngle(int angle)
{
	rotate(angle - d->angle );
	d->angle = angle;
}

KTBrushManager *KTPaintAreaBase::brushManager() const
{
	return d->scene->brushManager();
}

QRectF KTPaintAreaBase::drawingRect() const
{
	return d->drawingRect;
}

KTGraphicsScene *KTPaintAreaBase::graphicsScene() const
{
	return d->scene;
}

