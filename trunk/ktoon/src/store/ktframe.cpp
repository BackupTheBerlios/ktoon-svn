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

#include "ktframe.h"
#include "ddebug.h"


KTFrame::KTFrame(QObject *parent) : QGraphicsScene(parent), m_name("Frame"), m_isLocked(false)
{
}

KTFrame::KTFrame(const QString &frameName, QObject * parent) : QGraphicsScene(parent), m_name(frameName), m_isLocked(false)
{	
	
}


KTFrame::~KTFrame()
{
}

void KTFrame::setFrameName(const QString &name)
{
	m_name = name;
}

void KTFrame::setLocked(bool isLocked)
{
	m_isLocked = isLocked;
}

QString KTFrame::frameName() const
{
	return m_name;
}

bool KTFrame::isLocked()
{
	return m_isLocked;
}


