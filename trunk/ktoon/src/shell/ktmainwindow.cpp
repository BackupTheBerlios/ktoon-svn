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

// KToon

#include "dtipdialog.h"
#include "ddebug.h"
#include "dimageeffect.h"
#include "ktapplication.h"
#include "ktpluginmanager.h"

#include "ktprojectcommand.h"

// dlslib
#include <dgui/ditabwidget.h>
#include <dgui/displitter.h>
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
//


KTMainWindow::KTMainWindow(KTSplash *splash) : DMainWindow(), m_viewDoc(0), m_animationSpace(0), m_exposureSheet(0), m_scenes(0)
{
	DINIT;
	
	DPlayer->loadEngine("gstreamer"); // FIXME: ponerlo en la configuración
	
	m_undoCommands = new QUndoStack(this);

	setObjectName("KTMainWindow_");
	
	m_osd = new DOsd( 0 );
	
	m_statusBar = new KTStatusBar(this);
	setStatusBar( m_statusBar );
	
	setWindowTitle( tr("KToon: 2D animation toolkit" ));
	m_renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt());
	
	
	m_projectManager = new KTProjectManager(this);
	splash->setMessage( tr("Setting up the project manager") );
	
	splash->setMessage( tr("Loading action manager..."));
	m_actionManager = new DActionManager(this);
	
	splash->setMessage( tr("Creating menu bar...") );
	setupActions();
	
	splash->setMessage( tr("Creating GUI...") );
	
	createGUI();
	setupMenu();
	setupToolBar();
	
	m_pActiveTabWidget->setCurrentIndex( 0 );
	
	DCONFIG->beginGroup("TipOfDay");
	bool showTips = qvariant_cast<bool>(DCONFIG->value("ShowOnStart", true ));
	
	
	if ( showTips )
	{
		QTimer::singleShot(0, this, SLOT(showTipDialog()));
	}
	
	KTPluginManager::instance()->loadPlugins();
}


KTMainWindow::~KTMainWindow()
{
	DEND;
	if ( m_osd ) delete m_osd;
}


// Modal

void KTMainWindow::createNewProject(const QString &name, const QSize &size, const int fps)
{
	if(!closeProject())
	{
		return;
	}
	
	m_projectManager->setupNewProject(name);
	
	newViewDocument( name);
}

void KTMainWindow::newViewDocument(const QString &title)
{
	if ( m_projectManager->isOpen())
	{
		messageToStatus(tr("Opening a new paint area..."));
		
		messageToOSD(tr("Opening a new document..."));
		
// 		KTScene *scene = m_projectManager->currentScene();
// 		
// 		m_renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt());
// 		
// 		if ( scene )
// 		{
// 			m_statusBar->advance(4);
			/*KTViewDocument **/m_viewDoc = new KTViewDocument(  m_projectManager->project(), m_renderType);
			connectToDisplays( m_viewDoc );
			m_viewDoc->setAttribute(Qt::WA_DeleteOnClose, true);
			addWidget( m_viewDoc, tr("Illustration"), true);
			connectToDisplays( m_viewDoc );
			ui4project( m_viewDoc );
// 			viewDocument->drawArea()->setPen( m_penWidget->pen());
// 			
			
// 			m_statusBar->advance(7);
// 			
			m_animationSpace = new KTWorkspace;
			m_animationSpace->setWindowIcon(QIcon(THEME_DIR+"/icons/animation_mode.png"));
			m_animationSpace->setScrollBarsEnabled ( true );
// 	
// 			connect(m_animationSpace, SIGNAL(contextMenu( const QPoint& )), this, SLOT(showAnimationMenu( const QPoint& )));
// 
			addWidget(m_animationSpace, tr("Animation"), true);
			
			m_pActiveTabWidget->setCurrentWidget(m_viewDoc);
			
// 			KTViewCamera *camera = qobject_cast<KTViewCamera *>(m_animationSpace->activeWindow());
// 			
// 			if ( camera )
// 			{
// 				camera->animationArea()->setScene( scene );
// 			}
// 			else
// 			{
// 				QWidgetList cameras = m_animationSpace->windowList();
// 				
// 				if ( cameras.count() > 0 )
// 				{
// 					camera = qobject_cast<KTViewCamera *>(cameras[0]);
// 					if(camera)
// 					{
// 						camera->animationArea()->setScene(scene);
// 					}
// 				}
// 				else
// 				{
// 					newViewCamera(scene);
// 				}
// 			}
// 			
// 			m_viewDoc->show();
// 			
// 			m_statusBar->advance(10);
// 			
// 			m_statusBar->setStatus(tr("Opened."));
// 		}
// 		else
// 		{
// 			m_statusBar->advance(0);
// 			m_statusBar->setStatus(tr("Project not open."));
// 		}
	}
}

void KTMainWindow::newViewCamera(KTScene *scene)
{
	if ( m_projectManager->isOpen() )
	{
// 		KTViewCamera *viewCamera = new KTViewCamera(m_projectManager->documentSize() , m_animationSpace);
// 		viewCamera->setAttribute(Qt::WA_DeleteOnClose, true);
// 		connect(viewCamera, SIGNAL(sendMessage(const QString &, int)), m_statusBar, SLOT(setStatus(const QString &, int)));
// 		connect(viewCamera, SIGNAL(sendProgress(int, int)), m_statusBar, SLOT(advance(int, int)));
// 		
// 		m_animationSpace->addWindow(viewCamera);
// 		
// 		if ( scene )
// 		{
// 			viewCamera->animationArea()->setScene( scene );
// 		}
// 		else
// 		{
// 			viewCamera->animationArea()->setScene(m_projectManager->currentScene());
// 		}
// 		
// 		viewCamera->show();
	}
}




void KTMainWindow::newProject()
{
	KTNewProject *wizard = new KTNewProject;
	connectToDisplays(wizard);
	if ( wizard->exec() != QDialog::Rejected )
	{
		createNewProject( wizard->projectName(), wizard->dimension(), wizard->fps() );
	}
	delete wizard;
}

bool KTMainWindow::closeProject()
{
	dDebug() << "Closing..";
	
	if(!m_projectManager->isOpen())
	{
		return true;
	}
	
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
// 			m_projectManager->save();
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
	
	m_pActiveTabWidget->setCurrentWidget(m_viewDoc);
	m_projectManager->closeProject();
	
// 	m_viewDoc->closeAllWindows();
	m_animationSpace->closeAllWindows();
	
// 	QWidgetList cameras = m_animationSpace->windowList();
// 	
// 	foreach (QWidget *widget, cameras)
// 	{
// 		KTViewCamera *view = qobject_cast<KTViewCamera *>(widget);
// 		
// 		if ( view )
// 		{
// 			view->animationArea()->setScene(0);
// 			view->animationArea()->setSize(m_projectManager->documentSize());
// 		}
// 	}
	
	// Clean widgets
	m_exposureSheet->closeAllScenes();
	m_timeLine->closeAllScenes();
	m_scenes->closeAllScenes();
	
	m_fileName = QString();
	
	return true;
}

void KTMainWindow::openProject()
{
	QString package = QFileDialog::getOpenFileName ( this, tr("Import project package"), CACHE_DIR, tr("KToon Project Package (*.ktn)"));
	
	if ( package.isEmpty() ) return;
	
	openProject( package );
}

void KTMainWindow::openProject(const QString &path)
{
	KTPackageHandler packageHandler;

	if ( packageHandler.importPackage(path) )
	{
		if ( closeProject() )
		{
			m_pActiveTabWidget->setCurrentWidget(m_viewDoc);
			
// 			if ( m_projectManager->load( packageHandler.importedProjectPath() ) )
// 			{
// 				m_fileName = path;
// 				
// 				if ( QDir::isRelativePath(path) )
// 				{
// 					m_fileName = QDir::currentPath()+"/"+path;
// 				}
// 				
// 				int pos = m_recentProjects.indexOf(m_fileName);
// 				if ( pos == -1 )
// 				{
// 					if ( m_recentProjects.count() <= 6 )
// 					{
// 						m_recentProjects << m_fileName;
// 					}
// 					else
// 					{
// 						m_recentProjects.push_front(m_fileName);
// 					}
// 				}
// 				else
// 				{
// 					m_recentProjects.push_front(m_recentProjects.takeAt(pos));
// 				}
// 				
// 				newViewDocument();
// 				
// 				messageToOSD( tr("Project opened!"));
// 			}
		}
	}
}


void KTMainWindow::save()
{
	dDebug() << "Saving..";
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
	dDebug() << "Importing";
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
	connect(widget, SIGNAL(eventTriggered(const KTProjectEvent *)), this, SLOT(createCommand(const KTProjectEvent *)));
	connect(m_projectManager, SIGNAL(commandExecuted(KTProjectEvent* )), widget, SLOT(handleProjectEvent(KTProjectEvent *)));
}

void KTMainWindow::messageToStatus(const QString &msg)
{
	m_statusBar->setStatus(msg, msg.length() * 90);
}

void KTMainWindow::messageToOSD(const QString &msg, DOsd::Level level)
{
	m_osd->display( msg, level, msg.length()*90);
}

void KTMainWindow::showHelpPage(const QString &title, const QString &filePath)
{
	D_FUNCINFO;
	KTHelpBrowser *page = new KTHelpBrowser(this);
	page->setDataDirs( QStringList() << m_helper->helpPath() );
	
// 	page->setDocument( document );
	page->setSource( filePath);
	addWidget( page, tr("Help:%1").arg(title) );
}

void KTMainWindow::saveProject()
{
// 	m_projectManager->save();
// 	
// 	if ( m_fileName.isEmpty() )
// 	{
// 		saveProjectAs();
// 		return;
// 	}
// 	
// 	if( !m_fileName.endsWith(".ktn"))
// 	{
// 		m_fileName += ".ktn";
// 	}
// 	
// 	dDebug() << "Saving " << m_fileName;
// 	
// 	KTPackageHandler packageHandler;
// 	
// 	bool ok = packageHandler.makePackage(CACHE_DIR+"/"+m_projectManager->projectName(), m_fileName);
// 	
// 	if ( ok )
// 	{
// 		messageToOSD( tr("Project saved!"));
// 	}
	
}

void KTMainWindow::saveProjectAs()
{
// 	m_projectManager->save();
// 	
// 	m_fileName = QFileDialog::getSaveFileName( this, tr("Build project package"), CACHE_DIR, "KToon Project Package (*.ktn)");
// 	
// 	if ( m_fileName.isEmpty() ) return;
// 	
// 	if( !m_fileName.endsWith(".ktn"))
// 	{
// 		m_fileName += ".ktn";
// 	}
// 	
// 	KTPackageHandler packageHandler;
// 	
// 	bool ok = packageHandler.makePackage(CACHE_DIR+"/"+m_projectManager->projectName(), m_fileName);
// 	
// 	
// 	if ( ok )
// 	{
// 		messageToOSD( tr("Project saved in %1!").arg(m_fileName) );
// 	}
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
	if (! closeProject() )
	{
		event->ignore();
		return;
	}
	
	delete m_pBottomDock;
	delete m_pLeftDock;
	delete m_pRightDock;
	
	DCONFIG->beginGroup("General");
	DCONFIG->setValue("recents", m_recentProjects);
	
	m_osd->close();
	
	DMainWindow::closeEvent(event);
}

void KTMainWindow::createCommand(const KTProjectEvent *event)
{
	D_FUNCINFO;
	
	if ( !event->isValid() )
	{
		dDebug() << "Invalid event!";
		return;
	}
	
	KTProjectCommand *command = m_projectManager->createCommand(event);
	
	if ( command )
	{
		m_undoCommands->push(command);
	}
}


