/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktlistpackage.h"

/*
<list version="0" >
        <options pattern="string or regexp" type="0">
                <caseSensitive enabled="0" />
                <regexp enabled="0" />
        </options>
<list/>
*/

KTListPackage::KTListPackage(const QString & pattern, int type, bool regexp, bool caseSensitive)
 : QDomDocument()
{
	QDomElement root = createElement("list");
	root.setAttribute("version", "0");
	appendChild(root);
	
	m_options = createElement("options");
	m_options.setAttribute("pattern", pattern);
	m_options.setAttribute("type", type);
	root.appendChild(m_options);
	
	m_caseSensitive = createElement("caseSensitive");
	m_caseSensitive.setAttribute("enabled", caseSensitive);
	m_options.appendChild(m_caseSensitive);
	
	m_regexp = createElement("regexp");
	m_caseSensitive.setAttribute("enabled", regexp);
	m_options.appendChild(m_regexp);
}

KTListPackage::~KTListPackage()
{
}


void KTListPackage::setPattern(const QString & pattern)
{
	m_options.setAttribute("pattern", pattern);
	
}

void KTListPackage::setType(int type)
{
	m_options.setAttribute("type", type);
	
}

void KTListPackage::setCaseSensitive(bool caseSensitive)
{
	m_caseSensitive.setAttribute("enabled", caseSensitive);
}

void KTListPackage::setRegexp(bool regexp)
{
	m_caseSensitive.setAttribute("enabled", regexp);
}

