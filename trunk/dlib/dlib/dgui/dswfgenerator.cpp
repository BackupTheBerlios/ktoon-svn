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

#include <mingpp.h>

class DMingPaintEngine : public QPaintEngine
{
	public:
		DMingPaintEngine();
		~DMingPaintEngine();
		
		virtual bool begin ( QPaintDevice * pdev );
		virtual void drawPixmap ( const QRectF & r, const QPixmap & pm, const QRectF & sr );
		virtual void drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode );
		virtual void drawPath(const QPainterPath & path);
		virtual bool end();
		virtual QPaintEngine::Type type() const;
		virtual void updateState( const QPaintEngineState & state);
		
		
	public:
		void nextFrame();
		void save(const QString &filePath);
		
	private:
		SWFShape *createShape();
		
	private:
		SWFMovie *m_movie;
		QList<SWFShape *> m_shapes;
};

DMingPaintEngine::DMingPaintEngine() : QPaintEngine(QPaintEngine::PatternBrush | QPaintEngine::LinearGradientFill | QPaintEngine::RadialGradientFill | QPaintEngine::ConicalGradientFill | QPaintEngine::AlphaBlend | QPaintEngine::PorterDuff | QPaintEngine::PainterPaths | QPaintEngine::Antialiasing | QPaintEngine::ConstantOpacity | QPaintEngine::PaintOutsidePaintEvent)
{
	Ming_init();
	
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
}

void DMingPaintEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
	for(int i = 0; i < 100; i++)
	{
		SWFShape *shape = createShape();
		
		shape->setLine(1, 0, 0, 0);
		shape->movePenTo(50,50);
		shape->drawCircle(100);
		
		SWFDisplayItem *frame = m_movie->add( shape);
		
		frame->rotate(i*30);
		frame->moveTo(150, 150);
	}
}

void DMingPaintEngine::drawPath(const QPainterPath & path)
{
	SWFShape *shape = createShape();
	shape->drawCircle(100);
	
	m_movie->add( shape);
}

bool DMingPaintEngine::end()
{
	return true;
}


QPaintEngine::Type DMingPaintEngine::type() const
{
	return QPaintEngine::User;
}


void DMingPaintEngine::updateState( const QPaintEngineState & state)
{
}


void DMingPaintEngine::nextFrame()
{
	m_movie->nextFrame();
}

void DMingPaintEngine::save(const QString &filePath)
{
	m_movie->save(filePath.toLocal8Bit().data());
}

SWFShape *DMingPaintEngine::createShape()
{
	SWFShape *shape = new SWFShape;
	
	m_shapes << shape;
	
	return shape;
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




