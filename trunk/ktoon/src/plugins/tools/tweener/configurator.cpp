/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#include <kgui/kradiobuttongroup.h>
#include <kcore/kdebug.h>
 
#include "configurator.h"
#include "ktitemtweener.h"
#include "stepsviewer.h"
#include "kttweenerstep.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QGraphicsPathItem>

struct Configurator::Private
{
    KRadioButtonGroup *options;
    StepsViewer *stepViewer;
};

Configurator::Configurator(QWidget *parent) : QFrame(parent), k(new Private)
{
    setFont(QFont("Arial", 8, QFont::Normal, false));

    k->options = new KRadioButtonGroup(tr("Options"), Qt::Vertical);
    k->options->addItem(tr("Create path"), 0);
    k->options->addItem(tr("Select object"), 1);
    connect(k->options, SIGNAL(clicked(int)), this, SLOT(emitOptionChanged(int)));

    QPushButton *button = new QPushButton(tr("Apply"));
    connect(button, SIGNAL(clicked()), this, SIGNAL(clikedApplyTweener()));

    k->stepViewer = new StepsViewer;
    k->stepViewer->verticalHeader()->hide();
    k->stepViewer->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignHCenter);
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
    //return KTTweenerStep::createXml(k->stepViewer->totalSteps(), k->stepViewer->steps()).toString(); // FIXME: instead createXml, use KTItemTweener::toXml

    return KTItemTweener::tweenToXml(QString("tween00"), k->stepViewer->totalSteps(), k->stepViewer->steps()).toString();
}

int Configurator::totalSteps()
{
    return k->stepViewer->totalSteps();
}

void Configurator::activatePathMode()
{
    k->options->setCurrentIndex(0);
    k->stepViewer->cleanRows();
}

void Configurator::activateSelectionMode()
{
    k->options->setCurrentIndex(1);
}

