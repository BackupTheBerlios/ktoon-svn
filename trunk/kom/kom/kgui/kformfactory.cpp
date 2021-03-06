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

#include "kformfactory.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QLabel>

KFormFactory::KFormFactory()
{
}

KFormFactory::~KFormFactory()
{
}

QBoxLayout *KFormFactory::makeLine(const QString &text, QWidget *widget,  Qt::Orientation o)
{
    QBoxLayout *layout;
    
    if (o == Qt::Vertical)
        layout = new QVBoxLayout;
    else
        layout = new QHBoxLayout;
    
    layout->addWidget(new QLabel(text));
    layout->addWidget(widget);
    
    if (o == Qt::Vertical)
        layout->addStretch(3);
    
    return layout;
}

QGridLayout *KFormFactory::makeGrid(const QStringList &texts, const QWidgetList &widgets, Qt::Alignment alignment)
{
    Q_ASSERT(texts.count() != widgets.count());
    
    QGridLayout *layout = new QGridLayout;
    
    // layout->setColumnStretch(0, 1);
    
    for (int i = 0; i < widgets.count(); i++) {
         layout->addWidget(new QLabel(texts[i]), i, 0, Qt::AlignLeft);
         layout->addWidget(widgets[i], i, 1, alignment);
    }
    
    layout->setColumnStretch(2, 1);
    
    return layout;
}

