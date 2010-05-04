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

#ifndef KTTIMELINE_H
#define KTTIMELINE_H

#include <ktmodulewidgetbase.h>
#include <QSplitter>

#include <QStackedWidget>
#include <kgui/ktabwidget.h>

class KTLayerManager;
class KTFramesTable;
class KTProjectActionBar;
class KTFramesTableItem;
class KTLibrary;

/**
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class KTTimeLine : public KTModuleWidgetBase
{
    Q_OBJECT
    public:
        KTTimeLine(QWidget *parent = 0);
        ~KTTimeLine();
        void closeAllScenes();
        
        void setLibrary(const KTLibrary *library);
        
    private:
        KTLayerManager *layerManager(int sceneIndex);
        KTFramesTable *framesTable(int sceneIndex);
        
    protected:
        void sceneResponse(KTSceneResponse *e);
        void layerResponse(KTLayerResponse *e);
        void frameResponse(KTFrameResponse *e);
        void libraryResponse(KTLibraryResponse *response);
        
    public slots:
        void insertScene(int position, const QString &name);
        void removeScene(int position);

    private slots:
        void requestCommand(int action);
        bool requestFrameAction(int action, int framePos = -1, int layerPos = -1, int scenePos = -1, const QVariant &arg = QVariant());
        bool requestLayerAction(int action, int layerPos = -1, int scenePos = -1, const QVariant &arg = QVariant());
        bool requestSceneAction(int action, int scenePos = -1, const QVariant &arg = QVariant());
        void selectFrame(int indexLayer, int indexFrame);
        
        void emitRequestRenameLayer(int layer, const QString &name);
        void emitSelectionSignal();
        void emitLayerVisibility(int sceneIndex, int layerIndex, bool checked);
        
    private:
        struct Private;
        Private *const k;
};

#endif
