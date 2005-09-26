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

#ifndef KTIMAGEBUTTON_H
#define KTIMAGEBUTTON_H

#include <qpushbutton.h>
#include <qimage.h>
//Added by qt3to4:
#include <QPixmap>

/**
 * A image pressable
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTImageButton : public QPushButton
{
	Q_OBJECT

	public:
		KTImageButton(const QPixmap &image, int size, QWidget *parent = 0);
		~KTImageButton();
		virtual void setPixmap ( const QPixmap & pix);
		
	protected:
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		
	private:
		void setup();
		
	private:
		int m_imageSize;
		
// 	protected:
// 		void resizeEvent(QResizeEvent *e);
// 		void paintEvent(QPaintEvent *e);
// 		QPixmap getPixmap();
};

#endif
