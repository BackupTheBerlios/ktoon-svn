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
 
#ifndef KTPROJECTPARSER_H
#define KTPROJECTPARSER_H

#include <QXmlDefaultHandler>
#include <QSize>
#include <QBrush>
#include <QPen>

#include "agraphiccomponent.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTProjectParser : public QObject, public QXmlDefaultHandler
{
	Q_OBJECT
	public:
		KTProjectParser();
		~KTProjectParser();
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		QString partName() const;
		QStringList locations() const;
		
		QSize documentSize() const;

	signals:
		void createLayer();
		void createFrame();
		void createComponent(AGraphicComponent *component);
		
	private:
		QString m_root,m_qname;
		QList<AGraphicComponent *> m_components;
		
		QString m_partName;
		QStringList m_locations;
		
		QStringList m_polygons;
		
		QSize m_documentSize;
		
		QBrush m_brush;
		QPen m_pen;
		QGradient *m_gradient;
		QGradientStops m_gradientStops;
		
		QList<AGraphic *> m_graphics;
		
		AGraphicComponent *m_currentComponent;
		AGraphicComponent *m_rootComponent;
		
		int m_tagCounter;
};

#endif
