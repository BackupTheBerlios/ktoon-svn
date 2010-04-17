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

#ifndef KTLAYERTABLE_H
#define KTLAYERTABLE_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHash>

class KTFramesTable;
class KTFramesTableItemDelegate;

class KTFramesTableItem : public QTableWidgetItem
{
    public:
        enum Attributes
        {
            IsUsed = 0x01,
            IsLocked,
            IsSound
        };
        
        KTFramesTableItem();
        virtual ~KTFramesTableItem();
        
        bool isUsed();
        bool isLocked();
        bool isSound();
};

class KTTLRuler;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTFramesTable : public QTableWidget
{
    Q_OBJECT;
    
    friend class KTFramesTableItemDelegate;
    
    public:
        KTFramesTable(QWidget *parent = 0);
        ~KTFramesTable();
        
        bool isSoundLayer(int row);
        
    public slots:
        // Layers
        void insertLayer(int layerPos, const QString &name);
        void insertSoundLayer(int layerPos, const QString &name);
        
        void removeCurrentLayer();
        void removeLayer(int pos);
        void moveLayer(int pos, int newPos);
        
        int lastFrameByLayer(int layerPos);
        
        // Frames
        void insertFrame(int layerPos, const QString &name);
        
        void setCurrentFrame(KTFramesTableItem *);
        void setCurrentLayer(int layerPos);
        void selectFrame(int index);
        
        void setAttribute(int row, int col, KTFramesTableItem::Attributes att, bool value);
        
        void removeFrame(int layerPos, int position);
        
        void lockFrame(int layerPosition, int position, bool lock);
        
        void setItemSize(int w, int h);
        
    private:
        void setup();
        
    protected:
        void fixSize();
        
    private slots:
        void emitFrameSelected(int col);
        void emitFrameSelected(QTableWidgetItem *curr, QTableWidgetItem *prev);
        
    signals:
        void frameRequest(int action, int frame, int layer, int scene, const QVariant &argument = QVariant());
        
    private:
        struct Private;
        Private *const k;
};

#endif
