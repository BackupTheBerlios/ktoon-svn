/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "keditspinbox.h"
#include <QHBoxLayout>

KEditSpinBox::KEditSpinBox(int value, int minValue, int maxValue, int step, QString text, QWidget *parent, const char *name) :QGroupBox(parent)
{
    setObjectName(name);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(2);
    layout->setSpacing(0);
    setTitle(text);
    setLayout(layout);
    m_spin = new QSpinBox(this);
    m_spin->setMinimum(minValue);
    m_spin->setMaximum(maxValue);
    m_spin->setSingleStep ( step );
    m_spin->setValue(value);
    layout->addWidget(m_spin);
    m_slider = new QSlider (Qt::Horizontal, this);
    m_slider->setMinimum(minValue);
    m_slider->setMaximum(maxValue);
    m_slider->setSingleStep(step);
    
    layout->addWidget(m_slider);
    setupConnects();
    setMinimumHeight(sizeHint().height());
}

KEditSpinBox::~KEditSpinBox()
{
}

void KEditSpinBox::setupConnects()
{
    QObject::connect(m_spin, SIGNAL(valueChanged(int)),
             m_slider, SLOT(setValue(int)));
    QObject::connect(m_slider, SIGNAL(valueChanged(int)),
             m_spin, SLOT(setValue(int)));
    QObject::connect(m_slider,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
    QObject::connect(m_spin,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
}

void KEditSpinBox::setRange(int min, int max)
{
    m_spin->setMinimum(min);
    m_spin->setMaximum(max);
    m_slider->setMinimum(min);
    m_slider->setMaximum(max);
}

void KEditSpinBox::setValue(int value)
{
    m_spin->setValue(value);
    m_slider->setValue(value);
}

int KEditSpinBox::value()
{
    return m_slider->value();
}
