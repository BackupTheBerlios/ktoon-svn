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

#ifndef KTLIBRARYOBJECT_H
#define KTLIBRARYOBJECT_H

#include <QVariant>
#include <QGraphicsItem>

class KTLibraryObject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTLibraryObject : public QObject
{
	public:
		enum Type
		{
			Item = 0x01
		};
		
		KTLibraryObject(QObject *parent = 0);
		~KTLibraryObject();
		
		void setType(int type);
		int type() const;
		
		void setData(const QVariant &data);
		QVariant data() const;
		
	private:
		int m_type;
		QVariant m_data;
		
};

#endif

