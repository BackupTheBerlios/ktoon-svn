/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2007 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include <kgui/kradiobuttongroup.h>

#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QGraphicsPathItem>
#include "stepsviewer.h"
#include "kttweenerstep.h"

struct Configurator::Private
{
    KRadioButtonGroup *options;
    StepsViewer *stepViewer;
};

Configurator::Configurator(QWidget *parent) : QFrame(parent), k(new Private)
{
    k->options = new KRadioButtonGroup(tr("Options"), Qt::Vertical);
    k->options->addItems(QStringList() << tr("Create path"));
    k->options->addItems(QStringList() << tr("Select object"));
    connect(k->options, SIGNAL(clicked(int)), this, SLOT(emitOptionChanged(int)));

    QPushButton *button = new QPushButton(tr("apply"));
    connect(button, SIGNAL(clicked()), this, SIGNAL(clikedApplyTweener()));

    k->stepViewer = new StepsViewer;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(k->options);
    layout->addWidget(k->stepViewer);
    layout->addWidget(button);
    setLayout(layout);
}

Configurator::~Configurator()
{
    delete k;
}

void Configurator::updateSteps(const QGraphicsPathItem *path)
{
    k->stepViewer->setPath(path);
}

void Configurator::emitOptionChanged(int option)
{
    switch (option) {
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

QString Configurator::steps()
{
    return KTTweenerStep::createXml(k->stepViewer->totalSteps(), k->stepViewer->steps()).toString(); // FIXME: no usar createXml, usar KTItemTweener::toXml
}

int Configurator::totalSteps()
{
    return k->stepViewer->totalSteps();
}
