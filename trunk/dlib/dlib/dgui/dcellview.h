/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DCELLVIEW_H
#define DCELLVIEW_H
/**
 * @file dcellview.h
 * @brief Include this file if you need the class DCellView, DCellViewItem,  DCellViewItemDelegate or DCellViewModel
 */

#include <QTableWidget>
#include <QStyleOptionViewItem>
#include "dcore/dglobal.h"

class DCellViewItemDelegate;


class DCellViewItem : public QTableWidgetItem
{
	public:
		DCellViewItem();
		~DCellViewItem();
		
		QImage image() const;
		QBrush background() const;
		
};

/**
 * @author David Cuadrado <krawek@gmail.com>
 */
class DCellView : public QTableWidget
{
	Q_OBJECT;
	public:
		DCellView( int MAX_COLUMNS = 16, QWidget *parent = 0);
		DCellView(int rows, int columns, int MAX_COLUMNS = 16, QWidget *parent = 0);
		~DCellView();
		
		void setItemSize(int w, int h);
		
		void addItem(DCellViewItem *item);
		void addItem(const QBrush &brush);
		void addItem(const QImage &image);
		
	private:
		void setup();
		
	private slots:
		void fixSize();
		
	protected:
		virtual QStyleOptionViewItem viewOptions() const;
		virtual void wheelEvent(QWheelEvent *e);
		
	private:
		int m_rectWidth, m_rectHeight;
		
		
	private:
		int m_countColor;
		int m_col, m_row;
		const int MAX_COLUMNS;
		
};

#endif
