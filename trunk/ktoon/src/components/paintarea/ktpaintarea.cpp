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

#include "ktpaintarea.h"

#include "ddebug.h"

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

#include "ktprojectresponse.h"

#include "ktpaintareaevent.h"
#include "ktpaintarearotator.h"
#include "ktimagedevice.h"

#include "dcore/dconfig.h"
#include "dgui/dapplication.h"

#include <cmath>

#include "kttextitem.h"

#include "librarydialog.h"
#include "ktlibraryobject.h"
#include "ktrequestbuilder.h"


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

#include "ktscene.h"
#include "ktproject.h"

KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : QGraphicsView(parent), m_grid(0), m_tool(0), m_isDrawing(false), m_project(project), m_currentSceneIndex(0), m_drawGrid(false), m_angle(0)
{
	m_brushManager = new KTBrushManager(this);
	m_rotator = new KTPaintAreaRotator(this, this);
	m_inputInformation = new KTInputDeviceInformation(this);
	
	m_drawingRect = QRectF(QPointF(0,0), QSizeF( 500, 400 ) ); // FIXME: configurable
	
	setCurrentScene( 0 );
	if ( scene() )
	{
		qobject_cast<KTScene *>(scene())->setCurrentFrame( 0, 0 );
	}
	
	centerDrawingArea();
	
	setUseOpenGL( false );
	setInteractive ( true );
	
	restoreState();
}

void KTPaintArea::saveState()
{
	DConfig *config = dApp->config( "PaintArea" );
	config->setValue("RenderHints", int(renderHints()));
	
}

void KTPaintArea::restoreState()
{
	DConfig *config = dApp->config( "PaintArea" );
	
	int renderHints = config->value("RenderHints", int(this->renderHints())).toInt();
	setRenderHints(QPainter::RenderHints(renderHints));
	
}

KTPaintArea::~KTPaintArea()
{
	saveState();
}

void KTPaintArea::setCurrentScene(int index)
{
	KTScene *sscene = m_project->scene(index);
	if ( sscene )
	{
		m_currentSceneIndex = index;
		
		sscene->clean();
		
		sscene->setItemIndexMethod(QGraphicsScene::NoIndex);
		setScene(sscene);
		
		setBackgroundBrush(Qt::gray);
		
		sscene->setSceneRect( m_drawingRect );
		
		if ( KTScene *prev = dynamic_cast<KTScene *>(scene()) )
		{
			sscene->setCurrentFrame( prev->currentFrameIndex(), prev->currentLayerIndex());
		}
		
		if ( m_tool )
		{
			m_tool->init( this );
		}
		
		sscene->drawCurrentPhotogram();
	}
	else
	{
		setDragMode(QGraphicsView::NoDrag);
		setScene(0);
	}
}

void KTPaintArea::setAntialiasing(bool use)
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

void KTPaintArea::setUseOpenGL(bool opengl)
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
		setViewport( new KTImageDevice() );
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

void KTPaintArea::setDrawGrid(bool draw)
{
	m_drawGrid = draw;
// 	resetCachedContent();
	viewport()->update();
}

void KTPaintArea::setTool(KTToolPlugin *tool )
{
	if ( !scene() ) return;
	
	if ( m_tool )
	{
		m_tool->aboutToChangeTool();
		disconnect(m_tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
	}
	
	m_tool = tool;
	m_tool->init(this);
	
	connect(m_tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
}

bool KTPaintArea::drawGrid() const
{
	return m_drawGrid;
}

void KTPaintArea::mousePressEvent ( QMouseEvent * event )
{
	if ( !canPaint() ) return;
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	
	delete eventMapped;
	
	if ( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		m_isDrawing = false;
	}
	else if (m_tool )
	{
		KTScene *sscene = qobject_cast<KTScene *>(scene());
		QGraphicsView::mousePressEvent(event);
		
		if ( event->buttons() == Qt::LeftButton && !sscene->currentFrame()->isLocked() )
		{
			m_tool->begin();
			
			m_isDrawing = true;
			m_tool->press(m_inputInformation, m_brushManager, sscene, this );
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
			
			QAction *addToLib = menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
			
			if ( scene()->selectedItems().isEmpty() )
			{
				del->setEnabled(false);
				cut->setEnabled(false);
				copy->setEnabled(false);
			}
			
			if ( m_copiesXml.isEmpty() )
			{
				paste->setEnabled(false);
			}
			
			if ( QMenu *toolMenu = m_tool->menu() )
			{
				menu->addSeparator();
				menu->addMenu(toolMenu);
			}
			
			menu->exec(event->globalPos());
			
// 			QGraphicsView::mousePressEvent(event);
		}
	}
}


void KTPaintArea::mouseDoubleClickEvent( QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	
	if (m_tool)
	{
		m_tool->doubleClick( m_inputInformation,  qobject_cast<KTScene *>(scene()), this );
	}
	
	delete eventMapped;
}

void KTPaintArea::mouseMoveEvent ( QMouseEvent * event )
{
	if ( !canPaint()) return;
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	// Rotate
	if(  !m_isDrawing && event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		setUpdatesEnabled(false);
		
		
		setDragMode (QGraphicsView::NoDrag);
		
		QPointF p1 = event->pos();
		QPointF p2 = m_drawingRect.center();
		
		QPointF d = p1 - p2;
		
		if(d.x() != 0)
		{
			double a =  atan(d.y() / d.x())*(180/M_PI);
			if(d.x() < 0)
			{
				a += 180;
			}
			m_rotator->rotateTo( a );
		}
		
		setUpdatesEnabled(true);
	}
	else if (m_tool && m_isDrawing )
	{
		m_tool->move(m_inputInformation, m_brushManager,  qobject_cast<KTScene *>(scene()), this );
	}
	
	emit cursorPosition( mapToScene( event->pos() ) );
	
	delete eventMapped;
	
	QGraphicsView::mouseMoveEvent(event);
}

void KTPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
	if ( !canPaint()) return;
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	
	if ( m_tool && m_isDrawing )
	{
		KTScene *currentScene = qobject_cast<KTScene *>(scene());
		m_tool->release(m_inputInformation, m_brushManager,  currentScene, this );
		
		
		m_tool->end();
	}
	
	delete eventMapped;
	
	m_isDrawing = false;
	
	QGraphicsView::mouseReleaseEvent(event);
}

void KTPaintArea::tabletEvent ( QTabletEvent * event )
{
	m_inputInformation->updateFromTabletEvent( event );
	
	QGraphicsView::tabletEvent(event );
}


QMouseEvent *KTPaintArea::mapMouseEvent(QMouseEvent *event) const
{
	QMouseEvent *e = new QMouseEvent(event->type(), this->mapToScene(event->pos()).toPoint(), this->mapToScene( event->globalPos() ).toPoint(), event->button(), event->buttons(), event->modifiers () );
	
	return e;
}

void KTPaintArea::frameResponse(KTFrameResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			setCurrentScene( event->sceneIndex() );
			
			if ( !scene() ) return;
			
			KTScene *sscene = dynamic_cast<KTScene *>(scene());
			
			sscene->setCurrentFrame(event->layerIndex(), event->frameIndex());
			sscene->drawPhotogram(event->frameIndex());
			
			if ( event->action() == KTProjectRequest::Select )
			{
				if ( m_tool ) m_tool->init( this );
			}
			
			dDebug("paintarea") << "frame: " << event->frameIndex() << " " << "layer: " << event->layerIndex();
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTScene *sscene = dynamic_cast<KTScene *>(scene());
			
			if ( sscene->currentFrameIndex() == event->frameIndex() )
			{
				viewport()->update();
			}
		}
		break;
		default:
		{
			if ( m_tool )
			{
				m_tool->init( this );
			}
		}
		break;
	}
}

void KTPaintArea::layerResponse(KTLayerResponse *event)
{
	if ( !scene() ) return;
	if ( event->action() != KTProjectRequest::Add ||  event->action() != KTProjectRequest::Remove )
	{
		qobject_cast<KTScene *>(scene())->drawCurrentPhotogram();
		viewport()->update(scene()->sceneRect().toRect() );
	}
}

void KTPaintArea::sceneResponse(KTSceneResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			setCurrentScene( event->sceneIndex() );
			
		}
		break;
		case KTProjectRequest::Remove:
		{
			if ( event->sceneIndex() == m_currentSceneIndex )
			{
				setCurrentScene( m_currentSceneIndex-1 );
			}
		}
		break;
		default: break;
	}
}

void KTPaintArea::projectRequest(KTProjectRequest *event)
{
	Q_UNUSED(event);
}

void KTPaintArea::itemResponse(KTItemResponse *event)
{
	switch(event->action())
	{
// 		case KTProjectRequest::Add:
// 		{
// 		}
// 		break;
// 		case KTProjectRequest::Remove:
// 		{
// 			
// 		}
		break;
		case KTProjectRequest::Transform:
		{
			viewport()->update();
		}
		break;
		default:
		{
			qobject_cast<KTScene *>(scene())->drawCurrentPhotogram();
			viewport()->update(scene()->sceneRect().toRect() );
		}
	}
	
	if ( m_tool )
	{
		m_tool->itemResponse( event );
	}
}

void KTPaintArea::projectResponse(KTProjectResponse *)
{
}

// void KTPaintArea::libraryRequest(KTProjectRequest *request)
// {
// }

void KTPaintArea::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawBackground(painter, rect);
	
	painter->save();
	
	bool hasAntialiasing = painter->renderHints() & QPainter::Antialiasing;
	
	painter->setRenderHint(QPainter::Antialiasing, false);
	
	painter->setPen( QPen(QColor(0,0,0,180), 3) );
	painter->fillRect( m_drawingRect, Qt::white );
	painter->drawRect( m_drawingRect );
	
	if ( m_drawGrid )
	{
		int sx = painter->matrix().m11();
		int sy = painter->matrix().m22();
		
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

void KTPaintArea::drawForeground( QPainter *painter, const QRectF &rect )
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

bool KTPaintArea::canPaint() const
{
	if ( ! scene() ) return false;
	
	if ( KTScene *sscene = dynamic_cast<KTScene *>(scene()) )
	{
		if (sscene->currentFrameIndex() >= 0 && sscene->currentLayerIndex() >= 0) return true;
	}
	
	return false;
}

void KTPaintArea::centerDrawingArea()
{
// 	ensureVisible(m_drawingRect, (width() - m_drawingRect.width()) /2, (height() - m_drawingRect.height()) /2  );
	centerOn(m_drawingRect.center());
}


void KTPaintArea::deleteItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems ();
	
	if(!selecteds.empty())
	{
		QString strItems= "";
		
		KTScene* currentScene = static_cast<KTScene*>(scene());
		int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				int indexOfItem = currentScene->currentFrame()->indexOf(item);
				if(indexOfItem != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(indexOfItem);
						firstItem = indexOfItem;
					}
					else
					{
						strItems += " , "+ QString::number(indexOfItem);
					}
				}
			}
			strItems+= ")";
			
			KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Remove, strItems );
			
			dDebug(2) << "Borrando del frame " << currentScene->currentFrameIndex() << "\n" << "lo items " << strItems ;
			emit requestTriggered(&event);
		}
	}
}

void KTPaintArea::groupItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		QString strItems= "";
		
		KTScene* currentScene = static_cast<KTScene*>(scene());
		int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				if(currentScene->currentFrame()->indexOf(item) != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->indexOf(item)) ;
						firstItem = currentScene->currentFrame()->indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		
		QDomDocument doc;
		QDomElement root = doc.createElement("group");
		root.setAttribute("positions", strItems );
		doc.appendChild(root);
		
		
// 		KTProjectRequest event(KTProjectRequest::Group, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, doc.toString());
// 		emit requestTriggered(&event);
	}
	
	/*D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	QDomDocument doc;
	if(!selecteds.isEmpty())
	{
		KTScene* currentScene = static_cast<KTScene*>(scene());
		
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				doc.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( doc ));
			}
		}
	}
	
	KTScene* currentScene = static_cast<KTScene*>(scene());
	
	KTProjectRequest *event = new KTProjectRequest(KTProjectRequest::Group, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), -1, doc.toString());
	emit requestTriggered(event);
									     */
}

void KTPaintArea::ungroupItems()
{
}

void KTPaintArea::copyItems()
{
	D_FUNCINFO;
	m_copiesXml.clear();
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		KTScene* currentScene = static_cast<KTScene*>(scene());
		int firstItem = -1;
		
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				QDomDocument orig;
				orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
				m_copiesXml << orig.toString();
				
				// Paint it to clipbard
				QPixmap toPixmap(item->boundingRect().size().toSize());
				toPixmap.fill(Qt::transparent);
				
				QPainter painter(&toPixmap);
				painter.setRenderHint(QPainter::Antialiasing);
				
				QStyleOptionGraphicsItem opt;
				
				opt.state = QStyle::State_None;
				
				if (item->isEnabled())
					opt.state |= QStyle::State_Enabled;
				if (item->hasFocus())
					opt.state |= QStyle::State_HasFocus;
				if (item == currentScene->mouseGrabberItem())
					opt.state |= QStyle::State_Sunken;
				
				opt.exposedRect = item->boundingRect();
				opt.levelOfDetail = 1;
				opt.matrix = item->sceneMatrix();
				opt.palette = palette();
				
				item->paint(&painter, &opt, this);
				
				painter.end();
				
				QApplication::clipboard()->setPixmap(toPixmap);
			}
		}
	}
	
}

void KTPaintArea::pasteItems()
{
	D_FUNCINFO;
	KTScene* currentScene = static_cast<KTScene*>(scene());
	
	foreach(QString xml, m_copiesXml)
	{
		KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->graphics().count(), KTProjectRequest::Add, xml);
		
		emit requestTriggered(&event);
	}
}

void KTPaintArea::cutItems()
{
	D_FUNCINFO;
	copyItems();
	deleteItems();
}

void KTPaintArea::wheelEvent(QWheelEvent *event)
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

void KTPaintArea::keyPressEvent(QKeyEvent *event)
{
	if ( m_tool )
	{
		m_tool->keyPressEvent(event);
		
		if ( event->isAccepted() )
		{
			return;
		}
	}
	
	QGraphicsView::keyPressEvent(event);
}

void KTPaintArea::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void KTPaintArea::setRotationAngle(int angle)
{
	rotate(angle - m_angle );
	m_angle = angle;
}

KTBrushManager *KTPaintArea::brushManager() const
{
	return m_brushManager;
}

void KTPaintArea::setNextFramesOnionSkinCount(int n)
{
	if ( KTScene* currentScene = static_cast<KTScene*>(scene()) )
	{
		currentScene->setNextOnionSkinCount( n );
	}
}

void KTPaintArea::setPreviousFramesOnionSkinCount(int n)
{
	if ( KTScene* currentScene = static_cast<KTScene*>(scene()) )
	{
		currentScene->setPreviousOnionSkinCount( n );
	}
}

void KTPaintArea::addSelectedItemsToLibrary()
{
	dDebug("paintarea") << "Adding to library";
	
	LibraryDialog dialog;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	
	foreach (QGraphicsItem *item, selecteds )
	{
		dialog.addItem( item );
	}
	
	if ( dialog.exec() != QDialog::Accepted )
	{
		return;
	}
	
	foreach (QGraphicsItem *item, selecteds )
	{
		if ( KTAbstractSerializable *itemSerializable = dynamic_cast<KTAbstractSerializable *>(item) )
		{
			QString symName = dialog.symbolName( item );
			
			KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTLibraryObject::Item, KTProjectRequest::Add, symName );
			
			emit requestTriggered(&request);
		}
	}
}


