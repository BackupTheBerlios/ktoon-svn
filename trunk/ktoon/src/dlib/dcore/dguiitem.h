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

#ifndef DGUIITEM_H
#define DGUIITEM_H

#include <QIcon>
#include <QString>
#include <QKeySequence>
#include <QCursor>

/**
 * @if english
 * Class that represents an gui item, this class have data such as tooltip, text, whatis, accels, and so...
 * @elseif spanish
 * Clase que representa un item de la interfaz, esta clase tiene datos como tooltips, texto, aceleradores, etc...
 * @endif
 * @author David Cuadrado <krawek@toonka.com>
*/
class DGuiItem
{
	public:
		/**
		 * @if english
		 * Construct an item with text, icon, tooltip and whatis data
		 * @elseif spanish
		 * Construye un item con los datos texto, icono, tooltip, y whatis
		 * @endif
		 * @param text 
		 * @param icon 
		 * @param toolTip 
		 * @param whatIs 
		 * @return 
		 */
		DGuiItem(const QString &text, const QIcon &icon, const QString &toolTip, const QString &whatIs);
		/**
		 * Destructor
		 * @return 
		 */
		~DGuiItem();
		
		/**
		 * @if english
		 * set the item text
		 * @elseif spanish
		 * Pone el texto al item
		 * @endif
		 * @param text 
		 */
		void setText(const QString &text);
		
		/**
		 * @if english
		 * set the item icon
		 * @elseif spanish
		 * Pone el icono al item
		 * @endif
		 * @param icon 
		 */
		void setIcon(const QIcon &icon);
		/**
		 * @if english
		 * set the item tooltip
		 * @elseif spanish
		 * Pone el tooltip del item
		 * @endif
		 */
		void setToolTip(const QString &toolTip);
		/**
		 * @if english
		 * set the whatis icon message
		 * @elseif spanish
		 * Pone el mensaje que-es-esto del item
		 * @endif
		 */
		void setWhatIs( const QString &whatIs);
		/**
		 * @if english
		 * Sets the key sequence for this item
		 * @elseif spanish
		 * Pone la sequencia de teclas para este item
		 * @endif
		 */
		void setKeySequence(const QKeySequence &key);
		/**
		 * @if english
		 * Sets the cursor if is neeeded
		 * @elseif spanish
		 * Pone el cursor si es necesario
		 * @endif
		 */
		void setCursor(const QCursor &cursor);
		
		/**
		 * @if english
		 * Returns the item text
		 * @elseif spanish
		 * Retorna el texto asociado al item
		 * @endif
		 */
		QString text() const;
		/**
		 * @if english
		 * Return the item icon
		 * @elseif spanish
		 * Retorna el icono del item
		 * @endif
		 */
		QIcon icon() const;
		/**
		 * @if english
		 * Returns the tooltip
		 * @elseif spanish
		 * Retorna el tooltip
		 * @endif
		 */
		QString toolTip() const;
		
		/**
		 * @if english
		 * Returns the what-is message
		 * @elseif spanish
		 * Retorna el mensaje que-es-esto
		 * @endif
		 */
		QString whatIs() const;
		/**
		 * @if english
		 * Returns the key sequence
		 * @elseif spanish
		 * Retorna la sequencia de teclas
		 * @endif
		 */
		QKeySequence keySequence() const;
		/**
		 * @if english
		 * Returns the cursor
		 * @elseif spanish
		 * Retorna el cursor asociado
		 * @endif
		 */
		QCursor cursor() const;
		
	private:
		QString m_text;
		QIcon m_icon;
		QString m_tooltip;
		QString m_whatIs;
		
		
		QCursor m_cursor;
		QKeySequence m_keySequence;
		
		

};

#endif
