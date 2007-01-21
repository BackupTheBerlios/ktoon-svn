/***************************************************************************
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
#ifndef ITEMS_H
#define ITEMS_H

#include <package.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

namespace Packages
{
class Items : public Package
{
	public:
		Items();
		~Items();
		
		void appendItem(const QStringList& );
		
		void addItem(const QString& );
		void addItems(const QStringList& );
		void setItems(const QStringList& );
		
	private:
		QDomElement m_root;
};
}
#endif
