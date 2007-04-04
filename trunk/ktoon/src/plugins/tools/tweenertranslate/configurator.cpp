//
// C++ Implementation: configurator
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

