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

#include <kcore/kglobal.h>
#include "kitemselector.h"

#include <QListWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

struct KItemSelector::Private
{
    QListWidget *available;
    QListWidget *selected;
};

KItemSelector::KItemSelector(QWidget *parent) : QWidget(parent), k( new Private )
{
    QHBoxLayout *layout = new QHBoxLayout;

    k->available = new QListWidget;

    layout->addWidget(k->available);

    QVBoxLayout *controlBox = new QVBoxLayout;
    controlBox->setSpacing(0);
    controlBox->setMargin(2);
    controlBox->addStretch();

    QToolButton *bnext = new QToolButton;
    bnext->setIcon(QPixmap(THEME_DIR + "icons/export_scene.png"));
    //bnext->setText(">>");
    connect(bnext, SIGNAL(clicked()), this, SLOT(addCurrent()));

    controlBox->addWidget(bnext);
    controlBox->setSpacing(5);

    QToolButton *bprev = new QToolButton;
    //bprev->setText("<<");
    bprev->setIcon(QPixmap(THEME_DIR + "icons/unexport_scene.png"));
    connect(bprev, SIGNAL(clicked()), this, SLOT(removeCurrent()));

    controlBox->addWidget(bprev);
    controlBox->addStretch();

    layout->addLayout(controlBox);

    k->selected = new QListWidget;
    layout->addWidget(k->selected);

    setLayout(layout);
}


KItemSelector::~KItemSelector()
{
    delete k;
}

void KItemSelector::selectFirstItem() {
     if (k->available->item(0)) {
         k->available->item(0)->setSelected(true);
         emit changed();
     }
}

void KItemSelector::addCurrent()
{
    int row = k->available->currentRow();

    if (row >= 0) {
        QListWidgetItem *item = k->available->takeItem(row);
        k->selected->addItem(item);

        emit changed();
    }
}

void KItemSelector::removeCurrent()
{
    int row = k->selected->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = k->selected->takeItem(row);
        k->available->addItem(item);

        emit changed();
    }
}

void KItemSelector::upCurrent()
{
    int row = k->selected->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = k->selected->takeItem(row);
        k->selected->insertItem(row-1,item);
        k->selected->setCurrentItem(item);

        emit changed();
    }
}

void KItemSelector::downCurrent()
{
    int row = k->selected->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = k->selected->takeItem(row);
        k->selected->insertItem(row+1,item);
        k->selected->setCurrentItem(item);

        emit changed();
    }
}


void KItemSelector::setItems(const QStringList &items)
{
    k->available->clear();
    addItems(items);
}

int KItemSelector::addItem(const QString &item)
{
    QListWidgetItem *it = new QListWidgetItem(item, k->available);
    int index = k->available->count()-1;
    it->setData(4321, index);

    return index;
}

void KItemSelector::addItems(const QStringList &items)
{
    foreach (QString item, items)
             addItem(item);
}

QStringList KItemSelector::selectedItems() const
{
    QStringList items;
    for (int row = 0; row < k->selected->count(); row++) {
         QListWidgetItem *item = k->selected->item(row);
         items << item->text();
    }

    return items;
}

QList<int> KItemSelector::selectedIndexes() const
{
    QList<int> indexes;
    for (int row = 0; row < k->selected->count(); row++) {
         QListWidgetItem *item = k->selected->item(row);
         indexes << item->data(4321).toInt();
    }

    return indexes;
}

void KItemSelector::clear()
{
    k->available->clear();
    reset();
}

void KItemSelector::reset()
{
    k->selected->clear();
    emit changed();
}

