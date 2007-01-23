/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

// KToon

#include "dtipdialog.h"
#include "ddebug.h"
#include "dimageeffect.h"
#include "ktapplication.h"
#include "ktpluginmanager.h"

#include "ktprojectcommand.h"
#include "ktnetprojectmanagerhandler.h"
#include "ktnetprojectmanagerparams.h"
#include "ktlocalprojectmanagerhandler.h"
#include "ktconnectdialog.h"
#include "ktsavenetproject.h"

#include <dsound/daudioplayer.h>

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

#include "ktchat.h"

KTMainWindow::KTMainWindow(KTSplash *splash) : DTabbedMainWindow(), m_projectManager(0), m_viewDoc(0), m_animationSpace(0), m_exposureSheet(0), m_scenes(0)
{
	DINIT;
	
	DAudioPlayer::instance()->loadEngine("gstreamer"); // FIXME: ponerlo en la configuración
	
	setObjectName("KTMainWindow_");
	
	m_statusBar = new KTStatusBar(this);
	setStatusBar( m_statusBar );
	
	setWindowTitle( tr("KToon: 2D animation toolkit" ) );
	m_renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt());
	
	
	m_projectManager = new KTProjectManager(this);
	
	m_projectManager->setHandler( new KTLocalProjectManagerHandler );
	
// 	setProjectManager( projectManager );
	
	splash->setMessage( tr("Setting up the project manager") );
	
	splash->setMessage( tr("Loading action manager...") );
	m_actionManager = new DActionManager(this);
	
	splash->setMessage( tr("Creating menu bar...") );
	setupActions();
	
	splash->setMessage( tr("Creating GUI...") );
	
	createGUI();
	setupMenu();
	setupToolBar();
	
	DCONFIG->beginGroup("TipOfDay");
	bool showTips = qvariant_cast<bool>(DCONFIG->value("ShowOnStart", true ));
	
	
	if ( showTips )
	{
		QTimer::singleShot(0, this, SLOT(showTipDialog()));
	}
	
	KTPluginManager::instance()->loadPlugins();
	setCurrentPerspective( Drawing );
	
	DCONFIG->beginGroup("General");
	
	bool openLast = DCONFIG->value("OpenLastProject", true).toBool();
	
	if ( openLast )
	{
		openProject(DCONFIG->value("LastProject").toString());
	}
}


KTMainWindow::~KTMainWindow()
{
	DEND;
	delete KTPluginManager::instance();
	delete DOsd::self();
}

// Modal

void KTMainWindow::createNewProject(KTProjectManagerParams *params)
{
	if(!closeProject())
	{
		return;
	}
	
	m_projectManager->setParams(params);
	
	m_projectManager->setupNewProject();
	
	newViewDocument( params->projectName() );
}

void KTMainWindow::newViewDocument(const QString &title)
{
	if ( m_projectManager->isOpen())
	{
		messageToStatus(tr("Opening a new paint area..."));
		DOsd::self()->display(tr("Opening a new document..."));
		
		m_viewDoc = new KTViewDocument(m_projectManager->project());
		
		connectToDisplays( m_viewDoc );
		
// 		m_viewDoc->setAttribute(Qt::WA_DeleteOnClose, true);
		m_viewDoc->setWindowTitle(tr("Illustration: %1").arg(title) );
		addWidget( m_viewDoc, true, Drawing);
		connectToDisplays( m_viewDoc );
		ui4project( m_viewDoc );
		
		m_animationSpace = new KTWorkspace;
		m_animationSpace->setWindowIcon(QIcon(THEME_DIR+"/icons/animation_mode.png"));
		m_animationSpace->setScrollBarsEnabled ( true );
		
		m_animationSpace->setWindowTitle(tr("Animation"));
		addWidget(m_animationSpace, true, Animation);
		
		KTViewCamera *viewCamera = m_cameraWidget->viewCamera();
		ui4project( viewCamera );
		
		m_animationSpace->addWindow(viewCamera);
		viewCamera->showMaximized();
		
		setCurrentPerspective( Drawing );
	}
}

void KTMainWindow::newProject()
{
	KTNewProject *wizard = new KTNewProject;
// 	connectToDisplays(wizard);
	if ( wizard->exec() != QDialog::Rejected )
	{
		if ( wizard->useNetwork() )
		{
			m_projectManager->setHandler( new KTNetProjectManagerHandler );
		}
		else
		{
			m_projectManager->setHandler( new KTLocalProjectManagerHandler );
		}
		
		createNewProject( wizard->params() );
	}
	delete wizard;
}

bool KTMainWindow::closeProject()
{
	if(!m_projectManager->isOpen())
	{
		return true;
	}
	
	if ( m_projectManager->isModified() )
	{
		QMessageBox mb(QApplication::applicationName (), tr("Do you want to save?"),
			QMessageBox::Information,
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No,
			QMessageBox::Cancel | QMessageBox::Escape);
		mb.setButtonText(QMessageBox::Yes, tr("Save"));
		mb.setButtonText(QMessageBox::No, tr("Discard"));
		
		switch(mb.exec())
		{
			case QMessageBox::Yes:
			{
				saveProject();
			}
			break;
			case QMessageBox::No:
			{
			}
			break;
			case QMessageBox::Cancel:
			{
				return false;
			}
			break;
		}
	}
	
	setUpdatesEnabled(false);
	
	m_viewDoc->closeArea();
	m_animationSpace->closeAllWindows();
	
	removeWidget(m_animationSpace, true);
	
	removeWidget(m_viewDoc, true);
	delete m_viewDoc;
	m_viewDoc = 0;
	
	m_projectManager->closeProject();
	
	// Clean widgets
	
	m_exposureSheet->closeAllScenes();
	m_timeLine->closeAllScenes();
	m_scenes->closeAllScenes();
	
	m_fileName = QString();
	
	setUpdatesEnabled(true);
	
	return true;
}

void KTMainWindow::openProject()
{
	QString package = QFileDialog::getOpenFileName( this, tr("Import project package"), CACHE_DIR, tr("KToon Project Package (*.ktn);;KToon Net Project (*.ktnet)"));
	
	if( package.isEmpty() ) return;
	
	openProject( package );
}

void KTMainWindow::openProject(const QString &path)
{
	dWarning() << "Opening project: " << path;
	
	if(path.isEmpty() ) return;
	
	if ( path.endsWith(".ktnet"))
	{
		KTSaveNetProject loader;
		KTNetProjectManagerParams *params = loader.params(path);
		
		KTConnectDialog cndialog;
		cndialog.setServer(params->server());
		cndialog.setPort(params->port());
		
		if ( cndialog.exec() == QDialog::Accepted )
		{
			m_projectManager->setHandler( new KTNetProjectManagerHandler );
			m_projectManager->setParams(params);
		}
		else
		{
			return;
		}
	}
	
	if ( closeProject() )
	{
		setUpdatesEnabled(false);
		tabWidget()->setCurrentWidget(m_viewDoc);
		
		if ( m_projectManager->loadProject( path ) )
		{
			if ( QDir::isRelativePath(path) )
			{
				m_fileName = QDir::currentPath()+"/"+path;
			}
			else
			{
				m_fileName = path;
			}
			
			newViewDocument( m_projectManager->project()->projectName() );
			
			// HACK
			KTFrameResponse response(KTProjectRequest::Frame, KTProjectRequest::Select);
			response.setFrameIndex(0);
			response.setSceneIndex(0);
			response.setLayerIndex(0);
			m_viewDoc->handleProjectResponse(&response);
			m_exposureSheet->handleProjectResponse(&response);
			m_timeLine->handleProjectResponse(&response);
			
			{
				int pos = m_recentProjects.indexOf(m_fileName);
				if ( pos == -1 )
				{
					if ( m_recentProjects.count() <= 6 )
					{
						m_recentProjects << m_fileName;
					}
					else
					{
						m_recentProjects.push_front(m_fileName);
					}
				}
				else
				{
					m_recentProjects.push_front(m_recentProjects.takeAt(pos));
				}
				
				updateOpenRecentMenu(m_recentProjectsMenu, m_recentProjects);
			}
			
			setUpdatesEnabled(true);
			
			DOsd::self()->display( tr("Project %1 opened!").arg(m_projectManager->project()->projectName()) );
		}
		else
		{
			setUpdatesEnabled(true);
			DOsd::self()->display( tr("Cannot open project!"), DOsd::Error );
		}
	}
}


void KTMainWindow::save()
{
	dDebug("project") << "Saving..";
	QTimer::singleShot(0, this, SLOT(saveProject()));
}

void KTMainWindow::preferences()
{
	m_statusBar->setStatus( tr( "Preferences Dialog Opened" ), 2000 );
	KTPreferences *preferences = new KTPreferences( this );
	preferences->exec();
	
	delete preferences;
}

void KTMainWindow::aboutKToon()
{
	KTAbout *about = new KTAbout(this);
	about->exec();
	
	delete about;
}

void KTMainWindow::showTipDialog()
{
	DTipDialog *tipDialog = new DTipDialog(DATA_DIR+"/tips", this);
	tipDialog->show();
// 	tipDialog.exec();
}

void KTMainWindow::importPalettes()
{
	QStringList files = QFileDialog::getOpenFileNames( this, tr("Import gimp palettes"), QString(), "Gimp Palette (*.gpl)");
	
	m_statusBar->setStatus( tr("Importing palettes"));
	QStringList::ConstIterator it = files.begin();
	
	int progress = 1;
	while( it != files.end() )
	{
		KTPaletteImporter importer;
		importer.import( *it, KTPaletteImporter::Gimp);
		++it;
		importer.saveFile(CONFIG_DIR+"/palettes");
		
		m_colorPalette->parsePaletteFile( importer.filePath() );
		
		m_statusBar->advance( progress++, files.count());
	}
}

void KTMainWindow::ui4project(QWidget *widget)
{
	connect(widget, SIGNAL(requestTriggered(const KTProjectRequest *)), m_projectManager, SLOT(handleProjectRequest(const KTProjectRequest *)));
	
	connect(m_projectManager, SIGNAL(responsed( KTProjectResponse* )), widget, SLOT(handleProjectResponse(KTProjectResponse *)));
}

void KTMainWindow::ui4paintArea(QWidget *widget)
{
	connect(widget, SIGNAL(paintAreaEventTriggered(const KTPaintAreaEvent *)), this, SLOT(createCommand(const KTPaintAreaEvent *)));
}

void KTMainWindow::messageToStatus(const QString &msg)
{
	m_statusBar->setStatus(msg, msg.length() * 90);
}

void KTMainWindow::showHelpPage(const QString &title, const QString &filePath)
{
	D_FUNCINFO;
	KTHelpBrowser *page = new KTHelpBrowser(this);
	page->setDataDirs( QStringList() << m_helper->helpPath() );
	
// 	page->setDocument( document );
	page->setSource( filePath );
	page->setWindowTitle(tr("Help:%1").arg(title));
	addWidget( page, false, All );
}

void KTMainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName( this, tr("Build project package"), CACHE_DIR, "KToon Project Package (*.ktn);;KToon Net Project (*.ktnet)");
	
	if ( fileName.isEmpty() )
	{
		return;
	}
	
	m_fileName = fileName;
	save();
}

void KTMainWindow::saveProject()
{
	if ( m_fileName.isEmpty() )
	{
		saveAs();
		return;
	}
	
	if( !m_fileName.endsWith(".ktn"))
	{
		m_fileName += ".ktn"; // FIXME
	}
	
	if ( m_projectManager->saveProject(m_fileName) )
	{
		DOsd::self()->display(tr("Project %1 saved").arg(m_projectManager->project()->projectName()), DOsd::Info);
	}
	else
	{
		DOsd::self()->display(tr("Cannot save the project!"), DOsd::Error );
	}
}

void KTMainWindow::openRecentProject()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if ( action )
	{
		openProject( action->text() );
	}
}

void KTMainWindow::showAnimationMenu(const QPoint &p)
{
	QMenu *menu = new QMenu(tr("Animation"), m_animationSpace);
	menu->addAction(tr("New camera"), this, SLOT(newViewCamera()));
	menu->exec(p);
	delete menu;
}


void KTMainWindow::closeEvent( QCloseEvent *event )
{
	QString lastProject = m_fileName;
	
	if (! closeProject() )
	{
		event->ignore();
		return;
	}
	
	DCONFIG->beginGroup("General");
	DCONFIG->setValue("LastProject", lastProject);
	DCONFIG->setValue("recents", m_recentProjects);
	
	DMainWindow::closeEvent(event);
}

void KTMainWindow::createCommand(const KTPaintAreaEvent *event)
{
	if ( !m_viewDoc ) return;
	
	KTPaintAreaCommand *command = m_viewDoc->createCommand(event);
	
	if ( command )
	{
		m_projectManager->undoHistory()->push(command);
	}
}



