/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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


#include "ktcellscolor.h"
#include "ktdebug.h"
#include "ktpalettedocument.h"

#include <QFile>


KTCellsColor::KTCellsColor(QWidget *parent, Type type)
	: KTCellView(parent), m_type(type), m_countColor(0), m_readOnly(false),  m_col(0), m_row(0), MAX_COLUMNS(16)
{}


KTCellsColor::~KTCellsColor()
{
	
}

void KTCellsColor::addColor(const QBrush& b)
{
	KTCellViewItem *item = new KTCellViewItem;
	
	if( columnCount() < MAX_COLUMNS)
	{
		insertColumn( columnCount()+1);
	}
	
	if( m_countColor % MAX_COLUMNS == 0)
	{
		insertRow( (rowCount()+1));
		m_row++;
		m_col = 0;
	}
	else
	{
		m_col++;
	}
	item->setBackground(b);
	m_countColor++;
	setItem(m_row-1 , m_col , item);
}

void KTCellsColor::setReadOnly(bool enable)
{
	m_readOnly = enable;
}

bool KTCellsColor::isReadOnly()
{
	return m_readOnly;
}

void KTCellsColor::setType(Type type)
{
	m_type = type;
}

int KTCellsColor::type()
{
	return m_type;
}

QString KTCellsColor::name() const
{
	return m_name;
}

void KTCellsColor::setName(const QString& name)
{
	m_name = name;
}

void KTCellsColor::save( const QString &path)
{
	QFile save(path);
	KTPaletteDocument document(m_name, true);
	
	for(int i = 0; i < columnCount() ; i++)
	{
		for (int  j = 0; j < rowCount() ; j++)
		{
			KTCellViewItem *tmpItem = itemAt(i*25, j*25);
			if(tmpItem)
			{
				if(tmpItem->background().gradient())
				{
					document.addGradient(*tmpItem->background().gradient());
				}
				else if(tmpItem->background().color().isValid())
				{
					document.addColor(tmpItem->background().color());
				}
			}
		}
	}
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << document.toString();
		save.close();
	}
}

