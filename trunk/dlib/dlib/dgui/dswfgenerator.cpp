/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include "dswfgenerator.h"

#include <QPaintEngine>
#include <QPainterPath>
#include <QBuffer>

#include <QtDebug>

#include <mingpp.h>

class DMingPaintEngine : public QPaintEngine
{
	public:
		DMingPaintEngine();
		~DMingPaintEngine();
		
		virtual bool begin ( QPaintDevice * pdev );
		virtual void drawPixmap ( const QRectF & r, const QPixmap & pm, const QRectF & sr );
		virtual void drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode );
		virtual void drawPolygon ( const QPoint * points, int pointCount, PolygonDrawMode mode );
		virtual void drawPath(const QPainterPath & path);
		virtual bool end();
		virtual QPaintEngine::Type type() const;
		virtual void updateState( const QPaintEngineState & state);
		
		
	public:
		void nextFrame();
		SWFDisplayItem *addToFrame(SWFBlock *item);
		void save(const QString &filePath);
		
	private:
		SWFShape *createShape();
		void propertiesForShape(SWFShape *shape);
		
	private:
		SWFMovie *m_movie;
		QList<SWFShape *> m_shapes;
		QList<SWFDisplayItem *> m_currentFrame;
		
		struct {
			QPointF brushOrigin;
			QBrush backgroundBrush;
			QBrush brush;
			QPen pen;
		} m_state;
};

DMingPaintEngine::DMingPaintEngine() : QPaintEngine(QPaintEngine::PrimitiveTransform | QPaintEngine::PatternBrush | QPaintEngine::LinearGradientFill | QPaintEngine::RadialGradientFill | QPaintEngine::ConicalGradientFill | QPaintEngine::AlphaBlend | QPaintEngine::PorterDuff | QPaintEngine::PainterPaths | QPaintEngine::Antialiasing | QPaintEngine::ConstantOpacity | QPaintEngine::PaintOutsidePaintEvent)
{
	Ming_init();
	Ming_useSWFVersion(7);
	
	m_movie = new SWFMovie;
	
	m_movie->setDimension(300, 300 );
// 	m_movie->setBackground(0xff, 0xff, 0xff );
// 	m_movie->setRate(14);
}

DMingPaintEngine::~DMingPaintEngine()
{
// 	Ming_collectGarbage();
	Ming_cleanup();
	
	delete m_movie;
}

bool DMingPaintEngine::begin( QPaintDevice *pdev)
{
	
	
	return true;
}


void DMingPaintEngine::drawPixmap ( const QRectF & r, const QPixmap & pm, const QRectF & sr )
{
	QImage image = pm.toImage();
	
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG");
	
	SWFInput input((unsigned char*) ba.data(), ba.size());
	SWFBitmap *bitmap = new SWFBitmap( &input ); // FIXME: hay que borrar el puntero
	
	SWFShape *shape = createShape();
	shape->addBitmapFill(bitmap);
	addToFrame(shape);
}

void DMingPaintEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
	SWFShape *shape = createShape();
	
	shape->movePenTo(points[0].x(), points[0].y());
	
	for(int i = 1; i < pointCount; i++)
	{
		shape->drawLineTo(points[i].x(), points[i].y());
	}
	
	shape->drawLineTo(points[0].x(), points[0].y()); // FIXME: It's ok?
	
	addToFrame(shape);
}

void DMingPaintEngine::drawPolygon ( const QPoint *points, int pointCount, PolygonDrawMode mode )
{
	SWFShape *shape = createShape();
	
	
	shape->setLine(1, 0, 0, 0);
	
	shape->movePenTo(points[0].x(), points[0].y());
	
	for(int i = 0; i < pointCount; i++)
	{
		shape->drawLineTo(points[i].x(), points[i].y());
	}
	
	addToFrame(shape);
}

void DMingPaintEngine::drawPath(const QPainterPath & path)
{
	SWFShape *shape = createShape();
	
	shape->setLine(1, 0, 0, 0);
	
	QVector<QPointF> curve;
	
	for(int i = 0; i < path.elementCount(); i++)
	{
		QPainterPath::Element e = path.elementAt(i);
		
		qDebug() << e;
		
		switch(e.type)
		{
			case QPainterPath::MoveToElement:
			{
				shape->movePenTo(e.x, e.y);
				break;
			}
			case QPainterPath::LineToElement:
			{
				shape->drawLine(e.x, e.y);
				break;
			}
			case QPainterPath::CurveToDataElement:
			{
				curve << e;
				if(curve.count() == 3)
				{
					shape->drawCubicTo(curve[0].x(),curve[0].y(), curve[1].x(), curve[1].y(), curve[2].x(), curve[2].y());
				}
				break;
			}
			case QPainterPath::CurveToElement:
			{
				curve.clear();
				curve << e;
				break;
			}
		}
	}
	addToFrame(shape);
}

bool DMingPaintEngine::end()
{
	return true;
}


QPaintEngine::Type DMingPaintEngine::type() const
{
	return QPaintEngine::User;
}


void DMingPaintEngine::updateState(const QPaintEngineState & state)
{
	m_state.pen = state.pen();
	
	m_state.brushOrigin = state.brushOrigin();
	
	switch( state.state() )
	{
		case QPaintEngine::DirtyBrush:
		{
			m_state.brush = state.brush();
		}
		break;
		case QPaintEngine::DirtyBackground:
		{
			m_state.backgroundBrush = state.backgroundBrush();
		}
		break;
		default: break;
	}
}

SWFDisplayItem *DMingPaintEngine::addToFrame(SWFBlock *shape)
{
	SWFDisplayItem *item = m_movie->add(shape);
	m_currentFrame << item;
	
	return item;
}

void DMingPaintEngine::nextFrame()
{
	m_movie->nextFrame();
	
	foreach(SWFDisplayItem *item, m_currentFrame)
	{
		m_movie->remove(item);
	}
	qDeleteAll(m_currentFrame);
	m_currentFrame.clear();
}

void DMingPaintEngine::save(const QString &filePath)
{
	m_movie->save(filePath.toLocal8Bit().data());
}

SWFShape *DMingPaintEngine::createShape()
{
	SWFShape *shape = new SWFShape;
	
	m_shapes << shape;
	
	propertiesForShape(shape);
	
	return shape;
}

void DMingPaintEngine::propertiesForShape(SWFShape *shape)
{
	QBrush brush = m_state.brush;
	if( brush.gradient() )
	{
		qFatal("NO BRUSH GRADIENT SUPPORT");
	}
	else if (!brush.texture().isNull() )
	{
		qFatal("NO BRUSH TEXTURE SUPPORT");
	}
	else
	{
		QColor brushColor = brush.color();
		SWFFill *fill = shape->addSolidFill(brushColor.red(), brushColor.green(), brushColor.blue(), brushColor.alpha());
		
		fill->moveTo(m_state.brushOrigin.x(),m_state.brushOrigin.y());
	}
	
	QColor penColor = m_state.pen.color();
	shape->setLine(m_state.pen.width(), penColor.red(), penColor.green(), penColor.blue(), penColor.alpha());
}

/////////////////////////////////// DSwfGenerator //////////////////////////////////////

struct DSwfGenerator::Private
{
	DMingPaintEngine *paintEngine;
};

DSwfGenerator::DSwfGenerator()
 : QPaintDevice(), d(new Private)
{
	d->paintEngine = new DMingPaintEngine;
	
	nextFrame();
}


DSwfGenerator::~DSwfGenerator()
{
	delete d->paintEngine;
	delete d;
}

QPaintEngine *DSwfGenerator::paintEngine() const
{
	return d->paintEngine;
}

void DSwfGenerator::nextFrame()
{
	d->paintEngine->nextFrame();
}

void DSwfGenerator::save(const QString &filename)
{
	d->paintEngine->save(filename);
}




