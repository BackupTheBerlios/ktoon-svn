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

/**
 * @file dcirclebuttonbar.h
 * Include this file if you need the class DCircleButtonBar
 */

#include <qframe.h>
#include <QPixmap>
#include <QPolygon>
#include <QBoxLayout>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QImage>

#include "dgui/dcirclebutton.h"
#include "dcore/dglobal.h"

class QPainterPath;

/**
 * @short The DCircleButtonBar class provides a circular button bar
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_GUI_EXPORT DCircleButtonBar : public QFrame
{
	public:
		/**
		 * Constructs a DCircleButtonBar
		 */
		DCircleButtonBar(int radio = 40, QWidget *parent= 0);
		/**
		 * Destructor
		 */
		~DCircleButtonBar();
		/**
		 * Add button in DCircleButtonBar
		 * @param pix image of new DCircleButton
		 * @return pointer of new DCircleButton
		 */
		DCircleButton *addButton(const QPixmap &pix);
		
	private:
		QPixmap m_mask;
		
		int m_radio;
		int m_buttonCount;
		
		QBoxLayout *m_layout;
		
		int m_offset;
	
	protected:
		void paintEvent(QPaintEvent *e);
		void resizeEvent(QResizeEvent *e);
		
		QPainterPath m_border;

};

#endif
