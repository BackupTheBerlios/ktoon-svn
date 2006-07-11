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

#include "ktlayermanager.h"

#include "dapplication.h"
#include <qlabel.h>
#include <qlayout.h>

#include <QPixmap>
#include <QHeaderView>
#include <QPainter>

#include "dseparator.h"

#include "ddebug.h"


KTLayerManager::KTLayerManager(QWidget *parent) : QTableWidget(0, 3, parent), m_allSelected(false), m_allVisible(true), m_allLock(false), m_rowHeight(20)
{
	DINIT;
	
	setHorizontalHeaderLabels(QStringList() << tr("Layer") << tr("L") << tr("V") );
	verticalHeader()->hide();
	
}


KTLayerManager::~KTLayerManager()
{
	DEND;
}


void KTLayerManager::insertLayer(int position, const QString &name)
{
	QTableWidgetItem *newLayer = new QTableWidgetItem(name);
	newLayer->setTextAlignment(Qt::AlignCenter);
	
	if (  position >= 0 && position <= rowCount())
	{
		insertRow (position);
		setItem(position, 0, newLayer);
		
		fixSize();
	}
	else
	{
		delete newLayer;
	}
}

void KTLayerManager::removeLayer(int position)
{
	D_FUNCINFO << position;
	removeRow(position);
}


void KTLayerManager::resizeEvent( QResizeEvent *)
{
	fixSize();
}

void KTLayerManager::fixSize()
{
	int offset = 0;
	if ( verticalScrollBar()->isVisible() )
	{
		offset = verticalScrollBar()->width() +6 ;
	}
	
	int width = this->width() - offset;
		
	horizontalHeader()->resizeSection(0, width-(m_rowHeight*2) );
	horizontalHeader()->resizeSection(1, m_rowHeight );
	horizontalHeader()->resizeSection(2, m_rowHeight );
	
	for(int row = 0; row < rowCount(); row++)
	{
		verticalHeader()->resizeSection(row, m_rowHeight);
	}
}


void KTLayerManager::setRowHeight(int rowHeight)
{
	m_rowHeight = rowHeight;
}
