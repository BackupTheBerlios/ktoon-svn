/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
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

#ifndef DTHEMEMANAGER_H
#define DTHEMEMANAGER_H

#include <QXmlDefaultHandler>
#include <QPalette>
#include <dcore/dglobal.h>

class DThemeDocument;

/**
 * @author David Cuadrado
*/
class D_GUI_EXPORT DThemeManager : public QXmlDefaultHandler
{
	public:
		DThemeManager();
		~DThemeManager();
		
		bool applyTheme(const QString &file);
		bool applyTheme(const DThemeDocument &dd);
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );

		bool characters ( const QString & ch );
		
		QColor getColor(const QXmlAttributes& atts);
		
	private:
		QString m_root,m_qname;
		QPalette m_palette;
};

#endif
