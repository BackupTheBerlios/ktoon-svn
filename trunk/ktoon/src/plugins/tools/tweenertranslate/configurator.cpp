/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
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
 
#include "configurator.h"
#include <dgui/dradiobuttongroup.h>
#include <QVBoxLayout>
#include <QPushButton>

struct Configurator::Private
{
	DRadioButtonGroup *options;
};

Configurator::Configurator(QWidget *parent)
	: QFrame(parent), d(new Private)
{
	
	d->options = new DRadioButtonGroup(tr("options"), Qt::Vertical);
	d->options->addItems(QStringList() << tr("Create path"));
	d->options->addItems(QStringList() << tr("Select object"));
	
	connect(d->options, SIGNAL(clicked(int)), this, SLOT(emitOptionChanged(int)));
	
	QPushButton *button = new QPushButton(tr("apply"));
	connect(button, SIGNAL(clicked()), this, SIGNAL(clikedApplyTweener()));
	
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(d->options);
	layout->addWidget(button);
	setLayout(layout);
}


Configurator::~Configurator()
{
}

void Configurator::emitOptionChanged(int option)
{
	switch(option)
	{
		case 0:
		{
			emit clikedCreatePath();
		}
		break;
		case 1:
		{
			emit clikedSelect();
		}
	}
}

