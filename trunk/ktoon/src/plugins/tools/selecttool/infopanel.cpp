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

#include "infopanel.h"
#include <QBoxLayout>

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

InfoPanel::InfoPanel(QWidget *parent) :QWidget(parent)
{
    KINIT;
    QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QLabel *label = new QLabel(tr("Tips"));
    label->setAlignment(Qt::AlignHCenter); 
    layout->addWidget(label);

    mainLayout->addLayout(layout);
    
    QLabel *other = new QLabel(tr("Other"));
    other->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(other);
   
    mainLayout->addStretch(2);
}

InfoPanel::~InfoPanel()
{
    KEND;
}

void InfoPanel::resizeEvent(QResizeEvent *)
{
    //resize(minimumSizeHint());
}

