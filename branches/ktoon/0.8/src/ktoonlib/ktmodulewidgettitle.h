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

#ifndef KTMODULEWIDGETTITLE_H
#define KTMODULEWIDGETTITLE_H

#include <QFrame>

/**
 * @if english
 * This class represents the KTModuleWidget header
 * @elseif spanish
 * Esta clase representa la cabecera de KTModuleWidget
 * @endif
 * @author David Cuadrado
*/
class Q_GUI_EXPORT KTModuleWidgetTitle : public QFrame
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * Default Constructor
		 * @elseif spanish
		 * Constructor por defecto
		 * @endif
		 * 
		 * @param title 
		 * @param parent 
		 * @return 
		 */
		 KTModuleWidgetTitle(const QString &title, QWidget *parent = 0);
		/**
		 * Destructor
		 * @return 
		 */
		~KTModuleWidgetTitle();
		/**
		 * @if english
		 * Reimplemented from QWidget, this function returns the size hint for the widget
		 * @elseif spanish
		 * Reimplementado de QWidget, esta funcion retorna el tamaño apropiado del widget
		 * @endif
		 * @return 
		 */
		QSize sizeHint() const;
		
		
	public slots:
		/**
		 * @if english
		 * Sets the title text
		 * @elseif spanish
		 * Pone el texto del titulo
		 * @endif
		 * @param text 
		 */
		void setText(const QString &text);
		/**
		 * @if english
		 * Sets the title font
		 * @elseif spanish
		 * Pone la letra al titulo
		 * @endif
		 * @param font 
		 */
		void setFont(const QFont &font);
		
	signals:
		/**
		 * @if english
		 * This signal is emitted when title is double clicked
		 * @elseif spanish
		 * Este signal es emitido cuando el titulo ha sido clickeado dos veces
		 * @endif
		 */
		void doubleClicked();
		
	protected:
		void mouseDoubleClickEvent(QMouseEvent * e );
		void paintEvent(QPaintEvent *e);
		
	private:
		QString m_text;
		QFont m_font;
};

#endif
