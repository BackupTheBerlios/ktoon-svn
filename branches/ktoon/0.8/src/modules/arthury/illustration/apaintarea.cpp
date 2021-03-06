/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "apaintarea.h"
#include "ddebug.h"

#include <QPalette>
#include <QPainter>
#include <QTimer>
#include <QClipboard>
#include <QApplication>
#include <QBoxLayout>

#include "dgradientadjuster.h"
#include <dconfig.h>

#include "adrawcommand.h"

#include <cmath>

#define BEGIN_PAINTER(var) switch(m_renderType) { case KToon::Image: \
var.begin(IMAGE_DEVICE); break; \
	case KToon::Native: \
		var.begin(NATIVE_DEVICE); break; \
	case KToon::OpenGL: \
		var.begin(OPENGL_DEVICE); break; }; 

APaintArea::APaintArea(const QSize& size, KToon::RenderType type, QWidget *parent) : QWidget(parent), m_xpos(0), m_ypos(0), m_offset(0,0), m_drawGrid(true), m_currentTool(0), m_lastPosition(-1,-1), m_currentFrame(0), m_currentFrameIndex(-1), m_layer(0), m_scene(0), m_previousFramesNumber(0), m_zoomFactor(1.0f), m_nextFramesNumber(0), m_currentGraphic(0),  m_size(size), m_history(0)
{
	DINIT;
	
	loadProperties();
	
	m_redrawAll = true;
	
	m_renderType = type;
	
	setAttribute(Qt::WA_StaticContents);
// 	QBoxLayout *layout = new QBoxLayout( QBoxLayout::LeftToRight, this);
	
	switch(m_renderType)
	{
		case KToon::Image:
		{
			m_paintDevice = new AImageDeviceWidget(size, this);
			connect(m_paintDevice, SIGNAL(mousePos(const QPoint &)), this, SIGNAL(mousePos( const QPoint& )));
		}
		break;
		case KToon::OpenGL:
		{
			m_paintDevice = new AGLDevice(this);
			m_paintDevice->resize(size);
		}
		break;
		case KToon::Native:
		{
			m_paintDevice = new QWidget(this);
			m_paintDevice->resize(size);
			m_paintDevice->setAttribute (Qt::WA_PaintOutsidePaintEvent);
		}
		break;
	}
	
	m_paintDevice->setAutoFillBackground(true);
// 	layout->addWidget(m_paintDevice);
	
	fillDevice( m_properties.backgroundColor );
	
	setMouseTracking(true);
	m_grid.setDelta( m_properties.gridSeparation );
	m_grid.createGrid(m_paintDevice->width(), m_paintDevice->height() );
	
	m_path = QPainterPath();
	
	m_currentBrush = new KTBrush;
	
	m_currentFrame = new KTKeyFrame;
	setZoomFactor( 1 );
	setMinimumSize(m_paintDevice->size() + QSize(m_offset.x(),m_offset.y()));
	setFocusPolicy(Qt::ClickFocus);
}


APaintArea::~APaintArea()
{
	DEND;
	if ( m_currentBrush ) delete m_currentBrush;
	
	DCONFIG->beginGroup("PaintArea");
	
	DCONFIG->setValue("GridColor", m_properties.gridColor);
	DCONFIG->setValue("BackgroundColor", m_properties.backgroundColor);
	DCONFIG->setValue("OnionSkinColor", m_properties.onionSkinColor);
	DCONFIG->setValue("OnionSkinBackground", m_properties.onionSkinBackground);
}

QSize APaintArea::sizeHint() const
{
	return m_paintDevice->size() + QSize(m_offset.x()/*+10*/,m_offset.y()/*+10*/);
}

QSize APaintArea::minimumSizeHint () const
{
	return QSize(m_offset.x(),m_offset.y());
}

void APaintArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;
	
	if ( m_redrawAll )
	{
		fillDevice( m_properties.backgroundColor );
		
		BEGIN_PAINTER(painter);
		
		painter.scale(m_zoomFactor,m_zoomFactor);
		painter.setRenderHint(QPainter::Antialiasing);
		
		if(m_drawGrid)
		{
			painter.save();
			painter.setPen(m_properties.gridColor);
			painter.drawPath(m_grid.pathGrid());
			painter.restore();
		}
		
		painter.save();
		
// 		painter.setClipRect(e->rect());
		draw(&painter);
		
		painter.restore();
#if 0
		if(m_currentFrame)
		{
			painter.save();
			foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
			{
				painter.setPen(QPen(Qt::blue, 5)); //FIXME: configure
				if(!component->controlPoints().isEmpty())
				{
					painter.drawPoints(component->controlPoints());
				}
			}
			painter.restore();
		}
#endif
		painter.end();
		m_redrawAll = false;
	}
	m_paintDevice->update();
}

void APaintArea::setKeyFrame(int index)
{
// 	dDebug() << "APaintArea::setKeyFrame(" << index << ")";
	if ( m_layer )
	{
		if ( index >= 0 && index < m_layer->frames().count() )
		{
			KTKeyFrame *frame = m_layer->frames()[index];

			
			if (frame)
			{
				m_currentFrame = frame;
				m_currentFrameIndex = index;
				redrawAll();
			}
			else
			{
				dFatal() << "Frame not exists!!!";
			}
		}
	}
}

void APaintArea::setLayer(int index)
{
// 	dDebug( ) << "APaintArea::setLayer(" << index << ")";
	if ( m_scene )
	{
		if ( index >= 0 && index < m_scene->layers().count() )
		{
			KTLayer *layer = m_scene->layers()[index];
			
			if (layer )
			{
				m_layer = layer;
			}
			else
			{
				dFatal() << "Layer not exists!!!";
			}
		}
	}
}

void APaintArea::setScene(KTScene *scene)
{
	if (scene )
	{
		m_scene = scene;
		m_layer = m_scene->currentLayer();
		setKeyFrame( 0 );// FIXME
	}
	else
	{
		dFatal() << "Layer not exists!!!";
	}
	
}

void APaintArea::draw(QPainter *painter)
{
// 	D_FUNCINFO;
	
	if ( m_currentFrameIndex < 0 ) return;
	
	Layers layers = m_scene->layers();
	Layers::iterator layerIterator = layers.begin();
	
	int index = m_currentFrameIndex/*m_layer->frames().indexOf(m_currentFrame)*/;
	
	if ( index < 0 ) return;
	// draw visible layers
	while ( layerIterator != layers.end() )
	{
		if ( (*layerIterator) && (*layerIterator)->isVisible() )
		{
			// hasta indice - previews y indice + next
			
			float intensitive = 1;
			float pfactor = (double)1 / (double) m_previousFramesNumber;
			
			for (int frameIndex = index-1; frameIndex > index-m_previousFramesNumber-1; frameIndex-- )
			{
				intensitive -= pfactor;
				if ( frameIndex < 0 || frameIndex > (*layerIterator)->frames().count()-1 )
				{
					break;
				}
				
				drawFrame((*layerIterator)->frames()[ frameIndex ], painter, fabs(intensitive) );
			}
			
			intensitive = 1;
			pfactor = (double) 1 / (double)m_nextFramesNumber;
			for (int frameIndex = index+1; frameIndex < index+m_nextFramesNumber+1; frameIndex++ )
			{
				intensitive -= pfactor;
				
				if ( frameIndex > (*layerIterator)->frames().count()-1 )
				{
					break;
				}
				drawFrame((*layerIterator)->frames()[ frameIndex ], painter, fabs(intensitive) );
			}
			
			// Draw the current frame
			if ( index < (*layerIterator)->frames().count() )
			{
				KTKeyFrame *frame = (*layerIterator)->frames()[ index ];
				
				if(frame )
				{
					drawFrame(frame, painter);
				}
			}
		}
		
		++layerIterator;
	}
}

void APaintArea::drawFrame(const KTKeyFrame *frame, QPainter *painter, float intensitive)
{
// 	D_FUNCINFO;
	if ( frame  )
	{
		QList<AGraphicComponent *> componentList = frame->components();
		
		if ( componentList.count() > 0)
		{
			QList<AGraphicComponent *>::iterator it = componentList.begin();
				
			while ( it != componentList.end() )
			{
				if ( *it )
				{
					drawGraphic( *it, painter, intensitive);
				}
				++it;
			}
		}
		
		if(frame == m_currentFrame)
		{
			painter->save();
			painter->setPen(QPen(Qt::blue, 5)); //FIXME: configure 
			QList<AGraphicComponent *> selectedComponents = m_currentFrame->selectedComponents();
			foreach(AGraphicComponent *select, componentList)
			{
				if( !select->controlPoints().isEmpty())
				{
					painter->drawPoints(select->controlPoints());
				}
			}
			painter->restore();
		}
		
		
		
	}
}

void APaintArea::drawGraphic(const AGraphicComponent *graphicComponent, QPainter *painter, float intensitive )
{
	painter->save();
	foreach(AGraphic *graphic, graphicComponent->graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;

		if ( intensitive < 1 && intensitive >= 0 )
		{
			QColor penColor = m_properties.onionSkinColor;
			QColor brushColor = m_properties.onionSkinBackground;
			
			const float factor = fabs(intensitive)+0.1; // FIXME: try to  fix ;)
			
			penColor.setHsv(penColor.hue(),penColor.saturation(), 255 - (static_cast<int>(factor * (pen.color().alpha()+1)) % 255) );
						
			brushColor.setHsv(brushColor.hue(),brushColor.saturation(), 255-(static_cast<int>( factor * (brush.color().alpha())+1 ) % 255 ) );
			
// 			penColor.setHsv(0,0, factor * pen.color().value() );
// 			brushColor.setHsv(0,0, factor * brush.color().value() );
			
			penColor.setAlpha(pen.color().alpha() );
			brushColor.setAlpha(brush.color().alpha() );
			
			pen = QPen(penColor,1, Qt::DashDotDotLine);
			brush = QBrush(brushColor, Qt::Dense7Pattern);
		}
		else if ( intensitive > 1 && intensitive < 0)
		{
			dWarning() << "Intensitive must be lesser than 1 - No effect";
		}
		
		painter->setPen(pen);
		painter->setBrush(brush);
		
		if ( !graphic->pixmap.isNull() )
		{
			painter->drawPixmap(graphic->path.boundingRect().topLeft(), graphic->pixmap);
		}
		
#if 1
		painter->drawPath(graphic->path);
#else	
		QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();		
		if ( poligons.count() == 1 )
		{
			painter->drawPath(graphic->path);
		}
		else
		{
			QList<QPolygonF>::const_iterator it;
			for(it = poligons.begin(); it != poligons.end(); ++it)
			{
				painter->drawPolygon(*it);
			}
		}
#endif	
	}
	
	const QList< AGraphicComponent *> childs = graphicComponent->childs();
	if(childs.count() > 0)
	{
		foreach(AGraphicComponent *child, childs)
		{
			drawGraphic(child, painter, intensitive );
		}
	}
	
	
	painter->restore();
}

void APaintArea::redrawAll()
{
	setUpdatesEnabled(false);
	m_redrawAll = true;
	QTimer::singleShot(0, this, SLOT(update()));;
	setUpdatesEnabled(true);
}

void APaintArea::aUpdate(const QRect &rect)
{
	QPainter painter;
	BEGIN_PAINTER(painter);

	if ( !m_overBuffer.isNull())
	{
		painter.drawImage(m_overBufferRect, m_overBuffer);
		
// 		QTimer::singleShot(0, m_paintDevice, SIGNAL(update()));
		m_paintDevice->update(m_overBufferRect);
	}
	
	switch(m_renderType)
	{
		case KToon::Image:
		{
			m_overBuffer = IMAGE_DEVICE->copy( rect );
		}
		break;
		case KToon::Native:
		{
		}
		break;
		case KToon::OpenGL:
		{
			m_overBuffer = OPENGL_DEVICE->grabFrameBuffer().copy( rect );
		}
	}
	m_overBufferRect = rect;
}

void APaintArea::drawGhostGraphic(const QPainterPath &path)
{
	QMatrix m;
	m.scale(m_zoomFactor,m_zoomFactor);
	aUpdate(m.mapRect(path.boundingRect().toRect().normalized().adjusted(-60,-60,60,60) ));
	
	QPainter painter;
	BEGIN_PAINTER(painter);
	
// 	painter.drawImage(m_overBufferRect, m_overBuffer);
	painter.scale(m_zoomFactor,m_zoomFactor);
	painter.setPen(QPen(Qt::gray, 2, Qt::DotLine));
	painter.drawPath(path);
}

void APaintArea::resizeEvent( QResizeEvent * event )
{
// 	m_xpos = width() / 2 - m_paintDevice->width() / 2;
// 	m_ypos = height() / 2 - m_paintDevice->height() / 2;
// 	update();
	m_size = size();
	
	QWidget::resizeEvent(event);
}

QPoint APaintArea::paintDevicePosition() const
{
	return QPoint(m_xpos, m_ypos);
}

void APaintArea::setOffset(const QPoint & zero)
{
	m_offset = zero *2;
	m_paintDevice->move(zero);
// 	resize(m_paintDevice->sizeHint());
// 	update();
	adjustSize();
}

QWidget *APaintArea::paintDevice() const
{
	return m_paintDevice;
}

// void APaintArea::setPaintDevice(const QImage &image)
// {
// 	m_paintDevice = image.convertToFormat(QImage::Format_RGB32);
// 	update();
// 	updateGeometry();
// }

void APaintArea::mousePressEvent ( QMouseEvent * e )
{
	m_overBuffer = QImage();
	QPoint pos = (e->pos()-QPoint(m_offset.x()/2, m_offset.y()/2)) / m_zoomFactor;
	
	
	
	QMouseEvent *event = new QMouseEvent( e->type(), pos, mapToGlobal(pos ), e->button(), e->buttons(), 0 );
	
	if ( m_currentFrame )
	{
		if (event->button() == Qt::LeftButton && !m_currentFrame->isLocked())
		{
			if (m_currentTool )
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);

				painter.scale(m_zoomFactor,m_zoomFactor);

				QList<AGraphicComponent *> components =  m_currentFrame->components();
				QList<AGraphicComponent *>::iterator it;
				AGraphicComponent *toSelect = 0;
				for(it = components.end()-1; it != components.begin()-1; it--)
				{
					if( (*it) && (*it)->intersects( QRectF(QPointF(static_cast<double>(event->pos().x()-1), static_cast<double>(event->pos().y()-1) ), QSizeF(2,2) ) ) )
					{
						if ( *it )
						{
							if (!toSelect)
							{
								toSelect = (*it);
							}
							else if ( toSelect->boundingRect().contains((*it)->boundingRect() ) )
							{
								toSelect = (*it);
							}
							
// 							break;
						}
					}
				}
				
				if(!toSelect)
				{
					m_currentFrame->clearSelections();
				}
				else if ( e->modifiers() & Qt::ControlModifier )
				{
					m_currentFrame->addSelectedComponent( toSelect );
				}
				else
				{
					m_currentFrame->clearSelections();
					m_currentFrame->addSelectedComponent( toSelect );
				}
				
				if ( m_currentFrame->hasSelections() )
				{
					ADrawCommand *command = new ADrawCommand(m_currentFrame);
					addCommand( command, false);
				}
				
				m_currentGraphic = new AGraphicComponent;
				
				QRect rect = m_currentTool->press(m_currentKeyTool, painter, event->pos(), m_currentFrame);
				QMatrix matrix;
				matrix.scale(m_zoomFactor,m_zoomFactor);
				matrix.translate(m_xpos, m_ypos);
				update( matrix.mapRect(rect));
			}
			
			m_lastPosition = event->pos();
		}
	}
}

void APaintArea::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pos = (e->pos()-QPoint(m_offset.x()/2, m_offset.y()/2))/ m_zoomFactor;

	QMouseEvent *event = new QMouseEvent( e->type(), pos, mapToGlobal(pos ), e->button(), e->buttons(), 0 );
	emit mousePos(e->pos());
	
	if ( m_currentFrame )
	{
		if ((event->buttons() & Qt::LeftButton) && m_lastPosition != QPoint(-1, -1)  && !m_currentFrame->isLocked() ) // TODO: Mover debe entrar al plugin, al igual que los modificadores del mouse
		{
			if (m_currentTool)
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);
				painter.scale(m_zoomFactor,m_zoomFactor);
				QMatrix matrix;
				matrix.scale(m_zoomFactor,m_zoomFactor);
				QRect rect = m_currentTool->move(m_currentKeyTool, painter, m_lastPosition, event->pos());
				
				m_paintDevice->update(matrix.mapRect(rect));
			}
	
			m_lastPosition = event->pos();
		}
	}
}

void APaintArea::mouseReleaseEvent(QMouseEvent *e)
{
	QPoint pos = (e->pos()-QPoint(m_offset.x()/2, m_offset.y()/2))/ m_zoomFactor;
	
	
	QMouseEvent *event = new QMouseEvent( e->type(), pos, mapToGlobal(pos ), e->button(), e->buttons(), 0 );
	if ( m_currentFrame )
	{
		if (event->button() == Qt::LeftButton && m_lastPosition != QPoint(-1, -1)  && !m_currentFrame->isLocked())
		{
			if (m_currentTool)
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);
				painter.scale(m_zoomFactor,m_zoomFactor);
				QRect rect = m_currentTool->release(m_currentKeyTool, painter, event->pos());
				QMatrix matrix;
				matrix.scale(m_zoomFactor,m_zoomFactor);
				
				m_paintDevice->update(matrix.mapRect(rect));
				rect.translate(m_xpos, m_ypos);
				
				
				if ( !m_currentTool->path().isEmpty())
				{
					m_currentGraphic->addGraphic( m_currentTool->path(), painter.pen(), painter.brush());
				}
				
				if ( m_currentGraphic->isValid() && m_currentTool->isComplete() )
				{
// 					dDebug() << "Adding component";
					
					
					AAddGraphicCommand *command = new AAddGraphicCommand(m_currentFrame, m_currentGraphic);
					
					addCommand( command );
					
// 					dDebug() << "Components count: " << m_currentFrame->components().count();
				}
				
				if ( m_renderType == KToon::OpenGL ) // FIXME: remove for Qt 4.2
				{
					redrawAll();
				}
			}
			
			m_lastPosition = QPoint(-1, -1);
		}
	}
}

void APaintArea::wheelEvent( QWheelEvent *event )
{
	if(event->modifiers () == Qt::CTRL)
	{
		float f;
		f = m_zoomFactor + 0.001*event->delta();
		if( f < 32.0/ m_paintDevice->width() )
		{
			f = 32.0/m_paintDevice->width();
		}
		setZoomFactor( f );
	}
	else
	{
		QWidget::wheelEvent(event );
	}
}

void APaintArea::keyPressEvent(QKeyEvent *event)
{
	QList<AGraphicComponent *> components = m_currentFrame->selectedComponents();
	
	if ( event->key() == Qt::Key_Up)
	{
		foreach(AGraphicComponent *component, components )
		{
			QPointF pos = component->currentPosition();
			
			component->translate(pos.x(), pos.y()-10);
		}
	}
	else if ( event->key() == Qt::Key_Down )
	{
		foreach(AGraphicComponent *component, components )
		{
			QPointF pos = component->currentPosition();
			
			component->translate(pos.x(), pos.y()+10);
		}
	}
	else if ( event->key() == Qt::Key_Right)
	{
		foreach(AGraphicComponent *component, components )
		{
			QPointF pos = component->currentPosition();
			
			component->translate(pos.x()+10, pos.y());
		}
	}
	else if ( event->key() == Qt::Key_Left)
	{
		foreach(AGraphicComponent *component, components )
		{
			QPointF pos = component->currentPosition();
			
			component->translate(pos.x()-10, pos.y());
		}
	}
	else
	{
		event->ignore();
		return;
	}
	
	redrawAll();
	event->accept();
}

void APaintArea::setTool( AToolInterface *toolIface, const QString &tool)
{
	if ( m_currentTool )
	{
		m_currentTool->aboutToChangeTool();
// 		if ( m_currentFrame && m_currentGraphic)
// 		m_currentFrame->addComponent(  m_currentGraphic );
	}
	
	m_currentTool = toolIface;
	m_currentKeyTool = tool;
}

void APaintArea::setPen(const QPen &pen)
{
	QPen newPen = pen;
	newPen.setBrush( m_currentBrush->pen().brush() );
	
	m_currentBrush->setPen( newPen );
}

void APaintArea::setColors(const QBrush &foreground, const QBrush &background)
{
	m_currentBrush->setPenBrush( foreground );
	m_currentBrush->setBrush( background );
}



void APaintArea::setPreviousFrames(int n)
{
	if ( n >= 0 )
	{
		m_previousFramesNumber = n;
		redrawAll();
	}
}

void APaintArea::setNextFrames(int n)
{
	if ( n >= 0)
	{
		m_nextFramesNumber = n;
		redrawAll();
	}
}

void APaintArea::removeSelectsGraphics()
{
	m_currentFrame->removeSelections();
	redrawAll();
}


AGraphicComponent *APaintArea::currentGraphic()
{
	return m_currentGraphic;
}

KTKeyFrame *APaintArea::currentFrame() const
{
	return m_currentFrame;
}

void APaintArea::copy()
{
	QRectF bound;
	m_copiedGraphics.clear();
	if(m_currentFrame->selectedComponents().count() > 0)
	{
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
			AGraphicComponent *comp =  new AGraphicComponent(*component);
			comp->setSelected(false);
			m_copiedGraphics << comp;
			bound |= component->boundingRect();
		}
	}
	
	bound.adjust(-1,-1,1,1);
	
	// To clipboard
	QPixmap image(QSize(bound.width(), bound.height()));
	
	image.fill(Qt::transparent);
	
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	
	painter.translate(QPoint(m_xpos, m_ypos)-bound.topLeft());
	
	foreach (AGraphicComponent *component, m_copiedGraphics)
	{
		component->draw(&painter);
	}
	
	QApplication::clipboard()->setPixmap(image);
}

void APaintArea::paste()
{
	foreach(AGraphicComponent *component, m_copiedGraphics)
	{
		m_currentFrame->addComponent( new AGraphicComponent(*component));
		
		m_currentFrame->clearSelections();
	}
	redrawAll();
}

void APaintArea::cut()
{
	copy();
	m_currentFrame->removeSelections();
	redrawAll();
}


void APaintArea::group()
{
	D_FUNCINFO;
	
	if(m_currentFrame->selectedComponents().count() > 1)
	{
		AGraphicComponent *newComponent = new AGraphicComponent();
	
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
// 			AGraphicComponent *child = new AGraphicComponent(*component);
 			component->setControlPoints(QPolygonF());
			newComponent->addChild( new AGraphicComponent(*component) );
		}
		m_currentFrame->removeSelections();
		
		m_currentFrame->addComponent( newComponent );
		m_currentFrame->addSelectedComponent( newComponent );
		redrawAll();
	}
}

void APaintArea::ungroup()
{
	D_FUNCINFO;
	if(m_currentFrame->selectedComponents().count() > 0)
	{
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
			
			if(component->hasChilds())
			{
				
				foreach(AGraphicComponent *child, component->childs())
				{
					m_currentFrame->addComponent( child );
					component->removeChild(child);
				}
				m_currentFrame->removeComponent(component);
			}
		}
	redrawAll();
	}
}

void APaintArea::selectAll()
{
	D_FUNCINFO;
	m_currentFrame->selecteAllComponents();
	redrawAll();
}

void  APaintArea::bringToFromSelected()
{
	if(m_currentFrame )
	{
		m_currentFrame->bringToFromSelected();
		redrawAll();
	}
}

void APaintArea::sendToBackSelected()
{
	if(m_currentFrame )
	{
		m_currentFrame->sendToBackSelected();
		redrawAll();
	}
}


void APaintArea::oneStepForwardSelected()
{
	if(m_currentFrame )
	{
		m_currentFrame->oneStepForwardSelected();
		redrawAll();
	}
}

void APaintArea::oneStepBackwardSelected()
{
	if(m_currentFrame )
	{
		m_currentFrame->oneStepBackwardSelected();
		redrawAll();
	}
}
void APaintArea::setZoomFactor( double f )
{
	int w, h;
	
	if( f == m_zoomFactor )
		return;

	if(m_zoomFactor > 2.0 && (f - m_zoomFactor) > 0 || m_zoomFactor < 0.26 && (f - m_zoomFactor) < 0 )
	{
		return;
	}
	QPointF delta( f/m_zoomFactor, f/m_zoomFactor);
	
	QRect rect = m_paintDevice->rect();
	QMatrix matrix;
	matrix.scale(delta.x(), delta.y());
	rect = matrix.mapRect(rect);
	
	w = rect.width();
	h = rect.height();
	m_paintDevice->resize(w, h );
// 	setMinimumSize(m_paintDevice->size() + QSize(m_offset.x(), m_offset.y()));
	m_zoomFactor = f;
	emit changedZoomFactor(f);
	if ( delta.x() > 0 && delta.y() > 0 )
	{
		m_currentFrame->scale( delta.x(),  delta.y());
	}
	
	m_xpos = width() / 2 - m_paintDevice->width() / 2;
	m_ypos = height() / 2 - m_paintDevice->height() / 2;
	
	setUpdatesEnabled(false);
	adjustSize();
	
	redrawAll();
	setUpdatesEnabled(true);
}

void APaintArea::zoomIn()
{
	setZoomFactor( m_zoomFactor + 0.05 );
}

void APaintArea::zoomOut()
{
	setZoomFactor( m_zoomFactor -0.05 );
}

void APaintArea::flipVCurrentElement()
{
	D_FUNCINFO;
	if(m_currentFrame->selectedComponents().count() > 0)
	{
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
			
			component->flip(Qt::Vertical, component->boundingRect().center());
		}
		redrawAll();
	}
}

void APaintArea::flipHCurrentElement()
{
	if(m_currentFrame->selectedComponents().count() > 0)
	{
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
			component->flip(Qt::Horizontal, component->boundingRect().center());
		}
		redrawAll();
	}
}


void APaintArea::setProperties(const KTPaintAreaProperties &properties)
{
	m_properties = properties;
	
	m_grid.setDelta( m_properties.gridSeparation );
	
	dError() << "SET DELTA: " << m_properties.gridSeparation;
	m_grid.createGrid(m_paintDevice->width(), m_paintDevice->height() );
	
	redrawAll();
}

void APaintArea::loadProperties()
{
	DCONFIG->beginGroup("PaintArea");
	
	m_properties.gridColor = qvariant_cast<QColor>(DCONFIG->value("GridColor", QColor(Qt::gray)));
	
	m_properties.backgroundColor = qvariant_cast<QColor>(DCONFIG->value("BackgroundColor", QColor(Qt::white)));
	
	m_properties.onionSkinColor = qvariant_cast<QColor>(DCONFIG->value("OnionSkinColor", QColor(Qt::lightGray)));
	
	m_properties.onionSkinBackground = qvariant_cast<QColor>(DCONFIG->value("OnionSkinBackground", QColor(Qt::lightGray)));
}

void APaintArea::setHistory(DCommandHistory *history)
{
	m_history = history;
	
	connect(m_history, SIGNAL(modified()), this, SLOT(redrawAll()));
}

void APaintArea::addCommand(DCommand *command, bool execute)
{
	if ( m_history )
	{
		m_history->addCommand( command,execute );
	}
}


