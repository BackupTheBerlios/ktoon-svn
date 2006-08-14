/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DDISPLAYPATH_H
#define DDISPLAYPATH_H

#include <QFrame>
#include <QPainterPath>
#include <QImage>
#include <QPen>
#include <QBrush>
#include "dcore/dglobal.h"

/**
 * @if english
 * @short translate me
 * @elseif spanish
 * @short Esta clase provee de un visualizador de path.
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DDisplayPath : public QFrame
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Constructor por defecto.
		 * @endif
		 */
		DDisplayPath(QWidget *parent = 0);
		/**
		 * Destructor
		 */
		~DDisplayPath();
		
		virtual QSize sizeHint() const;
		
	public:
		/**
		* @if english
		* Translate
		* @elseif spanish
		* Pone el "painter path" a visualizar
		* @endif
		 */
		void setPath(const QPainterPath &form);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Pone el grosor de la linea con la que se va a visualizar el "painter path".
		 * @endif
		 */
		void setThickness(int value);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Pone el tipo de linea con la que se va a visualizar el "painter path".
		 * @endif
		 */
		void setPen(const QPen &pen);
		/**
		 * @if english
		 * Translate
		 * @elseif spanish
		 * Pone el tipo de relleno con la que se va a visualizar el "painter path".
		 * @endif
		 */
		void setBrush(const QBrush &brush);
		/**
		 * Devuelve el "painter path" actualmente visualizado.
		 */
		virtual QPainterPath currentPainterPath();
		/**
		 * Devuelve la imagen que genera el visualizar el "painter path".
		 */
		QImage *displayDevice();
		
	private:
		QPainterPath m_currentForm;
		int m_thickness;
		QImage m_displayArea;
		
		QBrush m_brush;
		QPen m_pen;
		
	protected:
		virtual void paintEvent ( QPaintEvent * event );
};

#endif
