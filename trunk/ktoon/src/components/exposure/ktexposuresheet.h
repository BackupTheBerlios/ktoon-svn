/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrox@toonka.com>             *
 *                                                                         *
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

#ifndef KTEXPOSURESHEET_H
#define KTEXPOSURESHEET_H

#include "ktmodulewidgetbase.h"
#include "ktexposuretable.h"

// Qt
#include <QButtonGroup>
#include <QGroupBox>
#include <QPushButton>
#include <QListWidget>
#include <QList>
#include <QActionGroup>

#include <kgui/ktabwidget.h>
#include <kgui/kimagebutton.h>

#include "ktprojectactionbar.h"

/**
* @author Jorge Cuadrado
*/

class KTExposureSheet : public KTModuleWidgetBase
{
    Q_OBJECT;
    public:
        KTExposureSheet(QWidget *parent = 0);
        ~KTExposureSheet();

    private:
        struct Private;
        Private * const k;
        void createMenu();

    protected:
        virtual void sceneResponse(KTSceneResponse *e);
        virtual void layerResponse(KTLayerResponse *e);
        virtual void frameResponse(KTFrameResponse *e);

    public slots:
        void closeAllScenes();
        void applyAction(int action);
        void addScene(int index, const QString &name);
        void renameScene(int index, const QString &name);
        void setScene(int index);
        void changeVisiblityLayer( int, bool  );

    private slots: 
        void emitRequestChangeScene(int index);
        void emitRequestCopyCurrentFrame();
        void emitRequestPasteInCurrentFrame();
        void emitRequestExpandCurrentFrame();
		
        void insertItem(int indexLayer, int indexFrame);
        void renameFrame(int indexLayer, int indexFrame, const QString & name);
        void selectFrame(int indexLayer, int indexFrame);
        void renameLayer(int indexLayer, const QString & name);
        void moveLayer(int oldIndex, int newIndex);
        void actionTiggered(QAction *action);
};

#endif
