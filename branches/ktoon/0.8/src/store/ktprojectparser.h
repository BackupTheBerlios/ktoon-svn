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
#include <QDir>

#include "agraphiccomponent.h"

/**
 * @brief Esta clase es el analizador del archivo del proyecto
 * @author David Cuadrado <krawek@toonka.com>
*/

class Q_DECL_EXPORT KTProjectParser : public QObject, public QXmlDefaultHandler
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTProjectParser();
		
		/**
		 * Destructor por defecto
		 */
		~KTProjectParser();
		
		/**
		 * Analiza etiquetas de apertura del documento XML
		 */
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		/**
		 * Analiza etiquetas de cierre del documento XML
		 */
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		/**
		 * Muestra errores en el analisis del documento
		 */
		bool error ( const QXmlParseException & exception );
		
		/**
		 * Muestra errores fatales en el analisis del documento
		 */
		bool fatalError ( const QXmlParseException & exception );
		
		/**
		 * Retorna el nombre del componente que esta siendo analizado
		 */
		QString partName() const;
		
		/**
		 * Retorna las rutas de componentes
		 */
		QStringList locations() const;
		
		/**
		 * Retorna el tamaño del documento
		 */
		QSize documentSize() const;
		
		
		bool parse(const QString &filePath);

	signals:
		/**
		 * Este signal se emite cuando se requiere crear un layer
		 */
		void createLayer(const QString &name);
		
		/**
		 * Este signal se emite cuando se requiere crear un frame
		 */
		void createFrame(const QString &name, int clones = 0);
		
		/**
		 * Este signal se emite cuando se requiere crear un componente
		 */
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
		
		QDir m_projectDir;
};

#endif
