/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include "zoomconfigurator.h"
#include <QBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>

#include <kgui/kimagebutton.h>
#include <kgui/kseparator.h>

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

ZoomConfigurator::ZoomConfigurator(QWidget *parent) :QWidget(parent)
{
    KINIT;
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
    QLabel *label = new QLabel(tr("Zoom Mode"));
    label->setAlignment(Qt::AlignHCenter); 
    layout->addWidget(label);
    layout->addWidget(new KSeparator(Qt::Horizontal));

    in = new QRadioButton(tr("Zoom In"), this);
    in->setChecked(true); 
    out = new QRadioButton(tr("Zoom Out"), this);

    connect(in, SIGNAL(clicked()), SLOT(setZoomIn()));
    connect(out, SIGNAL(clicked()), SLOT(setZoomOut()));

    layout->addWidget(in);
    layout->addWidget(out);

    scale = new QLabel(tr("Scale Factor"));
    scale->setAlignment(Qt::AlignHCenter);
    layout->addWidget(scale);

    factor = new QDoubleSpinBox();

    //factor->setValue(0.5);
    factor->setDecimals(1);
    factor->setSingleStep(0.1);
    factor->setMinimum(0.1);
    factor->setMaximum(0.9);
    layout->addWidget(factor);

    factor->setEnabled(false);

    mainLayout->addLayout(layout);
    mainLayout->addStretch(2);

    KCONFIG->beginGroup("Zoom tool");
    double value = KCONFIG->value("zoomoutfactor", -1).toDouble();

    if (value > 0) 
        factor->setValue(value);
    else 
        factor->setValue(0.5);
}

ZoomConfigurator::~ZoomConfigurator()
{
    KEND;
}

void ZoomConfigurator::resizeEvent(QResizeEvent *)
{
    resize(minimumSizeHint());
}

void ZoomConfigurator::setZoomIn()
{
    if (out->isChecked())
        out->setChecked(false);

    if (factor->isEnabled())
        factor->setEnabled(false);
}

void ZoomConfigurator::setZoomOut()
{
    if (in->isChecked())
        in->setChecked(false);

    if (!factor->isEnabled()) 
        factor->setEnabled(true);
}

bool ZoomConfigurator::zoomIn()
{
    return in->isChecked();
}

double ZoomConfigurator::getFactor() const
{
    return factor->value();
}

