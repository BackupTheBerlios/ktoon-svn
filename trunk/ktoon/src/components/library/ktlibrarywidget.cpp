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

#include "ktlibrarywidget.h"

#include "dglobal.h"
#include "ddebug.h"
#include "doptionaldialog.h"
#include "dconfig.h"

#include <QGroupBox>
#include <QFileDialog>

#include <cstdlib>
#include <ctime>


KTLibraryWidget::KTLibraryWidget(QWidget *parent) : KTModuleWidgetBase(parent), m_childCount(0)
{
	DINIT;
	
	setWindowIcon(QPixmap(THEME_DIR+"/icons/library.png"));
	setWindowTitle(tr("Library"));
	
	
	m_libraryDir = QDir(CONFIG_DIR+"/libraries");
	
	m_display = new KTDisplayGraphic(this);
	
	m_libraryTree = new KTGCTable(this);

	connect(m_libraryTree, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(drawCurrentItem(QTreeWidgetItem *, int)));
	connect(m_libraryTree, SIGNAL(itemRenamed( QTreeWidgetItem* )), this, SLOT(renameObject( QTreeWidgetItem* )));
	
// 	m_libraryTree->createFolder( tr("Custom") );
	
	setup();
	
	QGroupBox *m_buttons = new QGroupBox(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttons);
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(0);
	
	DImageButton *addGC = new DImageButton(QPixmap(THEME_DIR+"/icons/plussign.png" ), 22, m_buttons);
	connect(addGC, SIGNAL(clicked()), this, SIGNAL(requestCurrentGraphic()));
	
	buttonLayout->addWidget(addGC);
	addGC->setToolTip(tr( "Add the current graphic to Library" ));
	
	DImageButton *delGC = new DImageButton(QPixmap(THEME_DIR+"/icons/minussign.png" ), 22, m_buttons);
	connect(delGC, SIGNAL(clicked()), this, SLOT(removeCurrentGraphic()));
	
	delGC->setToolTip(tr( "Remove the selected Symbol from Library" ));
	buttonLayout->addWidget(delGC);
	
	DImageButton *gctoDrawingArea = new DImageButton(QPixmap(THEME_DIR+"/icons/insert_cg.png" ), 22, m_buttons);
	connect(gctoDrawingArea, SIGNAL(clicked()), this, SLOT(emitSelectedComponent()));
	gctoDrawingArea->setToolTip(tr( "Inserts the selected symbol into the drawing area" ) );
	buttonLayout->addWidget(gctoDrawingArea);
	
	DImageButton *addFolderGC = new DImageButton(QPixmap(THEME_DIR+"/icons/addfolder.png" ), 22, m_buttons);
	connect(addFolderGC, SIGNAL(clicked()), m_libraryTree, SLOT(createFolder()));
	addFolderGC->setToolTip(tr( "Adds a folder to the symbol list" ));
	buttonLayout->addWidget(addFolderGC);
	
	m_buttons->setLayout(buttonLayout);
	
	addChild( m_display );
	addChild( m_buttons );
	addChild( m_libraryTree );
}

void KTLibraryWidget::setup()
{
// 	if ( m_libraryDir.exists() )
// 	{
// 		// Parse!
// 		
// 		QStringList files = m_libraryDir.entryList(QStringList() << "*.ktlbr");
// 		
// 		foreach( QString file, files)
// 		{
// 			addFolder( file.left( file.length()-6 ));
// 			KTLibraryParser parser;
// 			
// 			QXmlSimpleReader reader;
// 			reader.setContentHandler(&parser);
// 			reader.setErrorHandler(&parser);
// 			
// 			QFile libFile(m_libraryDir.path() +"/"+ file );
// 			QXmlInputSource xmlsource(&libFile);
// 			
// 			if ( reader.parse(&xmlsource) )
// 			{
// 				foreach(KTGraphicComponent *component, parser.components() )
// 				{
// 					addGraphic(component);
// 				}
// 			}
// 			else
// 			{
// 				dError() << "Error while parse file: " << libFile.fileName();
// 			}
// 		}
// 	}
// 	else
// 	{
// 		m_libraryDir.mkdir(m_libraryDir.path() );
// 	}
// 	
// 	if ( !m_libraryDir.exists(m_libraryDir.path()+"/resources") )
// 	{
// 		m_libraryDir.mkdir(m_libraryDir.path()+"/resources" );
// 	}
}

KTLibraryWidget::~KTLibraryWidget()
{
	DEND;

// 	QList<QTreeWidgetItem *> folders = m_libraryTree->topLevelItems();
// 	QList<QTreeWidgetItem *>::ConstIterator folderIterator = folders.begin();
// 	
// 	while ( folderIterator != folders.end() )
// 	{
// 		QDomDocument doc;
// 		QDomElement root = doc.createElement("Library");
// 		doc.appendChild(root);
// 		
// 		for ( int index = 0; index < (*folderIterator)->childCount(); index++)
// 		{
// 			root.appendChild( m_graphics[(*folderIterator)->child(index) ]->createXML(doc));
// 		}
// 		
// 		QFile custom(CONFIG_DIR+"/libraries/"+(*folderIterator)->text(0)+".ktlbr");
// 		
// 		QDir brushesDir(CONFIG_DIR+"/libraries");
// 		
// 		if ( ! brushesDir.exists() )
// 		{
// 			brushesDir.mkdir(brushesDir.path() );
// 		}
// 		
// 		if ( custom.open(QIODevice::WriteOnly | QIODevice::Text))
// 		{
// 			QTextStream out(&custom);
// 			out << doc.toString();
// 			custom.close();
// 		}
// 		++folderIterator;
// 	}
}

// void KTLibraryWidget::addGraphic(const KTGraphicComponent *graphic)
// {
// 	D_FUNCINFO;
// 	if ( !m_libraryTree->currentFolder() )
// 	{
// 		addFolder( tr("General") );
// 	}
// 	
// 	KTGraphicComponent *copy = new KTGraphicComponent(*graphic);
// 	
// 	m_display->addGraphicComponent( copy);
// 	
// 	QTreeWidgetItem *item = new QTreeWidgetItem(m_libraryTree->currentFolder() );
// 	
// 	if( graphic->componentName().isNull() )
// 	{
// 		item->setText(0, tr("Component #%1").arg(m_childCount++));
// 	}
// 	else
// 	{
// 		item->setText(0, graphic->componentName());
// 	}
// 	
// 	m_graphics.insert(item, copy);
// 	m_libraryTree->setCurrentItem(item);
// }

void KTLibraryWidget::addFolder(const QString &name)
{
	m_libraryTree->createFolder(name);
}

void KTLibraryWidget::drawCurrentItem(QTreeWidgetItem *item, int)
{
	D_FUNCINFO;
	if ( item )
	{
// 		KTGraphicComponent *gc = m_graphics[item];
// 		if ( gc )
// 		{
// 			m_display->addGraphicComponent( gc);
// 		}
// 		else
// 		{
// 			m_libraryTree->setCurrentFolder(item);
// 		}
	}
}

void KTLibraryWidget::emitSelectedComponent()
{
// 	KTGraphicComponent *gc = m_graphics[ m_libraryTree->currentItem() ];
// 	
// 	if ( gc )
// 	{
// 		emit sendCurrentGraphic( gc );
// 	}
}

void KTLibraryWidget::removeCurrentGraphic()
{
// 	DCONFIG->beginGroup("Library");
// 	bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAsk", false));
// 	
// 	if ( ! noAsk )
// 	{
// 		DOptionalDialog dialog(tr("Do you want to remove this component?"),tr("Remove?"), this);
// 		
// 		if( dialog.exec() == QDialog::Rejected )
// 		{
// 			return;
// 		}
// 		
// 		DCONFIG->setValue("RemoveWithoutAsk", dialog.shownAgain());
// 		
// 		DCONFIG->sync();
// 	}
// 	
// 	KTGraphicComponent *gc = m_graphics.take(m_libraryTree->currentItem());
// 	if ( gc )
// 	{
// 		QTreeWidgetItem *item = m_libraryTree->currentItem();
// 		if ( item )
// 		{
// 			delete item;
// 			m_libraryTree->setCurrentItem(m_libraryTree->currentFolder());
// 			m_display->removeGraphic();
// 		}
// 	}
// 	else
// 	{
// 		QTreeWidgetItem *currentFolder = m_libraryTree->currentFolder();
// 		
// 		if ( currentFolder )
// 		{
// 			for(int item = 0; item < currentFolder->childCount(); item++)
// 			{
// 				QTreeWidgetItem *child = currentFolder->child(item);
// 				m_graphics.remove(child);
// 			}
// 			
// 			QString folder = m_libraryDir.path()+"/"+currentFolder->text(0)+".ktlbr";
// 			
// 			QFile::remove(folder);
// 			m_libraryTree->removeCurrentFolder();
// 		}
// 	}
}

void KTLibraryWidget::renameObject( QTreeWidgetItem* item)
{
// 	if ( item )
// 	{
// 		KTGraphicComponent *graphic = m_graphics[item];
// 		
// 		if ( graphic )
// 		{
// 			graphic->setComponentName(item->text(0));
// 		}
// 		else // A Folder
// 		{
// 			foreach( QTreeWidgetItem *folder, m_libraryTree->topLevelItems() )
// 			{
// 				if ( folder != item && folder->text(0) == item->text(0) )
// 				{
// 					// Invalid name
// 					item->setFlags(item->flags() | Qt::ItemIsEditable );
// 					item->setText(0, item->text(0)+QString::number(() % 999) );
// 					m_libraryTree->editItem( item, 0);
// 					break;
// 				}
// 			}
// 		}
// 	}
}

void KTLibraryWidget::importBitmap()
{
	QString image = QFileDialog::getOpenFileName ( this, tr("Import an image..."), QDir::homePath(),  tr("Images")+" (*.png *.xpm *.jpg)" );
	
	addBitmap( image );
}

void KTLibraryWidget::addBitmap(const QString &bitmap)
{
// 	QPixmap toImport(bitmap);
// 	
// 	if ( ! toImport.isNull() )
// 	{
// 		KTGraphicComponent *imageComponent = new KTGraphicComponent;
// 		
// 		QFile file(bitmap);
// 		QFileInfo finfo(file);
// 		
// 		imageComponent->setComponentName( finfo.baseName() );
// 		
// 		QPainterPath path;
// 		path.addRect(toImport.rect());
// 		imageComponent->addGraphic(path , Qt::NoPen, Qt::NoBrush,toImport);
// 		
// 		if ( !file.copy(m_libraryDir.path()+"/resources/"+ imageComponent->graphics()[0]->pixmapHash() ) )
// 		{
// 			emit sendToOSD(tr("Cannot import ")+finfo.fileName(), 2);
// 			return;
// 		}
// 		
// 		addGraphic( imageComponent );
// 	}
}

void KTLibraryWidget::libraryRequest(KTProjectRequest *request)
{
	
}


