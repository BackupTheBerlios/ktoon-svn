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

class QAbstractButton;

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
			Add = 0x01,
			Del = 0x02,
			Query = 0x04,
			Modify = 0x8,
			Custom1 = 0x10,
			Custom2 = 0x20
		};
		
		Q_DECLARE_FLAGS(Buttons, Button)
		
		ModuleButtonBar(Buttons buttons, QWidget *parent = 0);
		~ModuleButtonBar();
		
		bool hasButton(Button button) const;
		
		void setText(Button button, const QString &text);
		void setStatusTip(Button button, const QString &text);
		void setIcon(Button button, const QIcon &icon);
		
	signals:
		void buttonClicked ( QAbstractButton *button);
		void buttonClicked(int id);
		
		
	private:
		void setupButtons(Buttons buttons);
		
	private:
		struct Private;
		Private *const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModuleButtonBar::Buttons)

}
#endif
