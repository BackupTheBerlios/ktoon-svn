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

#ifndef KTREELISTWIDGET_H
#define KTREELISTWIDGET_H

#include <QTreeWidget>
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KTreeListWidget : public QTreeWidget
{
    Q_OBJECT
    public:
        KTreeListWidget(QWidget *parent = 0);
        ~KTreeListWidget();
        void addItems(const QStringList &items);
        QList<QTreeWidgetItem *> topLevelItems();
        
        void setEditable(bool isEditable);
        bool isEditable() const;
        
    public slots:
        void removeAll();
        
    private slots:
        void editDoubleClickedItem(QTreeWidgetItem *item, int col);
        
    protected slots:
        virtual void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);
        
    signals:
        void itemRenamed(QTreeWidgetItem *item);
        
    private:
        bool m_isEditable;
};

#endif
