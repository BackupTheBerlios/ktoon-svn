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

#ifndef KTLAYERINDEX_H
#define KTLAYERINDEX_H

#include <QPushButton>
#include <QToolTip>
#include <QLabel>
#include <QScrollBar>
#include <QButtonGroup>
#include <QTableWidget>

#include <kgui/kimagebutton.h>
#include "ktprojectrequest.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
**/

class KTLayerIndex : public QTableWidget
{
    Q_OBJECT;
    
    friend class KTLayerIndexItemDelegate;
    
    public:
        enum Actions
        {
            NoAction = 0,
            ShowOutlines,
            LockLayers,
            ToggleLayerView,
            InsertLayer,
            RemoveLayer,
            MoveLayerUp,
            MoveLayerDown
        };
        
        /**
         * Default constructor
         * @param parent 
         * @return 
         */
        KTLayerIndex(int sceneIndex=0, QWidget *parent = 0);
        ~KTLayerIndex();
        
        void insertLayer(int position, const QString &name);
        void insertSoundLayer(int position, const QString &name);
        void removeLayer(int position);
        void renameLayer(int position, const QString &name);
        void moveLayer(int position, int newPosition);
        void lockLayer(int position, bool locked);
        void setRowHeight(int rowHeight);
        void refresh();
        
    protected:
        void resizeEvent(QResizeEvent *e);
        virtual void fixSize();
        virtual void keyPressEvent(QKeyEvent *event);
        
    protected slots:
        void commitData(QWidget * editor);
        void setLocalRequest(int row, int column);

        //void emitSelectionSignal();
        
    signals:
        void requestRenameEvent(int layerPosition, const QString &newName);
        void localRequest();
        void layerVisibility(int sceneIndex, int layerIndex, bool checked);

        // void requestTriggered(const KTProjectRequest *event);
        
    private:
        struct Private;
        Private *const k;
};

#endif
