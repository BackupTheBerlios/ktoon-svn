/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#ifndef KTVIEWAREA_H
#define KTVIEWAREA_H

#include "ktdocumentruler.h"
#include <QMainWindow>

#include <kgui/kactionmanager.h>
#include <kgui/kosd.h>

#include "ktfilterinterface.h"
#include "kttoolinterface.h"
#include "ktconfigurationarea.h"
#include "ktglobal.h"

class KTProjectRequest;
class KTProject;
class KTBrushManager;
class KTPaintAreaCommand;
class KTPaintAreaEvent;
class KTProjectResponse;

/**
 * @author Jorge Cuadrado
*/

class KTViewDocument : public QMainWindow
{
    Q_OBJECT;

    public:
        KTViewDocument(KTProject *project, QWidget *parent = 0 );
        ~KTViewDocument();
        void closeArea();
        QSize sizeHint() const;

        void setAntialiasing(bool useIt );
        void setOpenGL(bool useIt);
        void setDrawGrid(bool draw);

        QPainter::RenderHints renderHints() const;
        void setRotationAngle(int angle);

        KTBrushManager *brushManager() const;
        KTPaintAreaCommand *createCommand(const KTPaintAreaEvent *event);

    public slots:
        void disablePreviousOnionSkin();
        void onePreviousOnionSkin();
        void twoPreviousOnionSkin();
        void threePreviousOnionSkin();
        void disableNextOnionSkin();
        void oneNextOnionSkin();
        void twoNextOnionSkin();
        void threeNextOnionSkin();

        void setNextOnionSkin(int n);
        void setPreviousOnionSkin(int n);
        void toggleShowGrid();

        void setZoomFactor(int porcent);
        void scaleRuler(double factor);
        void changeRulerOrigin(const QPointF &zero);

    private:
        struct Private;
        Private *const k;

    private:
        void setupDrawActions();
        void createToolBar();
        void createMenu();
        void createTools();

    private slots: 
        // Plugins
        void loadPlugins();

    private slots:
        void showPos(const QPointF &p);	
        void setCursor(const QCursor &c);

        void updateZoomFactor( double f);
        // void configure();
        void selectToolFromMenu(QAction *act);

    public slots:
        void undo();
        void redo();
        void selectTool();
        void applyFilter();

        bool handleProjectResponse(KTProjectResponse *event);

    signals:
        void sendToStatus(const QString& msg);
        void requestTriggered(const KTProjectRequest *event);
        void localRequestTriggered(const KTProjectRequest *event);

    protected:
        // void closeEvent(QCloseEvent *e);
};

#endif
