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

#ifndef DIMAGEBUTTON_H
#define DIMAGEBUTTON_H

#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <dcore/dglobal.h>

/**
 * A image pressable
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_GUI_EXPORT DImageButton : public QPushButton
{
	Q_OBJECT

	public:
		DImageButton(const QIcon &icon, int size, QWidget *parent = 0, bool animate = false);
		DImageButton(const QIcon &icon, int size, QObject *reciever, const  char *slot, QWidget *parent = 0, bool animate = false);
		~DImageButton();
		virtual void setImage( const QIcon &icon);
		
		void setAnimated(bool anim);
		
	protected:
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		
		
	private slots:
		void animate();
		
	private:
		void setup();
		
	private:
		int m_imageSize;
		class Animation;
		Animation *m_animator;
		
		bool m_isAnimated;
		
// 	protected:
// 		void resizeEvent(QResizeEvent *e);
// 		void paintEvent(QPaintEvent *e);
// 		QPixmap getPixmap();
};

#endif
