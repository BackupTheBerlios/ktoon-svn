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

#include "kxyspinbox.h"

#include <QHBoxLayout>
#include <QSizePolicy>

#include "kapplication.h"

#include "kdebug.h"

KXYSpinBox::KXYSpinBox(const QString &title, QWidget *parent) : QGroupBox(title, parent), m_modifyTogether(false)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QGridLayout *internal = new QGridLayout;
    m_textX = new QLabel("X: ");
    internal->addWidget(m_textX, 0, 0, Qt::AlignRight);

    m_x = new QSpinBox;
    m_x->setMinimumWidth(60);
    internal->addWidget(m_x, 0, 1);
    m_textX->setBuddy(m_x);

    m_textY = new QLabel("Y: ");
    internal->addWidget(m_textY, 1, 0, Qt::AlignRight);

    m_y = new QSpinBox;
    m_y->setMinimumWidth(60);
    internal->addWidget(m_y, 1, 1);

    m_textY->setBuddy(m_y);
    layout->addLayout(internal);

    m_separator = new QPushButton;
    m_separator->setFlat(true);
    m_separator->setMaximumWidth(20);
    m_separator->setIcon(QPixmap(THEME_DIR + "/icons/vchain_broken.png"));

    //layout->addWidget(m_separator);

    //connect(m_separator, SIGNAL(clicked()), this, SLOT(toggleModify()));
    setLayout(layout);

    connect(m_x, SIGNAL(editingFinished()), this, SLOT(updateXValue()));
    connect(m_y, SIGNAL(editingFinished()), this, SLOT(updateYValue()));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

KXYSpinBox::~KXYSpinBox()
{
}

void KXYSpinBox::updateXValue()
{
     int value = (int) m_x->value()*380/520;
     m_y->setValue(value);
}

void KXYSpinBox::updateYValue()
{
     int value = (int) m_y->value()*520/380; 
     m_x->setValue(value);
}

void KXYSpinBox::setModifyTogether(bool enable)
{
    m_modifyTogether = enable;
    toggleModify();
}

void KXYSpinBox::toggleModify()
{
    if (!m_modifyTogether) {
        m_modifyTogether = true;
        m_separator->setIcon(QPixmap(THEME_DIR + "/icons/vchain.png"));
        int x = m_x->value();
        if (m_y->value() != x)
            m_y->setValue(x);
    } else {
        m_modifyTogether = false;
        m_separator->setIcon(QPixmap(THEME_DIR + "/icons/vchain_broken.png"));
    }
}

void KXYSpinBox::setSingleStep(int step)
{
    m_x->setSingleStep(step);
    m_y->setSingleStep(step);
}

void KXYSpinBox::setMinimum(int min)
{
    m_x->setMinimum(min);
    m_y->setMinimum(min);
}

void KXYSpinBox::setMaximum(int max)
{
    m_x->setMaximum(max);
    m_y->setMaximum(max);
}

void KXYSpinBox::setX(int x)
{
    m_x->setValue(x);
}

void KXYSpinBox::setY(int y)
{
    m_y->setValue(y);
}

int KXYSpinBox::x()
{
    return m_x->value();
}

int KXYSpinBox::y()
{
    return m_y->value();
}
