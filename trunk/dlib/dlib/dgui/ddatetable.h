/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef DDATETABLE_H
#define DDATETABLE_H

#include <QTableWidget>
#include <QDate>
#include "dcore/dglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DDateTable : public QTableWidget
{
	Q_OBJECT;
	public:
		DDateTable(QWidget *parent = 0);
		~DDateTable();
		
		void setDate(const QDate &date);
		
		void setMonth(int month);
		
		int cellWidth() const;
		int cellHeight() const;
		
		QDate date() const;
		
	protected:
		void paintEvent(QPaintEvent *e);
		
	private:
		void setCellSize(int width, int height);
		QDate dateFromPosition(int position);
		
	private slots:
		QDate dateFromItem(QTableWidgetItem *item);
		
	signals:
		void dateChanged(const QDate &date);
		
	private:
		int m_cellWidth, m_cellHeight;
		QDate m_date;
};

#endif
