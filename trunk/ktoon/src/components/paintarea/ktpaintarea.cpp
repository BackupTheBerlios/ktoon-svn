/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktbrushmanager.h"

#ifdef QT_OPENGL_LIB

#include <QGLWidget>

class GLDevice : public QGLWidget
{
	public:
		GLDevice() {};
		~GLDevice() {};
		
	protected:
		void initializeGL()
		{
			glShadeModel(GL_FLAT);
			glDisable(GL_DEPTH_TEST);
	
			glEnable( GL_LINE_SMOOTH );
			glEnable( GL_BLEND );
			glEnable( GL_POINT_SMOOTH );
		}
};

#endif

#include "ktscene.h"
#include "ktproject.h"

KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : QGraphicsView(parent), m_grid(0), m_tool(0), m_isDrawing(false), m_project(project)
{
	setMouseTracking(true);
	
	m_brushManager = new KTBrushManager(this);
	connect(m_brushManager, SIGNAL(penChanged( const QPen& )), this, SLOT(setCurrentPen(const QPen &)));
	connect(m_brushManager, SIGNAL(brushChanged( const QBrush& )), this, SLOT(setCurrentBrush(const QBrush &)));
	
	setCurrentScene( 0 );
	
	QPoint zero(scene()->width() - 500, scene()->height() - 400); // FIXME: parametrizable
	m_drawingRect =  QRectF(mapToScene(zero/2), QSizeF( 500, 400) );
	
	ensureVisible(m_drawingRect);
}

KTPaintArea::~KTPaintArea()
{
	
}

void KTPaintArea::setCurrentScene(int index)
{
	KTScene *sscene = m_project->scene(index);
	if ( sscene )
	{
		sscene->clean();
		
		sscene->setItemIndexMethod(QGraphicsScene::NoIndex);
		setScene(sscene);
		setBackgroundBrush(Qt::gray);
	}
}

void KTPaintArea::setUseOpenGL(bool opengl)
{
#ifdef QT_OPENGL_LIB
	if ( opengl )
	{
		setViewport(new GLDevice());
	}
	else
	{
		setViewport(new QWidget());
	}
#else
	Q_UNUSED(opengl);
	dWarning() << tr("OpenGL isn't supported");
#endif
}


void KTPaintArea::setTool(KTToolPlugin *tool )
{
	m_tool = tool;
}

void KTPaintArea::mousePressEvent ( QMouseEvent * event )
{
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	if (m_tool )
	{
		m_isDrawing = true;
		m_tool->press(eventMapped, m_brushManager,  qobject_cast<KTScene *>(scene()) );
	}
	
	delete eventMapped;
	
	QGraphicsView::mousePressEvent(event);
	
}

void KTPaintArea::mouseMoveEvent ( QMouseEvent * event )
{
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	
	if (m_tool && m_isDrawing )
	{
		m_tool->move(eventMapped, m_brushManager,  qobject_cast<KTScene *>(scene()) );
	}
	
	emit cursorPosition( mapToScene( eventMapped->pos() ) );
	
	delete eventMapped;
	
	QGraphicsView::mouseMoveEvent(event);
}

void KTPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
	D_FUNCINFO;
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	if ( m_tool )
	{
		m_tool->release(eventMapped, m_brushManager,  qobject_cast<KTScene *>(scene()) );
	}
	
	delete eventMapped;
	
	m_isDrawing = false;
	
	QGraphicsView::mouseReleaseEvent(event);
}


void KTPaintArea::resizeEvent ( QResizeEvent * event )
{
// 	scene()->setSceneRect(rect().normalized().adjusted ( 0, 0, -25, -25 ) );
// 	QPoint zero(scene()->width() - 500, scene()->height() - 400);
// 	m_grid->setRect(QRectF(mapToScene(zero/2), QSizeF( 500, 400) ));
	
	QGraphicsView::resizeEvent(event);
}

QMouseEvent *KTPaintArea::mapMouseEvent(QMouseEvent *event) const
{
	QMouseEvent *e = new QMouseEvent(event->type(), this->mapToScene(event->pos()).toPoint(), this->mapToScene( event->globalPos() ).toPoint(), event->button(), event->buttons(), event->modifiers () );
			
	return e;
}

void KTPaintArea::frameEvent(KTFrameEvent *event)
{
}

void KTPaintArea::layerEvent(KTLayerEvent *event)
{
}

void KTPaintArea::sceneEvent(KTSceneEvent *event)
{
}

void KTPaintArea::projectEvent(KTProjectEvent *event)
{
}

void KTPaintArea::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawBackground(painter, rect);
	
	painter->setPen( QPen( Qt::black, 3) );
	painter->fillRect(m_drawingRect, Qt::white);
	painter->drawRect( m_drawingRect );
}


