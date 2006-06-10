/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktgraphicelement.h"
#include "ddebug.h"
#include <QPixmap>
#include <QHash>
#include <dalgorithm.h>
#include <dmd5hash.h>
#include <QFile>

KTGraphicElement::KTGraphicElement()
{
}

KTGraphicElement::KTGraphicElement(const KTGraphicElement &toCopy) : path(toCopy.path), brush(toCopy.brush), pen(toCopy.pen), pixmap(toCopy.pixmap), m_origPixmap(toCopy.m_origPixmap), m_pixmapHash(toCopy.m_pixmapHash)
{
}

KTGraphicElement::~KTGraphicElement()
{
}

void KTGraphicElement::mapTo(const QMatrix& matrix)
{
	path = matrix.map(path);
	
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
}

void KTGraphicElement::mapPixmap(const QMatrix &matrix)
{
	if ( !pixmap.isNull() )
	{
		pixmap = m_origPixmap.transformed(matrix, Qt::SmoothTransformation);
	}
}


void KTGraphicElement::flip(Qt::Orientation o)
{
	QPointF pos = path.boundingRect().center();
	flip(o, pos );
}

void KTGraphicElement::flip(Qt::Orientation o, const QPointF &pos )
{
	QMatrix  matrix;
	matrix.translate(-pos.x(),-pos.y());
	
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
	
	QList<QPolygonF> pols = path.toSubpathPolygons(matrix);
	
	QList<QPolygonF>::iterator itPol = pols.begin();
	path = QPainterPath();
	
	QPolygonF result;
	
	while(itPol != pols.end())
	{
		QPolygonF::iterator itPoint = (*itPol).begin();
		while(itPoint != (*itPol).end())
		{
			if ( o == Qt::Horizontal)
			{
				(*itPoint).setY(/*pos.y()*/-(*itPoint).y());
			}
			else
			{
				(*itPoint).setX(-(*itPoint).x());
			}
			result.append(*itPoint);
			++itPoint;
		}
// 		path.addPolygon((*itPol));
// 		path.closeSubpath();
		++itPol;
	}
	
	path.addPolygon(result);
	brush = DBrushAdjuster::flipBrush( brush, o );
	pen.setBrush( DBrushAdjuster::flipBrush( pen.brush(), o ));
	
	matrix.reset();
	matrix.translate(pos.x(), pos.y()/*-path.currentPosition().y()*/);
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
	path = matrix.map(path);
	
	if ( !pixmap.isNull() )
	{
		QImage original = pixmap.toImage();
		
		QImage result = original;
		
		if ( o == Qt::Horizontal)
		{
			for (int y = 0; y < original.height(); ++y) 
			{
				for (int x = 0; x < original.width(); ++x) 
				{
					int pixel = original.pixel(original.width() - x - 1, y);
					result.setPixel(x, y, pixel);
				}
			}
		}
		else
		{
			for (int y = 0; y < original.height(); ++y) 
			{
				for (int x = 0; x < original.width(); ++x) 
				{
					int pixel = original.pixel(x, original.height() - y - 1);
					result.setPixel(x, y, pixel);
				}
			}
		}
		
		setPixmap( QPixmap::fromImage(result));
	}
}



void KTGraphicElement::setPixmap(const QPixmap &pix, const QString &key)
{
	if ( pix.isNull() ) return;
	
	pixmap = pix;
	m_origPixmap = pix;
	
	if ( key.isNull() )
	{
		m_pixmapHash = DMD5Hash::hashData((const char *)pix.toImage().bits(), 4*pix.height()*pix.width()) ;
	}
	else
	{
		m_pixmapHash = key;
	}
}

QString KTGraphicElement::pixmapHash() const
{
	return m_pixmapHash;
}

void KTGraphicElement::savePixmap(const QString &path)
{
	if ( !m_origPixmap.isNull() )
	{
		QString file = path+"/"+m_pixmapHash;
		
		if ( !QFile::exists(file) )
		{
			m_origPixmap.save(file, "PNG" );
		}
	}
}

