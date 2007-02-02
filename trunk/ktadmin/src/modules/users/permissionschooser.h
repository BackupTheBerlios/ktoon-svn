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
 
#ifndef USERSPERMISSIONSCHOOSER_H
#define USERSPERMISSIONSCHOOSER_H

#include <QGroupBox>
#include <QStringList>

#include <permission.h>

namespace Users {

/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class PermissionsChooser : public QGroupBox
{
	Q_OBJECT;
	public:
		
		PermissionsChooser(const QStringList & modules, QWidget *parent = 0);
		~PermissionsChooser();
		
		QList<Users::Permission> permissions();
		void setPermissions(const QList<Users::Permission> &);
		
	private slots:
		void changePremission(int id);
		
	private:
		struct Private;
		Private * const d;
};

}

#endif
