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

#include "ktdebug.h"
#include "kimageeffect.h"

#include "ktapplication.h"
#include "ktabout.h"

#include "kthelpbrowser.h"

#include "ktpaletteimporter.h"

#include "kttip.h"

// dlslib
#include "dtabwidget.h"
#include "docksplitter.h"

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

KTMainWindow::KTMainWindow(KTSplash *splash) : DMainWindow(), m_exposureSheet(0), m_scenes(0)
{
	KTINIT;
	
	setObjectName("KTMainWindow_");
	
	m_osd = new KTOsd( centralWidget() );
	
	m_statusBar = new KTStatusBar(this);
	setStatusBar( m_statusBar );
	
	setWindowTitle(tr("KToon: 2D animation toolkit"));
	
	m_projectManager = new KTProjectManager(this);
	splash->setMessage( tr("Setting up the project manager") );
	
	m_drawingSpace = new KTWorkspace;
	m_drawingSpace->setScrollBarsEnabled ( true );

// 	m_drawingSpace->setBackground(QBrush(QPixmap(background_xpm))); 
	
	addWidget(m_drawingSpace, tr("Illustration"), true);
	
	m_animationSpace = new KTWorkspace;
	m_animationSpace->setScrollBarsEnabled ( true );
	

	addWidget(m_animationSpace, tr("Animation"), true);
	
	
	setupBackground();
	
	splash->setMessage( tr("Loading action manager..."));
	m_actionManager = new KTActionManager(this);
	
	// Create the menubar;
	splash->setMessage( tr("Creating menu bar..."));
	setupFileActions();
	setupEditActions();
	setupProjectActions();
	setupHelpActions();
	
	setupMenu();
	
	splash->setMessage( tr("Creating GUI..."));
	createGUI();
	
	m_pActiveTabWidget->setCurrentIndex( 0 );
	
	createNewProject("test", QSize(300,300), tr("Image"), 24);
	
	KTCONFIG->beginGroup("TipOfDay");
	bool showTips = qvariant_cast<bool>(KTCONFIG->value("ShowOnStart", true ));
	
	
	if ( showTips )
	{
		QTimer::singleShot(0, this, SLOT(showTipDialog()));
	}
}


KTMainWindow::~KTMainWindow()
{
	KTEND;
	
	if ( m_animationSpace )
		delete m_animationSpace;
	
	if ( m_drawingSpace )
		delete m_drawingSpace;
	
	if ( m_projectManager )
		delete m_projectManager;
}


// Modal

void KTMainWindow::createNewProject(const QString &name, const QSize &size, const QString & renderType, const int fps)
{
	closeProject();
	
	m_projectManager->init();
	
	KTDocument *document = m_projectManager->createDocument(name);
	m_projectManager->setCurrentDocument(0);
	
// 	newViewDocument( name, size);
// 	m_viewCamera->animationArea()->setScene(m_projectManager->currentScene());
	
	m_projectManager->setProjectName( name );
	m_projectManager->setProjectRender( renderType);
	m_projectManager->setProjectFPS( fps);
	m_projectManager->setDocumentSize( size );
	
	
	// Add by default a scene, layer, frame
	m_projectManager->createScene( true );
	m_projectManager->createLayer( true );
	m_projectManager->createFrame( true);
}

void KTMainWindow::newViewDocument(const QString &name)
{
	messageToStatus(tr("Opening a new document..."));
	
	messageToOSD(tr("Opening a new document..."));
	
	KTScene *scene = m_projectManager->currentScene();
	
	if ( scene )
	{
		
		KTViewDocument *viewDocument = new KTViewDocument( m_projectManager->documentSize(),  name, m_projectManager->projectRender(),  m_projectManager->currentDocument(), m_drawingSpace);
		viewDocument->setAttribute(Qt::WA_DeleteOnClose, true);
		m_drawingSpace->addWindow(viewDocument);
// 		viewDocument->setWindowTitle(name);
		
		m_statusBar->advance(4);
// 		viewDocument->setActiveWindow();
		m_statusBar->advance(7);
		viewDocument->show();
		m_statusBar->advance(10);
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
		
		m_statusBar->setStatus(tr("Opened."));
	}
	else
	{
		m_statusBar->advance(0);
		m_statusBar->setStatus(tr("Project not open."));
	}
}

void KTMainWindow::newViewCamera(KTScene *scene)
{
	KTViewCamera *viewCamera = new KTViewCamera(m_projectManager->documentSize() , m_animationSpace);
	viewCamera->setAttribute(Qt::WA_DeleteOnClose, true);
	connect(viewCamera, SIGNAL(sendMessage(const QString &, int)), m_statusBar, SLOT(setStatus(const QString &, int)));
	connect(viewCamera, SIGNAL(sendProgress(int, int)), m_statusBar, SLOT(advance(int, int)));
		
	m_animationSpace->addWindow(viewCamera);
		
	viewCamera->animationArea()->setScene( scene );
		
	viewCamera->show();
}

void KTMainWindow::newProject()
{
	KTNewProject *wizard = new KTNewProject;
	connectToDisplays(wizard);
	if ( wizard->exec() != QDialog::Rejected )
	{
		createNewProject( wizard->projectName(), wizard->dimension(), wizard->renderType(), wizard->fps() );
	}
	delete wizard;
}

void KTMainWindow::closeProject()
{
	ktDebug() << "Closing..";
	if(!m_projectManager->isOpen())
	{
		return;
	}
	QMessageBox mb(QApplication::applicationName (), "Do you want to save?",
		       QMessageBox::Information,
		       QMessageBox::Yes | QMessageBox::Default,
		       QMessageBox::No,
		       QMessageBox::Cancel | QMessageBox::Escape);
	mb.setButtonText(QMessageBox::Yes, "Save");
	mb.setButtonText(QMessageBox::No, "Discard");
	
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
			return;
		}
		break;
	}
	
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
	m_scenes->closeAllScenes();
}

void KTMainWindow::openProject()
{
	KTFileDialog opener(KTFileDialog::Repository, this);
	if ( opener.exec() != QDialog::Rejected )
	{
		QString fileName = opener.fileName();
		
		QString path = KTOON_REPOSITORY+"/"+fileName.left(fileName.length()-4)+"/"+fileName;
		
		closeProject();
		m_projectManager->load( path );
	}
}

void KTMainWindow::save()
{
	ktDebug() << "Saving.." << endl;
	QTimer::singleShot(100, m_projectManager, SLOT(save()));
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
	KTTipDialog *tipDialog = new KTTipDialog(KTOON_DATA_DIR+"/tips", this);
	tipDialog->show();
// 	tipDialog.exec();
}

void KTMainWindow::importPalettes()
{
	ktDebug() << "Importing";
	QStringList files = QFileDialog::getOpenFileNames( this, tr("Import gimp palettes"), QString(), "Gimp Palette (*.gpl)");
	
	m_statusBar->setStatus( tr("Importing palettes"));
	QStringList::ConstIterator it = files.begin();
	
	int progress = 1;
	while( it != files.end() )
	{
		KTPaletteImporter importer;
		importer.import( *it, KTPaletteImporter::Gimp);
		++it;
		importer.saveFile(ktapp->configDir()+"/palettes");
		
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
		doc->drawArea()->currentBrush()->setPenBrush(foreground);
		doc->drawArea()->currentBrush()->setBrush(background);
	}
}

void KTMainWindow::changeCurrentBrush(const KTBrush *form)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->setBrush( form );
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
	
	connect(widget, SIGNAL(requestMoveFrame(bool)), m_projectManager, SLOT(moveFrame(bool)));
	
	connect(widget, SIGNAL(requestRemoveFrame()), m_projectManager, SLOT(removeFrame()));
	
	connect(widget, SIGNAL(requestLockFrame()), m_projectManager, SLOT(lockCurrentFrame()));
	
	connect(widget, SIGNAL(requestRemoveLayer(int)), m_projectManager, SLOT(removeLayer(int)));
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
	KT_FUNCINFO;
	KTHelpBrowser *page = new KTHelpBrowser(this);
	page->setDataDirs( QStringList() << m_helper->helpPath() );
	
// 	page->setDocument( document );
	page->setSource( filePath);
	addWidget( page, tr("Help:%1").arg(title) );
}
