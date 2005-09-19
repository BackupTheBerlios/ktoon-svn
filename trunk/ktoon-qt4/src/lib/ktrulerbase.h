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

#ifndef RULER_H
#define RULER_H

#include <q3frame.h>
#include <qpixmap.h>
#include <q3pointarray.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>

#define UNITCOUNT 5

class KTRulerBase;

/**
 * @author Jorge Cuadrado
*/

class KTRulerBase : public Q3Frame
{
	Q_OBJECT
	
	public:
		enum Orientation
		{
			Horizontal,
			Vertical
		};


// 		enum Unit {
// 			SC_POINTS      = 0,
// 			SC_PT          = 0,
// 			SC_MILLIMETERS = 1,
// 			SC_MM          = 1,
// 			SC_INCHES      = 2,
// 			SC_IN          = 2,
// 			SC_PICAS       = 3,
// 			SC_P           = 3,
// 			SC_CENTIMETERS = 4,
// 			SC_CM          = 4,
// 			SC_CICERO      = 5,
// 			SC_C           = 5
// 		};
		
		KTRulerBase(Orientation orientation=Horizontal, QWidget *parent = 0, const char *name = 0);
		~KTRulerBase();
		
// 		const double unitGetRatioFromIndex(const int index);
// 		const double pts2mm(double pts);
// 		const double mm2pts(double mm);
		virtual void drawScale();
		int orientation();
		
		
	private:
		int m_position;
		int m_separation;
		Orientation m_orientation;
		bool m_drawPointer;
		Q3PopupMenu *m_menu;
		enum { ChangeScaleToFive, ChangeScaleToTen  };
		
	protected:
		Q3PointArray m_pArrow;
		QPixmap m_pScale;
		
	signals:
		void displayMenu(KTRulerBase *, QPoint pos);
		
	protected:
		virtual void paintEvent ( QPaintEvent * e);
		virtual void resizeEvent ( QResizeEvent * );
		virtual void mouseMoveEvent ( QMouseEvent * e );
		virtual void mousePressEvent (QMouseEvent *e);
		
	public slots:
 		virtual void movePointers(QPoint pos) = 0;
		void setSeparation(int sep);
		void setDrawPointer(bool yes = true);
		
		virtual void showMenu(KTRulerBase *, QPoint pos);
		virtual void chooseOption(int);
};

#endif
