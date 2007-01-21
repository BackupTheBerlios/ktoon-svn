/****************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#include "error.h"
namespace Packages
{
	
Error::Error(const QString &message, int level)
	: Package()
{
	QDomElement root = createElement ( "error" );
	root.setAttribute ( "version",  "0" );
	appendChild(root);
	
	m_message = createElement("message");
	root.appendChild(m_message);
	m_message.appendChild( createTextNode(message) );
	m_message.setAttribute("level", level );
}


Error::~Error()
{
}


void Error::setMessage(const QString &message)
{
	m_text.setData(message);
}

void Error::setLevel(int level)
{
	m_message.setAttribute("level", level );
}

}
