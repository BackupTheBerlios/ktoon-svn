/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef CCBAR_H
#define CCBAR_H

#include <qframe.h>
#include <QPixmap>
#include <QPolygon>
#include <QBoxLayout>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QImage>

#include "ccbutton.h"

class QPainterPath;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class CCBar : public QFrame
{
	public:
		CCBar(int radio = 40, QWidget *parent= 0);
		~CCBar();
		CCButton *addButton(const QPixmap &pix);
		
	private:
		QPixmap m_mask;
		
		int m_radio;
		int m_buttonCount;
		
		QBoxLayout *m_layout;
		
		int m_offset;
	protected:
		void paintEvent(QPaintEvent *e);

};

#endif
