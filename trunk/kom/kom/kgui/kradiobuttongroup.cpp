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


#include "kradiobuttongroup.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "kdebug.h"

KRadioButtonGroup::KRadioButtonGroup(const QString &title, Qt::Orientation orientation, QWidget *parent) : QGroupBox(title, parent), m_orientation(orientation)
{
    if (orientation == Qt::Horizontal)
        m_layout = new QHBoxLayout;
    else
        m_layout = new QVBoxLayout;
    
    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(emitButtonId(QAbstractButton *)));
    
    setLayout(m_layout);
}

KRadioButtonGroup::~KRadioButtonGroup()
{
}

void KRadioButtonGroup::addItem(const QString & text, int index)
{
    QRadioButton *button = new QRadioButton(text);
    m_buttonGroup->addButton(button, index);
    m_layout->addWidget(button);
}

void KRadioButtonGroup::addItems(const QStringList &texts)
{
    QStringList::ConstIterator it = texts.begin();
    
    while (it != texts.end()) {
           addItem(*it);
           ++it;
    }
    
    if (m_buttonGroup->buttons().count()) {
        QAbstractButton *button = m_buttonGroup->buttons()[0];

        if (button) 
            button->setChecked(true);
    }
}

int KRadioButtonGroup::currentIndex() const
{
    return m_buttonGroup->buttons().indexOf(m_buttonGroup->checkedButton());
}

void KRadioButtonGroup::emitButtonId(QAbstractButton *button)
{
    emit clicked(m_buttonGroup->buttons().indexOf(button));
}

void KRadioButtonGroup::setCurrentIndex(int index)
{
    QAbstractButton *button = m_buttonGroup->button(index);

    if (button) {
        button->setChecked(true);
        emit clicked(index);
    } 
}
