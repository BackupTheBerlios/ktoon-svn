/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#include "ktpaletteparser.h"
#include <kcore/kdebug.h>

struct KTPaletteParser::Private
{
		QString root, qname;
		QString paletteName;
		bool isEditable;
		QList<QBrush> brushes;
		
		QGradientStops gradientStops;
		QGradient *gradient;
		
		~Private()
		{
			delete gradient;
		}
};

KTPaletteParser::KTPaletteParser(): DXmlParserBase(), d(new Private)
{
// 	DINIT;
	 d->paletteName = "";
	 d->isEditable = false;
	 d->gradient = 0;
}


KTPaletteParser::~KTPaletteParser()
{
	delete d;
// 	DEND;
}


bool KTPaletteParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "Palette")
	{
		if(tag == root())
		{
			d->paletteName = atts.value("name");
			if ( atts.value("editable") == "true" )
			{
				d->isEditable = true;
			}
			else
			{
				d->isEditable = false;
			}
		}
		else if( tag == "Color")
		{
			QColor c = QColor(atts.value("colorName"));
			c.setAlpha( atts.value("alpha").toInt() );
			
			if ( c.isValid() )
			{
				d->brushes << c;
			}
			else
			{
				dError() << "Invalid Color";
			}
		}
		else if(tag == "Gradient" )
		{
			if ( d->gradient ) delete d->gradient;
			d->gradient = 0;
			
			d->gradientStops.clear();
			
			QGradient::Type type = QGradient::Type(atts.value("type").toInt());
			QGradient::Spread spread = QGradient::Spread(atts.value("spread").toInt());
			switch( type )
			{
				case QGradient::LinearGradient:
				{
					d->gradient = new QLinearGradient(atts.value("startX").toDouble(),atts.value("startY").toDouble(),atts.value("finalX").toDouble(), atts.value("finalY").toDouble());
				}
				break;
				case QGradient::RadialGradient:
				{
					d->gradient = new QRadialGradient(atts.value("centerX").toDouble(),atts.value("centerY").toDouble(), atts.value("radius").toDouble(),atts.value("focalX").toDouble(),atts.value("focalY").toDouble() );
				}
				break;
				case QGradient::ConicalGradient:
				{
					d->gradient = new QConicalGradient(atts.value("centerX").toDouble(),atts.value("centerY").toDouble(),atts.value("angle").toDouble());
				}
				break;
				default:
				{
					dFatal() << "No gradient type: " << type;
				}
				break;
				
			}
			d->gradient->setSpread(spread);
		}
		else if(tag == "Stop" )
		{
			QColor c(atts.value("colorName") );
			c.setAlpha(atts.value("alpha").toInt() );
			
			d->gradientStops << qMakePair(atts.value("value").toDouble(), c);	
		}
	}
	return true;
}

bool KTPaletteParser::endTag(const QString& tag)
{
	if ( root() == "Palette" )
	{
		if ( tag == "Gradient" && d->gradient )
		{
			d->gradient->setStops(d->gradientStops);
			d->brushes << *d->gradient;
			d->gradientStops.clear(); 
		}
	}
	return true;
}

void KTPaletteParser::text(const QString& )
{
	
}

QList<QBrush> KTPaletteParser::brushes() const
{
	return d->brushes;
}

QString KTPaletteParser::paletteName() const
{
	return d->paletteName;
}

bool KTPaletteParser::paletteIsEditable() const
{
	return d->isEditable;
}

