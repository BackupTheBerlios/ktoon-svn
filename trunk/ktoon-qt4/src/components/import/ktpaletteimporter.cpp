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

#include "ktpaletteimporter.h"
#include <QFile>
#include <QTextStream>

#include "ktdebug.h"

KTPaletteImporter::KTPaletteImporter() : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
		
	QDomElement root = createElement( "Palette" );
	appendChild( root );
}


KTPaletteImporter::~KTPaletteImporter()
{
}

void KTPaletteImporter::import(const QString &file, PaletteType pt)
{
	switch(pt)
	{
		case Gimp:
		{
			importGimpPalette(file);
		}
		break;
	}
}

void KTPaletteImporter::importGimpPalette(const QString &file)
{
	QFile f(file);
	
	if ( f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&f);
		
		if ( ! stream.readLine().contains("GIMP Palette") )
		{
			ktError() << "Don't contains \"GIMP Palette\"";
			return;
		}
		
		QString string = "";
		string = stream.readLine();
		
		
		m_paletteName = string.section("Name:", 1).trimmed();
		
		documentElement().setAttribute("name", m_paletteName);
		documentElement().setAttribute("editable", "false");
		
		stream >> string;
		
		if ( ! string.contains("#") )
		{
// 			ktDebug() << "Don't have \'#\' " << string;
// 			return;
			stream.readLine();
		}
		
		while ( !stream.atEnd())
		{
			int r, g, b;
			stream >> r >> g >> b;
			stream >> string;
			
			QColor c(r, g, b);
			
			if ( c.isValid() && !string.isEmpty() && string != "#" )
			{
				QDomElement element = createElement("Color");
				element.setAttribute("colorName", c.name());
				element.setAttribute("alpha", "255");
				
				documentElement().appendChild(element);
			}
			else
			{
				ktError() << "Bad color";
			}
		}
	}
}

void KTPaletteImporter::saveFile(const QString &path)
{
	if ( m_paletteName.isNull () )
		return;
	
	QFile file(path+"/"+m_paletteName.remove(' ')+".ktpl" );
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ts(&file);
		ts << toString();
		
		m_filePath = path+"/"+m_paletteName.remove(' ')+".ktpl";
	}
}

QString KTPaletteImporter::filePath() const
{
	return m_filePath;
}
