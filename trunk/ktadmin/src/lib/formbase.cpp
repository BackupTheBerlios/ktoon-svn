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
#include "formbase.h"

#include <QVBoxLayout>
#include <QLabel>

namespace Base {

struct FormBase::Private
{
	QLabel *title;
	QWidget *center;
	QDialogButtonBox *buttons;
	QVBoxLayout *layout;
};

FormBase::FormBase(QDialogButtonBox::StandardButtons buttons, const QString &title, QWidget *parent) : QFrame(parent), d(new Private)
{
	d->layout = new QVBoxLayout(this);
	d->title = new QLabel(title);
	d->layout->addWidget(d->title);
	d->center = new QWidget();
	d->layout->addWidget(d->center);
	d->buttons = new QDialogButtonBox(buttons);
	d->layout->addWidget(d->buttons);
	
	connect(d->buttons, SIGNAL(clicked(QAbstractButton *)), this, SLOT(handleCliked( QAbstractButton* )));
	
	setAttribute(Qt::WA_DeleteOnClose);
}


void FormBase::handleCliked(QAbstractButton * button )
{
	applyAction(d->buttons->buttonRole(button));
}


FormBase::~FormBase()
{
	
}


void FormBase::setCentralWidget(QWidget *widget)
{
	d->center->hide();
	d->layout->removeWidget ( d->center );
	d->layout->insertWidget(1, widget);
	if(d->center)
	{
		delete d->center;
	}
	d->center = widget;	
	
}

void FormBase::setButtons(QDialogButtonBox::StandardButtons buttons)
{
	d->buttons->setStandardButtons (  buttons );
}

void FormBase::setTitle( const QString &title)
{
	d->title->setText(title);
}

}
