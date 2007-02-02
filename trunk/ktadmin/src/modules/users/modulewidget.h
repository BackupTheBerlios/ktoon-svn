/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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
 
#ifndef USERSMODULEWIDGET_H
#define USERSMODULEWIDGET_H

#include <modulelistwidget.h>
#include <package.h>
#include "form.h"

namespace Users {

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class ModuleWidget : public Base::ModuleListWidget
{
	Q_OBJECT;
	public:
		ModuleWidget(QWidget *parent = 0);
		
		~ModuleWidget();
		virtual void handlePackage(Base::Package *const pkg);
		
	private slots:
		void removeUser(const QString& login);
		
	protected slots:
		virtual void addActionSelected(QTreeWidgetItem *current);
		virtual void delActionSelected(QTreeWidgetItem *current);
		virtual void modifyActionSelected(QTreeWidgetItem *current);
		
	protected:
		virtual void updateList();
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
