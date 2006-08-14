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

#ifndef CCBUTTON_H
#define CCBUTTON_H
/**
 * @file dcirclebutton.h
 * Include this file if you need the class DCircleButton
 */
#include <QPushButton>
#include <QStyleOptionButton>
#include "dcore/dglobal.h"

/**
 * @short The DCircleButton class provides a circular button
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_GUI_EXPORT DCircleButton : public QPushButton
{
	Q_OBJECT
	public:
		/**
		 * Constructs a DCircleButton
		 */
		DCircleButton(int diameter, bool animate = true, QWidget *parent = 0);
		/**
		 * Destructor
		 */
		~DCircleButton();
		
		QStyleOptionButton styleOption() const;
		QSize sizeHint() const;
		
	protected:
		/**
		 * Paints the button
		 * @param e 
		 */
		void paintEvent(QPaintEvent *e);
		
		/**
		 * Init animation
		 */
		void enterEvent(QEvent *);
		/**
		 * End animation
		 */
		void leaveEvent(QEvent *);
		
		/**
		 * Creates mask of button
		 */
		virtual void paintMask();
		
	private slots:
		void animate();
		
	private:
		QPixmap m_mask;
		QPixmap m_pix;
		int m_diameter : 22;
		
		class Animator;
		Animator *m_animator;

};

#endif
