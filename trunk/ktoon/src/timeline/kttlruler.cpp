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

#include "kttlruler.h"
#include "ktdebug.h"

KTTLRuler::KTTLRuler(QWidget *parent) : KTRulerBase(KTRulerBase::Horizontal, parent, "KTTLRuler")
{
	KTINIT;
	
	setSeparation(5);
	show();
}


KTTLRuler::~KTTLRuler()
{
	KTEND;
}

void KTTLRuler::movePointers(QPoint pos)
{
	ktDebug() << "movePointers" << endl;
}

// void KTTLRuler::showMenu(KTRulerBase *ruler, QPoint pos)
// {
// 	if(ruler)
// 	{
// 		m_menu->popup(pos);
// 	}
// }
// 
// void KTTLRuler::chooseOption(int opt)
// {
// 	switch(opt)
// 	{
// 		case ChangeScaleToFive:
// 		{
// 			setSeparation(5);
// 		}
// 		break;
// 		case ChangeScaleToTen:
// 		{
// 			setSeparation(10);
// 		}
// 		break;
// 		default:
// 		{
// 		}
// 		break;
// 	}
// }


