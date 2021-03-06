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

#include "dtip.h"
#include "ddebug.h"
#include "kimageeffect.h"
#include "ktapplication.h"
#include "ktpluginmanager.h"

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

KTMainWindow::KTMainWindow(KTSplash *splash) : DTabbedMainWindow(), m_exposureSheet(0), m_scenes(0)
{
	DINIT;
	
	setObjectName("KTMainWindow_");
	
	m_osd = new KTOsd( centralWidget() );
	
	m_statusBar = new KTStatusBar(this);
	setStatusBar( m_statusBar );
	
	setWindowTitle(tr("KToon: 2D animation toolkit"));
	m_renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt());
	
	
	m_projectManager = new KTProjectManager(this);
	splash->setMessage( tr("Setting up the project manager") );
	
	m_drawingSpace = new KTWorkspace;
	m_drawingSpace->setWindowIcon(QIcon(THEME_DIR+"/icons/illustration_mode.png"));
	m_drawingSpace->setScrollBarsEnabled( true );
	m_drawingSpace->setWindowTitle(tr("Illustration"));
	
	addWidget(m_drawingSpace, true);
	
	m_animationSpace = new KTWorkspace;
	m_animationSpace->setWindowIcon(QIcon(THEME_DIR+"/icons/animation_mode.png"));
	m_animationSpace->setScrollBarsEnabled ( true );
	
	connect(m_animationSpace, SIGNAL(contextMenu( const QPoint& )), this, SLOT(showAnimationMenu( const QPoint& )));
	
	m_animationSpace->setWindowTitle(tr("Animation"));
	addWidget(m_animationSpace, true);
	
	splash->setMessage( tr("Loading action manager..."));
	m_actionManager = new DActionManager(this);
	
// 	Create the menubar;
	splash->setMessage( tr("Creating menu bar..."));
	setupActions();
	
	splash->setMessage( tr("Creating GUI..."));
	
	createGUI();
	
	setupMenu();
	
// 	m_pActiveTabWidget->setCurrentIndex( 0 );
	
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
}


// Modal

void KTMainWindow::createNewProject(const QString &name, const QSize &size, const int fps)
{
	if(!closeProject()) return;
	
	m_projectManager->init();
	
	/*KTDocument *document = */m_projectManager->createDocument(name);
	m_projectManager->setCurrentDocument(0);
	
// 	newViewDocument( name, size);
// 	m_viewCamera->animationArea()->setScene(m_projectManager->currentScene());
	
	m_projectManager->setProjectName( name );
// 	m_projectManager->setProjectRender( renderType );
	m_projectManager->setProjectFPS( fps);
	m_projectManager->setDocumentSize( size );
	
	
	// Add by default a scene, layer, frame
	m_projectManager->createScene( true );
	m_projectManager->createLayer( true );
	m_projectManager->createFrame( true );
}

void KTMainWindow::newViewDocument(const QString &name)
{
	if ( m_projectManager->isOpen())
	{
		messageToStatus(tr("Opening a new paint area..."));
		
		messageToOSD(tr("Opening a new document..."));
		
		KTScene *scene = m_projectManager->currentScene();
		
		m_renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt());
		
		if ( scene )
		{
			m_statusBar->advance(4);
			KTViewDocument *viewDocument = new KTViewDocument( m_projectManager->documentSize(),  name, m_renderType,  m_projectManager->currentDocument(), m_drawingSpace);
			
			viewDocument->setAttribute(Qt::WA_DeleteOnClose, true);
			
			viewDocument->drawArea()->setPen( m_penWidget->pen());
			
			m_drawingSpace->addWindow(viewDocument);
			m_statusBar->advance(7);
			
			KTViewCamera *camera = qobject_cast<KTViewCamera *>(m_animationSpace->activeWindow());
			
			if ( camera )
			{
				camera->animationArea()->setScene( scene );
			}
			else
			{
				QWidgetList cameras = m_animationSpace->windowList();
				
				if ( cameras.count() > 0 )
				{
					camera = qobject_cast<KTViewCamera *>(cameras[0]);
					if(camera)
					{
						camera->animationArea()->setScene(scene);
					}
				}
				else
				{
					newViewCamera(scene);
				}
			}
			
			viewDocument->show();
			
			m_statusBar->advance(10);
			
			m_statusBar->setStatus(tr("Opened."));
		}
		else
		{
			m_statusBar->advance(0);
			m_statusBar->setStatus(tr("Project not open."));
		}
	}
}

void KTMainWindow::newViewCamera(KTScene *scene)
{
	if ( m_projectManager->isOpen() )
	{
		KTViewCamera *viewCamera = new KTViewCamera(m_projectManager->documentSize() , m_animationSpace);
		viewCamera->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(viewCamera, SIGNAL(sendMessage(const QString &, int)), m_statusBar, SLOT(setStatus(const QString &, int)));
		connect(viewCamera, SIGNAL(sendProgress(int, int)), m_statusBar, SLOT(advance(int, int)));
		
		m_animationSpace->addWindow(viewCamera);
		
		if ( scene )
		{
			viewCamera->animationArea()->setScene( scene );
		}
		else
		{
			viewCamera->animationArea()->setScene(m_projectManager->currentScene());
		}
		
		viewCamera->show();
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
			m_projectManager->save();
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
	
// 	m_pActiveTabWidget->setCurrentWidget(m_drawingSpace);
	m_projectManager->close();
	
	m_drawingSpace->closeAllWindows();
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
	QString package = QFileDialog::getOpenFileName ( this, tr("Import project package"), REPOSITORY, tr("KToon Project Package (*.ktn)"));
	
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
// 			m_pActiveTabWidget->setCurrentWidget(m_drawingSpace);
			
			if ( m_projectManager->load( packageHandler.importedProjectPath() ) )
			{
				m_fileName = path;
				
				if ( QDir::isRelativePath(path) )
				{
					m_fileName = QDir::currentPath()+"/"+path;
				}
				
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
				
				newViewDocument();
				
				messageToOSD( tr("Project opened!"));
			}
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

// Drawing
void KTMainWindow::changeCurrentColors(const QBrush &foreground, const QBrush &background)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow());
	
	if ( doc )
	{
// 		doc->drawArea()->currentBrush()->setPenBrush(foreground);
// 		doc->drawArea()->currentBrush()->setBrush(background);
		doc->drawArea()->setColors(foreground, background);
	}
}

void KTMainWindow::changeCurrentPen(const QPen &pen)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->setPen( pen );
	}
}

void KTMainWindow::changeFPS(int fps)
{
	KTScene *scene = m_projectManager->currentScene();
	
	if ( scene )
	{
		scene->setFPS( fps );
	}
}

void KTMainWindow::ui4project(QWidget *widget)
{
	connect(widget, SIGNAL(requestInsertFrame(bool)), m_projectManager, SLOT(createFrame(bool)));
	
	connect(widget, SIGNAL(requestInsertLayer()), m_projectManager, SLOT(createLayer()));
	
	connect(widget, SIGNAL(frameSelected( int, int )), this, SLOT(selectFrame(int,int)));
	
	connect(widget, SIGNAL(layerVisibilityChanged( int, bool)), m_projectManager, SLOT(setLayerVisibility( int, bool)));
	
	connect(widget, SIGNAL(layerSelected( int)), m_projectManager, SLOT(setCurrentLayer( int)));
	
	connect(widget, SIGNAL(requestCopyFrame(int)), m_projectManager, SLOT(copyFrame(int)));
	
	connect(widget, SIGNAL(requestPasteFrame(int)), m_projectManager, SLOT(pasteFrame(int)));
	
	connect(widget, SIGNAL(requestCloneFrame(int, int)), m_projectManager, SLOT(cloneFrame(int, int)));
	
	connect(widget, SIGNAL(requestMoveFrame(bool)), m_projectManager, SLOT(moveFrame(bool)));
	
	connect(widget, SIGNAL(requestRemoveFrame()), m_projectManager, SLOT(removeFrame()));
	
	connect(widget, SIGNAL(requestLockFrame()), m_projectManager, SLOT(lockCurrentFrame()));
	
	connect(widget, SIGNAL(requestRemoveLayer(int)), m_projectManager, SLOT(removeLayer(int)));
	
	
	connect(widget, SIGNAL(requestMoveLayer(bool)), m_projectManager, SLOT(moveLayer(bool)));
	
	connect(widget, SIGNAL(requestRenameLayer(int, const QString &)), m_projectManager, SLOT(renameLayer(int , const QString &)));
	
	connect(widget, SIGNAL(requestRenameFrame(int, int, const QString &)), m_projectManager, SLOT(renameFrame(int, int, const QString &)));
}

void KTMainWindow::messageToStatus(const QString &msg)
{
	m_statusBar->setStatus(msg, msg.length() * 90);
}

void KTMainWindow::messageToOSD(const QString &msg, int level)
{
	m_osd->display( msg, KTOsd::Level(level), msg.length()*90);
}

void KTMainWindow::showHelpPage(const QString &title, const QString &filePath)
{
	D_FUNCINFO;
	KTHelpBrowser *page = new KTHelpBrowser(this);
	page->setDataDirs( QStringList() << m_helper->helpPath() );
	
// 	page->setDocument( document );
	page->setSource( filePath);
	page->setWindowTitle(tr("Help:%1").arg(title));
	addWidget( page );
}

void KTMainWindow::saveProject()
{
	m_projectManager->save();
	
	if ( m_fileName.isEmpty() )
	{
		saveProjectAs();
		return;
	}
	
	if( !m_fileName.endsWith(".ktn"))
	{
		m_fileName += ".ktn";
	}
	
	dDebug() << "Saving " << m_fileName;
	
	KTPackageHandler packageHandler;
	
	bool ok = packageHandler.makePackage(REPOSITORY+"/"+m_projectManager->projectName(), m_fileName);
	
	if ( ok )
	{
		messageToOSD( tr("Project saved!"));
	}
	
}

void KTMainWindow::saveProjectAs()
{
	m_projectManager->save();
	
	m_fileName = QFileDialog::getSaveFileName( this, tr("Build project package"), REPOSITORY, "KToon Project Package (*.ktn)");
	
	if ( m_fileName.isEmpty() ) return;
	
	if( !m_fileName.endsWith(".ktn"))
	{
		m_fileName += ".ktn";
	}
	
	KTPackageHandler packageHandler;
	
	bool ok = packageHandler.makePackage(REPOSITORY+"/"+m_projectManager->projectName(), m_fileName);
	
	
	if ( ok )
	{
		messageToOSD( tr("Project saved in %1!").arg(m_fileName) );
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
	if (! closeProject() )
	{
		event->ignore();
		return;
	}
	
	DCONFIG->beginGroup("General");
	DCONFIG->setValue("recents", m_recentProjects);
	
	DMainWindow::closeEvent(event);
}

