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
#include "ktscene.h"

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
	KTToolPlugin *tool;
	
	bool isDrawing;
	
	KTBrushManager *brushManager;
	KTInputDeviceInformation *inputInformation;
	
	QRectF drawingRect;
	
	bool drawGrid;
	double angle;
	
	KTPaintAreaRotator *rotator;
	
	QStringList copiesXml;
};



KTPaintAreaBase::KTPaintAreaBase(QWidget * parent) : QGraphicsView(parent), d(new Private)
{
	d->grid = 0;
	d->tool = 0;
	d->isDrawing = false;
	
	d->drawGrid = false;
	d->angle = 0;
	
	d->brushManager = new KTBrushManager(this);
	d->rotator = new KTPaintAreaRotator(this, this);
	d->inputInformation = new KTInputDeviceInformation(this);
	
	d->drawingRect = QRectF(QPointF(0,0), QSizeF( 500, 400 ) ); // FIXME: configurable
	
	setScene(0);
	
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
	
	if ( d->tool )
	{
		d->tool->aboutToChangeTool();
		disconnect(d->tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
	}
	
	d->tool = tool;
	d->tool->init(this);
	
	connect(d->tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
}

bool KTPaintAreaBase::drawGrid() const
{
	return d->drawGrid;
}

void KTPaintAreaBase::mousePressEvent ( QMouseEvent * event )
{
	if ( !canPaint() ) return;
	
	QMouseEvent *eventMapped = mapToArea( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	delete eventMapped;
	
	if ( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		d->isDrawing = false;
	}
	else if (d->tool )
	{
		KTScene *sscene = qobject_cast<KTScene *>(scene());
		
		if ( event->buttons() == Qt::LeftButton && !sscene->currentFrame()->isLocked() )
		{
			QGraphicsView::mousePressEvent(event);
			
			d->tool->begin();
			d->isDrawing = true;
			d->tool->press(d->inputInformation, d->brushManager, sscene, this );
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
			
			menu->addSeparator();
			QAction *del = menu->addAction(tr("Delete"), this, SLOT(deleteItems()));
			
			menu->addSeparator();
			
			/*QAction *addToLib = */menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
			
			if ( scene()->selectedItems().isEmpty() )
			{
				del->setEnabled(false);
				cut->setEnabled(false);
				copy->setEnabled(false);
			}
			
			if ( d->copiesXml.isEmpty() )
			{
				paste->setEnabled(false);
			}
			
			if ( QMenu *toolMenu = d->tool->menu() )
			{
				menu->addSeparator();
				menu->addMenu(toolMenu);
			}
			
			menu->exec(event->globalPos());
			
// 			QGraphicsView::mousePressEvent(event);
		}
	}
}


void KTPaintAreaBase::mouseDoubleClickEvent( QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	
	QMouseEvent *eventMapped = mapToArea( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	if (d->tool)
	{
		d->tool->doubleClick( d->inputInformation,  qobject_cast<KTScene *>(scene()), this );
	}
	
	delete eventMapped;
}

void KTPaintAreaBase::mouseMoveEvent ( QMouseEvent * event )
{
	if ( !canPaint()) return;
	
	QMouseEvent *eventMapped = mapToArea( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	// Rotate
	if(  !d->isDrawing && event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		setUpdatesEnabled(false);
		
		
		setDragMode (QGraphicsView::NoDrag);
		
		QPointF p1 = event->pos();
		QPointF p2 = d->drawingRect.center();
		
		QPointF dp = p1 - p2;
		
		if(dp.x() != 0)
		{
			double a =  atan(dp.y() / dp.x())*(180/M_PI);
			if(dp.x() < 0)
			{
				a += 180;
			}
			d->rotator->rotateTo( (int)a );
		}
		
		setUpdatesEnabled(true);
	}
	else if (d->tool && d->isDrawing )
	{
		d->tool->move(d->inputInformation, d->brushManager,  qobject_cast<KTScene *>(scene()), this );
	}
	
	emit cursorPosition( mapToScene( event->pos() ) );
	
	delete eventMapped;
	
	QGraphicsView::mouseMoveEvent(event);
}

void KTPaintAreaBase::mouseReleaseEvent(QMouseEvent *event)
{
	if ( !canPaint()) return;
	QMouseEvent *eventMapped = mapToArea( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	if ( d->tool && d->isDrawing )
	{
		KTScene *currentScene = qobject_cast<KTScene *>(scene());
		d->tool->release(d->inputInformation, d->brushManager,  currentScene, this );
		
		
		d->tool->end();
	}
	
	delete eventMapped;
	
	d->isDrawing = false;
	
	QGraphicsView::mouseReleaseEvent(event);
}

void KTPaintAreaBase::tabletEvent ( QTabletEvent * event )
{
	d->inputInformation->updateFromTabletEvent( event );
	
	QGraphicsView::tabletEvent(event );
}


QMouseEvent *KTPaintAreaBase::mapToArea(QMouseEvent *event) const
{
	QMouseEvent *e = new QMouseEvent(event->type(), this->mapToScene(event->pos()).toPoint(), this->mapToScene( event->globalPos() ).toPoint(), event->button(), event->buttons(), event->modifiers () );
	
	return e;
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
	
	painter->restore();
}

void KTPaintAreaBase::drawForeground( QPainter *painter, const QRectF &rect )
{
	if ( KTScene *sscene = qobject_cast<KTScene *>(scene()) )
	{
		if ( KTFrame *frame = sscene->currentFrame() )
		{
			if ( frame->isLocked() )
			{
				painter->fillRect(rect, QColor(201,201,201, 200));
				
				painter->setFont(QFont("Arial", 30) );
				QFontMetricsF fm(painter->font());
				QString text = tr("Locked");
				
				painter->drawText(QPointF(sscene->sceneRect().topRight().x() - fm.width(text), (sscene->sceneRect().topRight().y() + fm.height()) / 2), text);
			}
		}
	}
}

bool KTPaintAreaBase::canPaint() const
{
	return scene() != 0;
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

void KTPaintAreaBase::keyPressEvent(QKeyEvent *event)
{
	if ( d->tool )
	{
		d->tool->keyPressEvent(event);
		
		if ( event->isAccepted() )
		{
			return;
		}
	}
	
	QGraphicsView::keyPressEvent(event);
}

void KTPaintAreaBase::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void KTPaintAreaBase::setRotationAngle(int angle)
{
	rotate(angle - d->angle );
	d->angle = angle;
}

KTBrushManager *KTPaintAreaBase::brushManager() const
{
	return d->brushManager;
}


QRectF KTPaintAreaBase::drawingRect() const
{
	return d->drawingRect;
}


