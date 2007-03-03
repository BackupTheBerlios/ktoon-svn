/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
 
#include "walldialog.h"

#include <QLineEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "../packages/wall.h"

namespace Comunications {

struct WallDialog::Private
{
	QLineEdit *message;
	QPushButton *send, *close;
};

WallDialog::WallDialog(QWidget * parent): QFrame( parent), d(new Private)
{
	setWindowTitle( "Send wall" );
	
	QVBoxLayout *layout = new QVBoxLayout( this );
	d->message = new QLineEdit();
	
	layout->addWidget(d->message);
	
	QHBoxLayout *buttons = new QHBoxLayout();
	d->send = new QPushButton("send");
	buttons->addWidget(d->send);
	d->close = new QPushButton("close");
	
	buttons->addWidget(d->close);
	connect(d->close, SIGNAL(clicked()), this, SLOT(close()));
	connect(d->send, SIGNAL(clicked()), this, SLOT(send()));
	connect(d->message, SIGNAL(returnPressed()), d->send, SLOT(animateClick ()));
	
	layout->addLayout(buttons);
	hide();
}


WallDialog::~WallDialog()
{
}


void WallDialog::send()
{
	emit requestSendWall( Packages::Wall(d->message->text()).toString());
	d->message->clear();
}

}
