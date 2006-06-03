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

#ifndef KTGCTABLE_H
#define KTGCTABLE_H

#include "dtreelistwidget.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
 * @todo Crear un item delegador para editar!
*/
class KTGCTable : public DTreeListWidget
{
	Q_OBJECT
	public:
		KTGCTable(QWidget *parent = 0);
		~KTGCTable();
		QTreeWidgetItem *currentFolder();
		void setCurrentFolder(QTreeWidgetItem *cf);
		void removeCurrentFolder();
		
	public slots:
		void createFolder(const QString &name = QString() );
		
	private:
		QTreeWidgetItem *m_currentFolder;

};

#endif
