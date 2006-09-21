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

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"
#include "ktitemevent.h"
#include "ktpaintareaevent.h"
#include "ktpaintarearotator.h"
#include "ktimagedevice.h"

#include <cmath>

#include "kttextitem.h"

#ifdef QT_OPENGL_LIB

#include <QGLWidget>

class GLDevice : public QGLWidget
{
	public:
		GLDevice() : QGLWidget()
		{
			makeCurrent();
		}
		~GLDevice() {  };
		
	protected:
		void initializeGL()
		{
// 			glShadeModel(GL_FLAT);
			glDisable(GL_DEPTH_TEST);
			
			glEnable( GL_LINE_SMOOTH );
			glEnable( GL_BLEND );
			glEnable( GL_POINT_SMOOTH );
			glEnable(GL_POLYGON_SMOOTH );
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST  );
			
// 			glDepthFunc(GL_LESS);
// 			glEnable(GL_DEPTH_TEST);
// 			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
};

#endif

#include "ktscene.h"
#include "ktproject.h"

KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : QGraphicsView(parent), m_grid(0), m_tool(0), m_isDrawing(false), m_project(project), m_currentSceneIndex(0), m_drawGrid(false), m_angle(0)
{
// 	setMouseTracking(true);
	
	m_brushManager = new KTBrushManager(this);
	m_rotator = new KTPaintAreaRotator(this, this);
	m_inputInformation = new KTInputDeviceInformation(this);
	
	m_drawingRect = QRectF(QPointF(0,0), QSizeF( 500, 400 ) ); // FIXME: parametrizable
	
	setCurrentScene( 0 );
	qobject_cast<KTScene *>(scene())->setCurrentFrame( 0, 0 ); 
	
	centerDrawingArea();
	
// 	setViewport(new KTImageDevice() );
	setUseOpenGL( false );
	setInteractive ( true );
}

KTPaintArea::~KTPaintArea()
{
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
	if ( m_tool )
	{
		m_tool->aboutToChangeTool();
	}
	
	m_tool = tool;
	m_tool->init(this);
}

bool KTPaintArea::drawGrid() const
{
	return m_drawGrid;
}

void KTPaintArea::mousePressEvent ( QMouseEvent * event )
{
	QGraphicsView::mousePressEvent(event);
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	
	delete eventMapped;
	

	if ( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		m_isDrawing = false;
		
// 		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
	else if (m_tool )
	{
		m_tool->begin();
		
		m_isDrawing = true;
		m_tool->press(m_inputInformation, m_brushManager,  qobject_cast<KTScene *>(scene()), this );
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
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	//Rotate
	if(  !m_isDrawing && event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		setUpdatesEnabled(false);
		
		
		setDragMode (QGraphicsView::NoDrag);
		
		QPointF p1 = event->pos();
		QPointF p2 = m_drawingRect.center();
		
		QPointF d = p1 - p2;
		
// 		if( d.x() < -40 || d.x() > 40  )
		if(d.x() != 0)
		{
			double a =  atan(d.y() / d.x())*(180/M_PI);
			if(d.x() < 0)
			{
				a += 180;
			}
			m_rotator->rotateTo( a );
// 			setRotationAngle( a );
		}
		
		setUpdatesEnabled(true);
	}
	else if (m_tool && m_isDrawing )
	{
		m_tool->move(m_inputInformation, m_brushManager,  qobject_cast<KTScene *>(scene()), this );
	}
	
	emit cursorPosition( mapToScene( eventMapped->pos() ) );
	
	delete eventMapped;
	
	QGraphicsView::mouseMoveEvent(event);
}

void KTPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	m_inputInformation->updateFromMouseEvent( eventMapped );
	
	if ( m_tool && m_isDrawing )
	{
		KTScene *currentScene = qobject_cast<KTScene *>(scene());
		m_tool->release(m_inputInformation, m_brushManager,  currentScene, this );
		
		foreach( KTProjectEvent *e, m_tool->events() )
		{
			emit eventTriggered( e );
		}
		m_tool->end();
	}
	
	delete eventMapped;
	
	m_isDrawing = false;
	
	QGraphicsView::mouseReleaseEvent(event);
}

void KTPaintArea::tabletEvent ( QTabletEvent * event )
{
	m_inputInformation->updateFromTabletEvent( event );
	event->ignore();
	
	QGraphicsView::tabletEvent(event );
}


QMouseEvent *KTPaintArea::mapMouseEvent(QMouseEvent *event) const
{
	QMouseEvent *e = new QMouseEvent(event->type(), this->mapToScene(event->pos()).toPoint(), this->mapToScene( event->globalPos() ).toPoint(), event->button(), event->buttons(), event->modifiers () );
	
	return e;
}

void KTPaintArea::frameEvent(KTFrameEvent *event)
{
	switch(event->action())
	{
		case KTProjectEvent::Select:
		{
			setCurrentScene( event->sceneIndex() );
			qobject_cast<KTScene *>(scene())->setCurrentFrame(event->layerIndex(), event->frameIndex());
			
			qobject_cast<KTScene *>(scene())->drawPhotogram(event->frameIndex());
			
			if ( event->action() == KTProjectEvent::Select )
			{
				if ( m_tool ) m_tool->init( this );
			}
		}
		break;
		default: break;
	}
}

void KTPaintArea::layerEvent(KTLayerEvent *event)
{
	if ( event->action() != KTProjectEvent::Add ||  event->action() != KTProjectEvent::Remove )
	{
		qobject_cast<KTScene *>(scene())->drawCurrentPhotogram();
		viewport()->update(scene()->sceneRect().toRect() );
	}
}

void KTPaintArea::sceneEvent(KTSceneEvent *event)
{
	switch(event->action())
	{
		case KTProjectEvent::Select:
		{
			setCurrentScene( event->sceneIndex() );
		}
		break;
		case KTProjectEvent::Remove:
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

void KTPaintArea::projectEvent(KTProjectEvent *event)
{
	Q_UNUSED(event);
}

void KTPaintArea::itemEvent(KTItemEvent *event)
{
	switch(event->action())
	{
// 		case KTProjectEvent::Add:
// 		{
// 		}
// 		break;
// 		case KTProjectEvent::Remove:
// 		{
// 			
// 		}
		break;
		case KTProjectEvent::Transform:
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
		m_tool->itemEvent( event );
	}
}

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

void KTPaintArea::centerDrawingArea()
{
// 	ensureVisible(m_drawingRect, (width() - m_drawingRect.width()) /2, (height() - m_drawingRect.height()) /2  );
	centerOn(m_drawingRect.center());
}


void KTPaintArea::deleteItems()
{
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
				if(currentScene->currentFrame()->graphics().indexOf(item) != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->graphics().indexOf(item)) ;
						firstItem = currentScene->currentFrame()->graphics().indexOf(item);
					}
					else
					{
						
						strItems += " , "+ QString::number(currentScene->currentFrame()->graphics().indexOf(item)) ;
					}
				}
			}
			strItems+= ")";
		}
		
		QDomDocument doc;
		QDomElement root = doc.createElement("group");
		root.setAttribute("positions", strItems );
		doc.appendChild(root);
		
		KTItemEvent *event = new KTItemEvent(KTProjectEvent::Remove, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, doc.toString());
		emit eventTriggered(event);
		dDebug() << strItems;
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
				if(currentScene->currentFrame()->graphics().indexOf(item) != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->graphics().indexOf(item)) ;
						firstItem = currentScene->currentFrame()->graphics().indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->graphics().indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		
		QDomDocument doc;
		QDomElement root = doc.createElement("group");
		root.setAttribute("positions", strItems );
		doc.appendChild(root);
		
		
		KTItemEvent *event = new KTItemEvent(KTProjectEvent::Group, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, doc.toString());
		emit eventTriggered(event);
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
	
	KTItemEvent *event = new KTItemEvent(KTProjectEvent::Group, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), -1, doc.toString());
	emit eventTriggered(event);
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
			}
		}
	}
	
}

void KTPaintArea::pasteItems()
{
	D_FUNCINFO;
	foreach(QString xml, m_copiesXml)
	{
		KTScene* currentScene = static_cast<KTScene*>(scene());
		KTItemEvent *event = new KTItemEvent(KTProjectEvent::Add, currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), -1, xml);
		emit eventTriggered(event);
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

