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

#ifndef KTMAINWINDOW_H
#define KTMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QUndoStack>
#include <kgui/kactionmanager.h>
#include "ktviewdocument.h"

#include "ktanimationspace.h"
#include "ktpreferences.h"

// modules
#include "ktexposuresheet.h"
#include "kinaswidget.h"
#include "ktpenwidget.h"
#include "ktcamerawidget.h"
#include "ktcolorpalette.h"
#include "ktsceneswidget.h"
#include "ktlibrarywidget.h"
#include "kttimeline.h"
#include "ktviewcamera.h"
#include "kthelpwidget.h"
#include "kthelpbrowser.h"
#include "ktexportwidget.h"

#include "ktviewdocument.h"

#include <kgui/ktabbedmainwindow.h>
#include "ktstatusbar.h"

#include <kgui/kosd.h>
#include <kgui/ktoolview.h>

// Including headers about projects management
#include "ktprojectmanager.h"
#include "ktsplash.h"

#include "ktviewdocument.h"

class KTProjectManagerParams;
class KTNetProjectManagerParams;
class KTProjectResponse;

/**
 * This class defines the main window application
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTMainWindow : public KTabbedMainWindow
{
    Q_OBJECT;
    public:

        enum Perspective {
             Drawing = 0x01,
             Animation = 0x02,
             Help = 0x04,
             All = Drawing | Animation | Help
        };

        KTMainWindow(KTSplash *splash = 0, int parameters = 1);
        ~KTMainWindow();

    private:
        /**
         * Creates the file action 
         */
         void setupFileActions();
         void setupSettingsActions();
         void setupWindowActions();
         void setupInsertActions();

        /**
         * Sets up the actions in the toolbar
         */
         void setupToolBar();

        /**
         * Sets up he actions in the menu
         */
         void setupMenu();

         void setupHelpActions();
         void setupActions();
         void setMenuItemsContext(bool flag);

         void ui4project(QWidget *widget);
         void ui4paintArea(QWidget *widget);
         void ui4localRequest(QWidget *widget);
         void flipTabs(int index);

         bool setupNetworkProject(const QString& projectName = QString(), const QString &server = QString(), int port = -1);
         bool setupNetworkProject(KTProjectManagerParams *params);
         bool setupLocalProject(KTProjectManagerParams *params);
         void setUndoRedoActions();

    protected:
         /**
          *  Event for main window closing control
          *
          * Reimplemented from QWidget.
          * @param close_event The input event
          */
          void closeEvent(QCloseEvent *event);

         /**
          *  Creates the application GUI according to the information from the data classes
          */
          virtual void createGUI();

         /**
          *  Updates the open recent menu item names according to the @a recent_names list of file names
          */
          void updateOpenRecentMenu(QMenu *menu, QStringList recents);

    public slots:
          void openProject(const QString &path);

    private slots:
          void viewNewDocument();
          void createNewProject();
          void newProject();
          bool closeProject();
          void openProject();
          void openProjectFromServer();
          void importProjectToServer();
          void exportProject();

          void save();
          void saveAs();

          void showHelpPage(const QString &document);
          void showWidgetPage();

          void showAnimationMenu(const QPoint &p);

          void changePerspective(QAction *a);
          void setHelpPerspective();

          void addPage(QWidget *widget);
          void updateCurrentTab(int index);

    private slots:
          void messageToStatus(const QString &);
          void preferences();
          void aboutKToon();
          void showTipDialog();
          void importPalettes();
          void connectToDisplays(const QWidget *widget);
          void saveProject();
          void openRecentProject();
          void createCommand(const KTPaintAreaEvent *event);
          void callSave();

    private:
          KTProjectManager *m_projectManager;
          KToon::RenderType m_renderType;
          QString m_fileName;

    private:
          KTViewDocument *m_viewDoc;
          KTAnimationspace *m_animationSpace;
          KTStatusBar *m_statusBar;
          KActionManager *m_actionManager;
          QMenu *m_fileMenu;
          QMenu *m_settingsMenu;
          QMenu *m_viewMenu;
          QMenu *m_insertMenu;
          QMenu *m_toolsMenu; 
          QMenu *m_windowMenu;
          QMenu *m_helpMenu;

          QStringList m_recentProjects;

          QMenu *m_recentProjectsMenu;

    private: // Network variables
          bool m_isNetworkProject;
          KToolView *m_viewChat;

    // Components
    private:
          KTExposureSheet *m_exposureSheet;
          KTScenesWidget *m_scenes;
          KTTimeLine *m_timeLine;
          KTHelpWidget *m_helper;
          KTHelpBrowser *page;
          KTLibraryWidget *m_libraryWidget;
          KTColorPalette *m_colorPalette;
          KTPenWidget *m_penWidget;
          KToolView *exposureView;
          KToolView *colorView;
          KToolView *penView;
          KToolView *libraryView;
          KToolView *scenesView;  
          KToolView *helpView;
          KToolView *timeView;
          KToolView *exportView;
          KTViewCamera *viewCamera;
          bool isSaveDialogOpen; 
          int lastTab;

    signals:
          void responsed(KTProjectResponse *);
          void updateAnimationModule(KTProject *, int, int, int);
};

#endif
