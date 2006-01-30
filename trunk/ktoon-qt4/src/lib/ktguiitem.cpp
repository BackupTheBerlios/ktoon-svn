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

#include "ktguiitem.h"

KTGuiItem::KTGuiItem(const QString &text, const QIcon &icon, const QString &toolTip, const QString &whatIs) : m_text(text), m_icon(icon), m_tooltip(toolTip), m_whatIs(whatIs)
{
}


KTGuiItem::~KTGuiItem()
{
}

void KTGuiItem::setText(const QString &text)
{
	m_text = text;
}

void KTGuiItem::setIcon(const QIcon &icon)
{
	m_icon = icon;
}

void KTGuiItem::setToolTip(const QString &toolTip)
{
	m_tooltip = toolTip;
}

void KTGuiItem::setWhatIs( const QString &whatIs)
{
	m_whatIs = whatIs;
}

void KTGuiItem::setKeySequence(const QKeySequence &key)
{
	m_keySequence = key;
}

void  KTGuiItem::setCursor(const QCursor &cursor)
{
	m_cursor = cursor;
}


QString  KTGuiItem::text() const
{
	return m_text;
}

QIcon  KTGuiItem::icon() const
{
	return m_icon;
}

QString  KTGuiItem::toolTip() const
{
	return m_tooltip;
}

QString  KTGuiItem::whatIs() const
{
	return m_whatIs;
}

QKeySequence  KTGuiItem::keySequence() const
{
	return m_keySequence;
}

QCursor  KTGuiItem::cursor() const
{
	return m_cursor;
}

