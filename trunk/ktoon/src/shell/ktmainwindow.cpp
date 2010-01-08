/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktmainwindow.h"

#include "ktnewproject.h"
#include "ktabout.h"
#include "kthelpbrowser.h"

#include "ktpackagehandler.h"

#include "ktpaletteimporter.h"
#include "ktpaintareacommand.h"

// Including some KOM headers
#include <kgui/ktipdialog.h>
#include <kcore/kdebug.h>
#include <kgui/kosd.h>
#include <kgui/kimageeffect.h>
#include <ksound/kaudioplayer.h>

#include "ktapplication.h"
#include "ktpluginmanager.h"

#include "ktprojectcommand.h"
#include "ktlocalprojectmanagerhandler.h"

// #ifdef USE_NET
#include "ktnetprojectmanagerhandler.h"
#include "ktnetprojectmanagerparams.h"
#include "ktconnectdialog.h"
#include "ktsavenetproject.h"
#include "ktlistpackage.h"
#include "ktimportprojectpackage.h"
#include "ktlistprojectspackage.h"
// #end

// Qt
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
#include <QMenu>
#include <QCloseEvent>
#include <QTextEdit>
#include <QFileDialog>
#include <QDomDocument>
#include <QMessageBox>
#include <QDesktopServices>
//

/**
 * This class defines the main window application.
 * Here is where all the KToon GUI is initialized 
 * @author David Cuadrado <krawek@toonka.com>
*/

/**
 * @if english
 * This is the constructor method for this class.
 * @endif
 * @if spanish
 * Este es el metodo constructor para esta clase.
 * @endif
 * @return 
*/

KTMainWindow::KTMainWindow(KTSplash *splash) : 
              KTabbedMainWindow(), m_projectManager(0), m_viewDoc(0), m_animationSpace(0), 
              m_viewChat(0), m_exposureSheet(0), m_scenes(0)
{
    #ifdef K_DEBUG
       KINIT;
    #endif

    // Loading audio player plugin
    KAudioPlayer::instance()->loadEngine("gstreamer"); // FIXME: Move this to the settings 
    setObjectName("KTMainWindow_");

    // Defining the status bar
    m_statusBar = new KTStatusBar(this);
    setStatusBar(m_statusBar);

    // Naming the main frame...
    setWindowTitle(tr("KToon: 2D Animation Toolkit"));

    // Defining the render type for the drawings
    m_renderType = KToon::RenderType(KCONFIG->value("RenderType").toInt());

    // Calling out the project manager
    m_projectManager = new KTProjectManager(this);
    m_projectManager->setHandler(new KTLocalProjectManagerHandler);

    splash->setMessage(tr("Setting up the project manager"));

    // Calling out the events/actions manager
    splash->setMessage(tr("Loading action manager..."));
    m_actionManager = new KActionManager(this);
	
    // Defining the menu bar
    splash->setMessage(tr("Creating menu bar..."));
    setupActions();
	
    splash->setMessage(tr("Creating GUI..."));
	
    // Setting up all the GUI...
    createGUI(); // This method is called from the ktmainwindow_gui class
    setupMenu();
    setupToolBar();
	
    // Check if user wants to see a KToon tip for every time he launches the program
    KCONFIG->beginGroup("TipOfDay");
    bool showTips = qvariant_cast<bool>(KCONFIG->value("ShowOnStart", true));
	
    // If option is enabled, then, show a little dialog with a nice tip
    if (showTips)
        QTimer::singleShot(0, this, SLOT(showTipDialog()));
	
    // Time to load plugins... 
    KTPluginManager::instance()->loadPlugins();

    // Defining the Drawing view, as the first interface to show up	
    setCurrentPerspective(Drawing);

    KCONFIG->beginGroup("General");
    // check if into the config file, user always wants to start opening his last project created
    bool openLast = KCONFIG->value("OpenLastProject", true).toBool();

    if (openLast)
        openProject(KCONFIG->value("LastProject").toString());
}

/**
 * @if english
 * This is the destructor method for this class.
 * @endif
 * @if spanish
 * Este es el metodo destructor para esta clase.
 * @endif
*/
KTMainWindow::~KTMainWindow()
{
    #ifdef K_DEBUG
       KEND;
    #endif
    delete KTPluginManager::instance();
    delete KOsd::self();
}

/**
 * @if english
 * This method cleans and set the whole interface to start a new project.
 * @endif
 * @if spanish
 * Este metodo limpia y configura toda la interfaz para iniciar un nuevo proyecto.
 * @endif
*/

void KTMainWindow::createNewProject()
{
    // Modal
    //if (!closeProject())
    //    return;

    kFatal() << "*** Setting up a new project";

    m_projectManager->setupNewProject();
    enableToolViews(true);
    setMenuItemsContext(true);

    if (!m_isNetworkProject)
        viewNewDocument(tr("Document"));
}

/**
 * @if english
 * This method supports all the low level tasks for the method createNewProject().
 * @endif
 * @if spanish
 * Este metodo soporta todas las tareas de bajo nivel para el metodo createNewProject().
 * @endif
*/

void KTMainWindow::viewNewDocument(const QString &title)
{
    #ifdef K_DEBUG
       K_FUNCINFO;
       kDebug() << m_projectManager->isOpen();
    #endif

    if (m_projectManager->isOpen()) {
        // Setting undo/redo actions
        setUndoRedoActions();

        messageToStatus(tr("Opening a new paint area..."));
        KOsd::self()->display(tr("Opening a new document..."));

        m_viewDoc = new KTViewDocument(m_projectManager->project());
        connectToDisplays(m_viewDoc);

        m_viewDoc->setWindowTitle(tr("Illustration: %1").arg(title));

        addWidget(m_viewDoc, true, All);

        connectToDisplays(m_viewDoc);
        ui4project(m_viewDoc);
        ui4localRequest(m_viewDoc);
      
        m_viewDoc->setAntialiasing(true);

        // KTViewCamera *
        viewCamera = new KTViewCamera(m_projectManager->project());
        ui4project(viewCamera);

        connect(this, SIGNAL(tabHasChanged(int)), this, SLOT(updateAnimation(int)));

        m_animationSpace = new KTAnimationspace(viewCamera);
        m_animationSpace->setWindowIcon(QIcon(THEME_DIR + "icons/animation_mode.png"));
        m_animationSpace->setWindowTitle(tr("Animation"));

        addWidget(m_animationSpace, true, All);

        exposureView->expandDock(true);
    }
}

/**
 * @if english
 * This method is the first level instruction called when a new project is requested.
 * @endif
 * @if spanish
 * Este metodo es la instruccion de primer nivel llamada cuando un nuevo proyecto es solicitado.
 * @endif
*/

void KTMainWindow::newProject()
{
    #ifdef K_DEBUG
       kWarning() << "Creating new project...";
    #endif

    KTNewProject *wizard = new KTNewProject(this);
    // connectToDisplays(wizard);
    if (wizard->exec() != QDialog::Rejected) {
       if (wizard->useNetwork())
           setupNetworkProject(wizard->parameters());
       else
           setupLocalProject(wizard->parameters());
       createNewProject();
    }

    delete wizard;
}

/**
 * @if english
 * This method handles all the tasks required to close a project session.
 * @endif
 * @if spanish
 * Este metodo se encarga de todas las tareas requeridas para cerrar un proyecto en sesion.
 * @endif
 * @return true if the project is closed successfully
*/

bool KTMainWindow::closeProject()
{
    if (!m_projectManager->isOpen())
        return true;
	
    if (m_projectManager->isModified()) {
        int ret = QMessageBox::warning(this, QApplication::applicationName (),
                                       tr("The document has been modified.\n"
                                       "Do you want to save your project?"),
                                       QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
        switch (ret) {
            case QMessageBox::Save:
                 saveProject();
                 break;
            case QMessageBox::Discard:
                 break;
            case QMessageBox::Cancel:
                 return false;
                 break;
        }

    }

    if (exposureView->isExpanded())
        exposureView->expandDock(false);

    if (colorView->isExpanded())
        colorView->expandDock(false);

    if (penView->isExpanded())
        penView->expandDock(false);

    if (libraryView->isExpanded())
        libraryView->expandDock(false);

    if (scenesView->isExpanded())
        scenesView->expandDock(false);
    
    if (helpView->isExpanded())
        helpView->expandDock(false);
    
    if (timeView->isExpanded())
        timeView->expandDock(false);

    /*
    if (exportView->isExpanded())
        exportView->expandDock(false);
    */

    setUpdatesEnabled(false);
    enableToolViews(false);
    setMenuItemsContext(false);

    if (m_viewDoc)
        m_viewDoc->closeArea();

    removeWidget(m_animationSpace, true);
    removeWidget(m_viewDoc, true);
    delete m_viewDoc;
    m_viewDoc = 0;
	
    m_projectManager->closeProject();

    // Cleaning widgets
    m_exposureSheet->closeAllScenes();
    m_timeLine->closeAllScenes();
    m_scenes->closeAllScenes();

    m_fileName = QString();
    setUpdatesEnabled(true);

    return true;
}

/**
 * @if english
 * This method sets up a KToon network project.
 * @endif
 * @if spanish
 * Este metodo configura un proyecto para trabajo en red de KToon.
 * @endif
 * @return true if the network project can be configured
*/

bool KTMainWindow::setupNetworkProject(const QString& projectName ,const QString &server , int port)
{
    KTConnectDialog *cndialog = new KTConnectDialog(this);

    if (!server.isEmpty())
        cndialog->setServer(server);

    if (port != -1)
        cndialog->setPort(port);

    KTNetProjectManagerParams *params = new KTNetProjectManagerParams();

    if (cndialog->exec() == QDialog::Accepted) {
        params->setServer(cndialog->server());
        params->setPort(cndialog->port());
        params->setLogin(cndialog->login());
        params->setPassword(cndialog->password());
        params->setProjectName(projectName);

        return setupNetworkProject(params);
    }

    return false;
}

/**
 * @if english
 * This method sets up a KToon network project.
 * @endif
 * @if spanish
 * Este metodo configura un proyecto para trabajo en red de KToon.
 * @endif
 * @return 
*/

bool KTMainWindow::setupNetworkProject(KTProjectManagerParams *params)
{
    if (closeProject()) {
        KTNetProjectManagerHandler *netProjectManagerHandler =  new KTNetProjectManagerHandler;
        connect(netProjectManagerHandler, SIGNAL(openNewArea(const QString&)), this, SLOT(viewNewDocument(const QString&)));
        m_projectManager->setHandler(netProjectManagerHandler);
        m_projectManager->setParams(params);
        m_isNetworkProject = true;

        if (m_viewChat) {
            removeToolView(m_viewChat);
            delete m_viewChat;
        }

        m_viewChat = addToolView(netProjectManagerHandler->comunicationWidget(), Qt::RightDockWidgetArea, All);
        m_viewChat->setVisible(false);

        return true;
    }

    return false;
}

/**
 * @if english
 * This method sets up a KToon local/single project.
 * @endif
 * @if spanish
 * Este metodo configura un proyecto local/individual de KToon.
 * @endif
 * @return true if the local project can be configured
*/

bool KTMainWindow::setupLocalProject(KTProjectManagerParams *params)
{
    if (closeProject()) {
        m_projectManager->setHandler(new KTLocalProjectManagerHandler);
        m_projectManager->setParams(params);
        m_isNetworkProject = false;

        return true;
    }

    return false;
}

/**
 * @if english
 * This method opens a KToon project. 
 * @endif
 * @if spanish
 * Este metodo abre un proyecto de KToon.
 * @endif
*/

void KTMainWindow::openProject()
{
    const char *home = getenv("HOME");

    QString package = QFileDialog::getOpenFileName(this, tr("Import project package"), home, 
                      tr("KToon Project Package (*.ktn);;KToon Net Project (*.ktnet)"));

    if (package.isEmpty()) 
        return;

    openProject(package);
}

/**
 * @if english
 * This method does all the tasks required to open a project.
 * @endif
 * @if spanish
 * Este metodo realiza todas las tareas requeridas para abrir un proyecto.
 * @endif
*/

void KTMainWindow::openProject(const QString &path)
{
    #ifdef K_DEBUG
       kWarning() << "Opening project: " << path;
    #endif

    if (path.isEmpty())
        return;

    if (path.endsWith(".ktnet")) {
        KTSaveNetProject loader;
        KTNetProjectManagerParams *params = loader.params(path);

        setupNetworkProject(params->projectName(), params->server(), params->port());
        delete params;
    } else if (path.endsWith(".ktn")) {
               m_projectManager->setHandler(new KTLocalProjectManagerHandler);
               m_isNetworkProject = false;
    }

    if (closeProject()) {
        setUpdatesEnabled(false);
        tabWidget()->setCurrentWidget(m_viewDoc);

        if (m_projectManager->loadProject(path)) {
            if (QDir::isRelativePath(path))
                m_fileName = QDir::currentPath() + "/" + path;
            else
                m_fileName = path;

            viewNewDocument(m_projectManager->project()->projectName());
			
            // TODO: move this code to the project manager class
            KTFrameResponse response(KTProjectRequest::Frame, KTProjectRequest::Select);
            response.setSceneIndex(0);
            response.setLayerIndex(0);
            response.setFrameIndex(0);

            m_viewDoc->handleProjectResponse(&response);
            m_exposureSheet->handleProjectResponse(&response);
            m_timeLine->handleProjectResponse(&response);

            int pos = m_recentProjects.indexOf(m_fileName);

            if (pos == -1) {
                if (m_recentProjects.count() <= 6)
                    m_recentProjects << m_fileName;
                else
                    m_recentProjects.push_front(m_fileName);
            } else {
                    m_recentProjects.push_front(m_recentProjects.takeAt(pos));
            }

            updateOpenRecentMenu(m_recentProjectsMenu, m_recentProjects);

            enableToolViews(true);
            setMenuItemsContext(true);

            setUpdatesEnabled(true);

            // Showing a info message in a bubble
            KOsd::self()->display(tr("Project %1 opened!").arg(m_projectManager->project()->projectName()));
        } else {
                 setUpdatesEnabled(true);
                 KOsd::self()->display( tr("Cannot open project!"), KOsd::Error );
        }
    }
}

/**
 * @if english
 * This method opens an animation project from a KToon Server.
 * @endif
 * @if spanish
 * Este metodo abre un proyecto de animacion desde un servidor de KToon.
 * @endif
*/

void KTMainWindow::openProjectFromServer()
{
    if (setupNetworkProject()) {
        KTNetProjectManagerHandler *handler = static_cast<KTNetProjectManagerHandler *>
                                              (m_projectManager->handler());
        if (handler->isValid()) {
            KTListProjectsPackage package;
            handler->sendPackage(package);
        }
    }
}

/**
 * @if english
 * This method sends a local KToon project into the animations server.
 * @endif
 * @if spanish
 * Este metodo envia un proyecto local de KToon en el servidor de animaciones.
 * @endif
*/

void KTMainWindow::importProjectToServer()
{
     if (setupNetworkProject()) {
         KTNetProjectManagerHandler *handler = static_cast<KTNetProjectManagerHandler *>
                                               (m_projectManager->handler());

         if (handler->isValid()) {
             const char *home = getenv("HOME");
             QString file = QFileDialog::getOpenFileName(this, tr("Import project package"), 
                                                     home, tr("KToon Project Package (*.ktn)"));
             KTImportProjectPackage package(file);		
             handler->sendPackage(package);
         }
     }
}

/**
 * @if english
 * This method calls the methods required to save the current project.
 * @endif
 * @if spanish
 * Este metodo llama a los metodos requeridos para salvar el proyecto actual.
 * @endif
*/

void KTMainWindow::save()
{
    #ifdef K_DEBUG
       kDebug("project") << "Saving..";
    #endif
    QTimer::singleShot(0, this, SLOT(saveProject()));
}

/**
 * @if english
 * This method opens the KToon preferences dialog.
 * @endif
 * @if spanish
 * Este metodo abre el dialogo de preferencias de KToon.
 * @endif
*/

void KTMainWindow::preferences()
{
    m_statusBar->setStatus( tr( "Preferences Dialog Opened" ), 2000 );
    KTPreferences *preferences = new KTPreferences( this );
    preferences->exec();

    delete preferences;
}

/**
 * @if english
 * This method opens the "About KToon" dialog.
 * @endif
 * @if spanish
 * Este metodo abre el dialogo "Acerca de KToon".
 * @endif
*/

void KTMainWindow::aboutKToon()
{
    KTAbout *about = new KTAbout(this);
    about->exec();

    delete about;
}

/**
 * @if english
 * This method opens the tips dialog.
 * @endif
 * @if spanish
 * Este metodo abre el dialogo de consejos utiles.
 * @endif
*/
void KTMainWindow::showTipDialog()
{
    KTipDialog *tipDialog = new KTipDialog(DATA_DIR + "tips", this);
    tipDialog->show();
    // tipDialog.exec();
}

/**
 * @if english
 * This method imports Gimp color palettes for KToon.
 * @endif
 * @if spanish
 * Este metodo importa paletas de colores de Gimp para KToon.
 * @endif
*/

void KTMainWindow::importPalettes()
{
    const char *home = getenv("HOME");
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Import gimp palettes"), home, 
                                                       tr("Gimp Palette (*.gpl)"));

    m_statusBar->setStatus(tr("Importing palettes"));
    QStringList::ConstIterator it = files.begin();
	
    int progress = 1;
    while (it != files.end()) {
           KTPaletteImporter importer;
           importer.import(*it, KTPaletteImporter::Gimp);
           ++it;

           importer.saveFile(CONFIG_DIR + "/palettes");
           m_colorPalette->parsePaletteFile( importer.filePath());
           m_statusBar->advance(progress++, files.count());
    }
}

/**
 * @if english
 * This method defines the events handlers for the project opened.
 * @endif
 * @if spanish
 * Este metodo define los manejadores de eventos para el proyecto abierto.
 * @endif
*/

void KTMainWindow::ui4project(QWidget *widget)
{
    connect(widget, SIGNAL(requestTriggered(const KTProjectRequest *)), m_projectManager, 
            SLOT(handleProjectRequest(const KTProjectRequest *)));

    connect(m_projectManager, SIGNAL(responsed(KTProjectResponse*)), widget, 
            SLOT(handleProjectResponse(KTProjectResponse *)));

    // PENDING TO CHECK
    //connect(widget, SIGNAL(postPage(QWidget *)), this, SLOT(addPage(QWidget *)));
}

/**
 * @if english
 * This method defines the events handlers for the paint area.
 * @endif
 * @if spanish
 * Este metodo define los manejadores de eventos para el area de dibujo.
 * @endif
*/

void KTMainWindow::ui4paintArea(QWidget *widget)
{
    connect(widget, SIGNAL(paintAreaEventTriggered(const KTPaintAreaEvent *)), this, 
            SLOT(createCommand(const KTPaintAreaEvent *)));
}

/**
 * @if english
 * This method defines the events handlers for the local requests.
 * @endif
 * @if spanish
 * Este metodo define los manejadores de eventos para las solicitudes locales.
 * @endif
*/

void KTMainWindow::ui4localRequest(QWidget *widget)
{
    connect(widget, SIGNAL(localRequestTriggered(const KTProjectRequest *)), m_projectManager, 
            SLOT(handleLocalRequest(const KTProjectRequest *)));
}

/**
 * @if english
 * This method sets a message into the status bar.
 * @endif
 * @if spanish
 * Este metodo asigna un mensaje a la barra de estados.
 * @endif
*/

void KTMainWindow::messageToStatus(const QString &msg)
{
    m_statusBar->setStatus(msg, msg.length() * 90);
}

/**
 * @if english
 * This method display a help page.
 * @endif
 * @if spanish
 * Este metodo despliega una pagina de ayuda.
 * @endif
*/

void KTMainWindow::showHelpPage(const QString &title, const QString &filePath)
{
    #ifdef K_DEBUG
       K_FUNCINFO;
    #endif
    KTHelpBrowser *page = new KTHelpBrowser(this);
    page->setDataDirs(QStringList() << m_helper->helpPath());

    // page->setDocument(document);
    page->setSource(filePath);
    page->setWindowTitle(tr("Help:%1").arg(title));
    addWidget(page, false, All);
}

/**
 * @if english 
 * This method is in charge of the function "Save as" for KToon projects.
 * @endif
 * @if spanish
 * Este metodo se encarga de la funcion "Salvar como" para proyectos de KToon.
 * @endif
*/

void KTMainWindow::saveAs()
{
    const char *home = getenv("HOME");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Build project package"), home, 
                       "KToon Project Package (*.ktn);;KToon Net Project (*.ktnet)");

    if (fileName.isEmpty())
        return;

    int indexPath = fileName.lastIndexOf("/");
    int indexFile = fileName.length() - indexPath;
    QString name = fileName.right(indexFile - 1);
    QString path = fileName.left(indexPath + 1);

    QDir directory(path);
    if (!directory.exists()) {
        //QMessageBox::critical(this, tr("Error!"), tr("Directory \"" + path.toLocal8Bit() + "\" does not exist! Please, choose another path."), QMessageBox::Ok);
        KOsd::self()->display(tr("Directory \"" + path.toLocal8Bit() + "\" does not exist! Please, choose another path."), KOsd::Error);
        return;
    } else {
        QFile file(directory.filePath(name));
        if (!file.open(QIODevice::ReadWrite)) {
            file.remove();
            //QMessageBox::critical(this, tr("Error!"), tr("You have no permission to create this file. Please, choose another path."), QMessageBox::Ok);
            KOsd::self()->display(tr("You have no permission to create this file. Please, choose another path."), KOsd::Error);
            return;
        }
        file.remove();
    }

    m_fileName = fileName;
    save();
}

/**
 * @if english 
 * This method does all the tasks required to save a KToon Project.
 * @endif
 * @if spanish
 * Este metodo se encarga de todas las tareas necesarias para guardar un proyecto.
 * @endif
*/

void KTMainWindow::saveProject()
{
    if (m_fileName.isEmpty()) {
        saveAs();
        return;
    }

    if (m_projectManager->saveProject(m_fileName)) 
        KOsd::self()->display(tr("Project %1 saved").arg(m_projectManager->project()->projectName()), KOsd::Info);
    else
        KOsd::self()->display(tr("Cannot save the project!"), KOsd::Error );
}

/**
 * @if english 
 * This method opens a recent project.
 * @endif
 * @if spanish
 * Este metodo abre un proyecto reciente.
 * @endif
*/

void KTMainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openProject(action->text());
}

/**
 * @if english 
 * This method shows the Animation module menu.
 * @endif
 * @if spanish
 * Este metodo muestra el menu del modulo de Animacion.
 * @endif
*/

void KTMainWindow::showAnimationMenu(const QPoint &p)
{
    QMenu *menu = new QMenu(tr("Animation"), m_animationSpace);
    menu->addAction(tr("New camera"), this, SLOT(newViewCamera()));
    menu->exec(p);
    delete menu;
}

/**
 * @if english 
 * This method redefines the closeEvent method for the main window.
 * @endif
 * @if spanish
 * Este metodo redefine el metodo closeEvent para el marco principal de la interfaz.
 * @endif
*/

void KTMainWindow::closeEvent(QCloseEvent *event)
{
    QString lastProject = m_fileName;

    if (!closeProject()) {
        event->ignore();
        return;
    }

    KCONFIG->beginGroup("General");
    KCONFIG->setValue("LastProject", lastProject);
    KCONFIG->setValue("recents", m_recentProjects);

    KMainWindow::closeEvent(event);
}

/**
 * @if english 
 * This method creates a command for the paint area and include it into the undo/redo history.
 * @endif
 * @if spanish
 * Este metodo crea un comando para el area de dibujo y lo incluye en el historial de hacer/deshacer.
 * @endif
*/

void KTMainWindow::createCommand(const KTPaintAreaEvent *event)
{
    if (!m_viewDoc) 
        return;

    KTPaintAreaCommand *command = m_viewDoc->createCommand(event);

    if (command) 
        m_projectManager->undoHistory()->push(command);

}

/**
 * @if english 
 * This method adds a page/tab to the main window.
 * @endif
 * @if spanish
 * Este metodo adiciona una pagina/tab a la ventana principal.
 * @endif
*/

void KTMainWindow::addPage(QWidget *widget)
{
    addWidget(widget);
}

void KTMainWindow::updateAnimation(int index)
{
    if (index == 1) {
        kFatal() << "Updating video module! :)";
        viewCamera->updatePhotograms(m_projectManager->project());
    } else {
        kFatal() << "Updating illustration module! :)";
        m_viewDoc->updatePaintArea();
    }
}

/**
 * @if english 
 * This method exports the animation project to a video/image format.
 * @endif
 * @if spanish
 * Este metodo exporta un proyecto de animacion a un formato de video/imagen.
 * @endif
*/

void KTMainWindow::exportProject()
{
    KTExportWidget exportWidget(m_projectManager->project(), this);
    exportWidget.exec();
}
