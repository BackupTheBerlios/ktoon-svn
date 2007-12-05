/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include "ktgctable.h"

#include <QHeaderView>

#include <kcore/kglobal.h>

KTGCTable::KTGCTable(QWidget *parent)
	: KTreeListWidget(parent), m_currentFolder(0)
{
	setHeaderLabels(QStringList() << "" << "" );
	header()->setResizeMode(QHeaderView::ResizeToContents);
}


KTGCTable::~KTGCTable()
{
}

void KTGCTable::createFolder(const QString &name)
{
	QTreeWidgetItem *newFolder = new QTreeWidgetItem(this);
	
	if ( name.isNull() )
	{
		newFolder->setText(0, tr("New folder %1").arg( topLevelItemCount ()) );
	}
	else
	{
		newFolder->setText(0, name );
	}
	newFolder->setIcon(0, QPixmap(THEME_DIR+"/icons/folder_icon.png" ));
	
	m_currentFolder = newFolder;
	
	setCurrentItem(m_currentFolder);
}

QTreeWidgetItem *KTGCTable::currentFolder()
{
	return m_currentFolder;
}

void KTGCTable::setCurrentFolder(QTreeWidgetItem *cf)
{
	if ( cf )
	{
		m_currentFolder = cf;
	}
}

void KTGCTable::removeCurrentFolder()
{
	if ( m_currentFolder )
	{
		int index = indexOfTopLevelItem(m_currentFolder) - 1;
		
		delete m_currentFolder;
		
		m_currentFolder = topLevelItem (index);
		setCurrentItem(m_currentFolder);
	}
}

