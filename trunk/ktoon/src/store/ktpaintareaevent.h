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

#ifndef KTPAINTAREAEVENT_H
#define KTPAINTAREAEVENT_H

#include <ktprojectevent.h>
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTPaintAreaEvent
{
	public:
		enum Action
		{
			None = 0,
			ChangePen,
			ChangePenBrush,
			ChangeBrush
		};
		
		KTPaintAreaEvent(Action action, const QVariant &data);
		virtual ~KTPaintAreaEvent();
		
		Action action() const;
		QVariant data() const;
		
		virtual KTPaintAreaEvent *clone() const;
		
	private:
		Action m_action;
		QVariant m_data;
};

#endif