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

#include "ack.h"

namespace Packages {

Ack::Ack(const QString &motd, const QString &sign) : QDomDocument()
{
    QDomElement root = createElement("ack");
    root.setAttribute("version", 0);
    QDomElement motde = createElement("motd");
    QDomText motdetext = createTextNode(motd);
    motde.appendChild(motdetext);
    
    QDomElement signe = createElement("sign");
    QDomText singetext = createTextNode(sign);
    signe.appendChild(singetext);
    
    root.appendChild(motde);
    root.appendChild(signe);
    
    m_perms = createElement("permissions");
    root.appendChild(m_perms);
    
    appendChild(root);
}

Ack::~Ack()
{
}

void Ack::addPermission(const Users::Right *right)
{
    QDomElement righte = createElement("perm");
    righte.setAttribute("module", right->module());
    righte.setAttribute("read", right->read());
    righte.setAttribute("write", right->write());
    
    m_perms.appendChild(righte);
}

}

