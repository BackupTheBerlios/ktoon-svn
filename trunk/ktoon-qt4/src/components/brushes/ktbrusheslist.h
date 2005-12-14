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

#ifndef KTBRUSHESLIST_H
#define KTBRUSHESLIST_H

// #include <QTreeWidget>
// #include <QTreeWidgetItem>

#include <QListWidget>
#include <QListWidgetItem>
#include <QPainterPath>

#include "ktcellview.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTBrushesList : public KTCellView
{
	Q_OBJECT
	public:
		KTBrushesList(QWidget *parent = 0);
		~KTBrushesList();
		void addBrush(const QPainterPath &form);
		QPainterPath path(int);
		int count() const;
		
	signals:
		void changeCurrentBrush( QListWidgetItem*);
		
		
	public:
		const int MAX_COLUMNS;
		
	private:
		QList<QPainterPath> m_forms;
		int m_row, m_col;

};

#endif
