/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado       krawek@gmail.com           *
 *                                                                         *
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

#ifndef BASEMODULEBUTTONBAR_H
#define BASEMODULEBUTTONBAR_H

#include <QFrame>


#include <QButtonGroup>

namespace Base {
/**
 * Abstraccion de una barra de botones.
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class ModuleButtonBar : public QFrame
{
	Q_OBJECT;
	public:
		enum Button
		{
			Add = 1<<0,
			Del = 1<<1,
			Query = 1 << 2,
			Modify = 1 << 3
		};
		
		ModuleButtonBar(int buttons, QWidget *parent = 0);
		~ModuleButtonBar();
		
	signals:
		void buttonClicked ( QAbstractButton *button);
		void buttonClicked(int id);
		
		
	private:
		void setupButtons(int buttons);
		
	private:
		QButtonGroup m_buttons;
};
}
#endif
