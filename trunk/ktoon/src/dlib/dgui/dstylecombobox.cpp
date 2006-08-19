/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "dstylecombobox.h"

#include <QStyleFactory>
#include <QApplication>

#include <ddebug.h>

DStyleComboBox::DStyleComboBox(QWidget *parent) : QComboBox(parent)
{
	addItems( QStyleFactory::keys() );
	
	connect(this, SIGNAL(activated( const QString& )), this, SLOT(chooseStyle(const QString &)));
	
	setCurrentIndex( findText(QApplication::style()->objectName(), Qt::MatchExactly|Qt::MatchFixedString) );
}


DStyleComboBox::~DStyleComboBox()
{
}


void DStyleComboBox::chooseStyle(const QString &style)
{
	QStyle *st = QStyleFactory::create( style );
	if(st)
	{
		qApp->setStyle( st );
		qApp->setPalette(st->standardPalette());
	}
}

