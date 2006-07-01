/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "dguiitem.h"

DGuiItem::DGuiItem(const QString &text, const QIcon &icon, const QString &toolTip, const QString &whatIs) : m_text(text), m_icon(icon), m_tooltip(toolTip), m_whatIs(whatIs)
{
}


DGuiItem::~DGuiItem()
{
}

void DGuiItem::setText(const QString &text)
{
	m_text = text;
}

void DGuiItem::setIcon(const QIcon &icon)
{
	m_icon = icon;
}

void DGuiItem::setToolTip(const QString &toolTip)
{
	m_tooltip = toolTip;
}

void DGuiItem::setWhatIs( const QString &whatIs)
{
	m_whatIs = whatIs;
}

void DGuiItem::setKeySequence(const QKeySequence &key)
{
	m_keySequence = key;
}

void  DGuiItem::setCursor(const QCursor &cursor)
{
	m_cursor = cursor;
}


QString  DGuiItem::text() const
{
	return m_text;
}

QIcon  DGuiItem::icon() const
{
	return m_icon;
}

QString  DGuiItem::toolTip() const
{
	return m_tooltip;
}

QString  DGuiItem::whatIs() const
{
	return m_whatIs;
}

QKeySequence  DGuiItem::keySequence() const
{
	return m_keySequence;
}

QCursor  DGuiItem::cursor() const
{
	return m_cursor;
}

