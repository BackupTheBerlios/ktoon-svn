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

#include "ktdialogtitle.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <QLabel>
#include <Q3Frame>

KTDialogTitle::KTDialogTitle(const QString &title, QWidget *parent, const char *name)
 : QLabel(parent, name)
{
	setFrameStyle( Q3Frame::Box | Q3Frame::Raised );
	setText(title);
}


KTDialogTitle::~KTDialogTitle()
{
}

void KTDialogTitle::mouseDoubleClickEvent(QMouseEvent *e)
{
	emit doubleClicked();
}

void KTDialogTitle::setText( const QString &text)
{
	QLabel::setText("<div align=center>"+text+"</div>");
}

