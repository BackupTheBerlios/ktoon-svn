/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                        *
 *   krawek@gmail.com                                        	   *
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

#ifndef KLTABLE_H
#define KLTABLE_H

#include <qtable.h>

/**
* @author CetiSoft
*/

class KLTable : public QTable
{
	Q_OBJECT
	public:
		KLTable(int rows, int cols, QWidget *parent = 0, const char *name = 0);
		~KLTable();

		void resizeEvent ( QResizeEvent * e );
		void setHeaderHText(int col, const QString &text);
		void drawContents ( QPainter * p, int cx, int cy, int cw, int ch );
		void paintEvent ( QPaintEvent * event );
// 		void viewportPaintEvent(QPaintEvent * pe );
				
	public slots:
		void fixSize();
		void expandTableSize();
		void redimensionTable(int rows);
		void insertRowData(const QStringList &data, int row = -1);
		void setViewportText(const QString &text, int size = 70);
		
	private:
		QString m_text;
		int m_textSize;
};

#endif
