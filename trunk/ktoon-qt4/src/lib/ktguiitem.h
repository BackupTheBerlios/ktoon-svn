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

#ifndef KTGUIITEM_H
#define KTGUIITEM_H

#include <QIcon>
#include <QString>
#include <QKeySequence>
#include <QCursor>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTGuiItem
{
	public:
		KTGuiItem(const QString &text, const QIcon &icon, const QString &toolTip, const QString &whatIs);
		~KTGuiItem();
		
		void setText(const QString &text);
		void setIcon(const QIcon &icon);
		void setToolTip(const QString &toolTip);
		void setWhatIs( const QString &whatIs);
		void setKeySequence(const QKeySequence &key);
		void setCursor(const QCursor &cursor);
		
		QString text() const;
		QIcon icon() const;
		QString toolTip() const;
		QString whatIs() const;
		QKeySequence keySequence() const;
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
