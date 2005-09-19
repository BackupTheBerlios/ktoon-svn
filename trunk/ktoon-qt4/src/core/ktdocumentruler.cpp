/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#include "ktdocumentruler.h"

KTDocumentRuler::KTDocumentRuler(Orientation orientation, QWidget *parent, const char *name) : KTRulerBase(orientation, parent, name)
{
	setDrawPointer(true);
}


KTDocumentRuler::~KTDocumentRuler()
{
}

void KTDocumentRuler::movePointers(QPoint pos)
{
	if(orientation() == Horizontal)
	{
		if(pos.x() > 0)
		{
			m_pArrow[0].setX ( pos.x()-5 );
			m_pArrow[1].setX ( pos.x() );
			m_pArrow[2].setX ( pos.x()+5 ); 
		}
	}
	else if(orientation() == Vertical)
	{
		if(pos.x() > 0)
		{
			m_pArrow[0].setY ( pos.y()-5 );
			m_pArrow[1].setY ( pos.y() );
			m_pArrow[2].setY ( pos.y()+5);
		}
	}
	repaint();
}
