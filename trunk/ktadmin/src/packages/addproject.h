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
 
#ifndef PACKAGESADDPROJECT_H
#define PACKAGESADDPROJECT_H

#include <QDomDocument>

namespace Packages {

/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class AddProject : public QDomDocument
{
	public:
		AddProject(const QString & name, const QString & author, const QString& description );
		~AddProject();
		void addUser( const QString& login, int type );
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
