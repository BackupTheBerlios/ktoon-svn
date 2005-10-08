/*
 *   Copyright (C) 1997  Michael Roth <mroth@wirlweb.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <qstyle.h>

#include <QFrame>
#include <QStyleOption>
#include <QPainter>

#include "kseparator.h"
#include "ktdebug.h"
#include "ktapplication.h"


KSeparator::KSeparator(QWidget* parent)
   : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation( HLine );
}



KSeparator::KSeparator(int orientation, QWidget* parent)
   : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation( orientation );
}



void KSeparator::setOrientation(int orientation)
{
	switch(orientation)
	{
		case Qt::Vertical:
		case VLine:
			setFrameShape ( QFrame::VLine);
			setFrameShadow( QFrame::Sunken );
			setMinimumSize(2, 0);
			break;

		default:
			ktWarning() << "KSeparator::setOrientation(): invalid orientation, using default orientation HLine" << endl;

		case Qt::Horizontal:
		case HLine:
			setFrameStyle ( QFrame::HLine  );
			setFrameShadow( QFrame::Sunken );
			setMinimumSize(0, 2);
			break;
	}

}



int KSeparator::orientation() const
{
   if ( frameStyle() & VLine )
      return VLine;
   
   if ( frameStyle() & HLine )
      return HLine;
   
   return 0;
}

void KSeparator::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	QStyleOption opt;
	opt.init(this);

	QPoint p1, p2;
	QRect r = frameRect();
	if ( frameStyle() & HLine )
	{
		p1 = QPoint( r.x(), r.height()/2 );
		p2 = QPoint( r.x()+r.width(), p1.y() );
	}
	else 
	{
		p1 = QPoint( r.x()+r.width()/2, 0 );
		p2 = QPoint( p1.x(), r.height() );
	}

	opt.rect = QRect(p1, p2);
	opt.palette.setBrush(QPalette::Background, palette().dark() );

	style()->drawPrimitive( QStyle::PE_Q3Separator, &opt, &p);
}


QSize KSeparator::sizeHint() const
{
   if ( frameStyle() & VLine )
      return QSize(2, 0);
   
   if ( frameStyle() & HLine )
      return QSize(0, 2);
   
   return QSize(-1, -1);
}

void KSeparator::virtual_hook( int, void* )
{ /*BASE::virtual_hook( id, data );*/ }

