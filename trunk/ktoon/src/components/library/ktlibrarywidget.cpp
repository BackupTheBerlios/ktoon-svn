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

#include <dcore/dglobal.h>
#include <dcore/ddebug.h>
#include <dgui/doptionaldialog.h>
#include <dcore/dconfig.h>

#include <QGroupBox>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QLabel>

#include <ktrequestbuilder.h>

#include <cstdlib>
#include <ctime>
#include <QTreeWidgetItemIterator>

#include "ktlibrary.h"
#include "ktlibraryobject.h"
#include "ktsymboleditor.h"

#include <dgui/dosd.h>
#include <dsound/daudioplayer.h>

struct KTLibraryWidget::Private
{
	Private()
	{
		currentFrame.frame = -1;
	}
	
	const KTLibrary *library;
	KTItemPreview *display;
	KTGCTable *libraryTree;
	int childCount;
	QDir libraryDir;
	
	int currentPlayerId;
	
	struct Frame
	{
		int scene;
		int layer;
		int frame;
	} currentFrame;
};

KTLibraryWidget::KTLibraryWidget(const KTLibrary *library,QWidget *parent) : KTModuleWidgetBase(parent), d(new Private)
{
	DINIT;
	
	d->library = library;
	d->childCount = 0;
	
	setWindowIcon(QPixmap(THEME_DIR+"/icons/library.png"));
	setWindowTitle(tr("&Library"));
	
	
	d->libraryDir = QDir(CONFIG_DIR+"/libraries");
	
	d->display = new KTItemPreview(this);
	
	d->libraryTree = new KTGCTable(this);

	connect(d->libraryTree, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(previewItem(QTreeWidgetItem *, int)));
	connect(d->libraryTree, SIGNAL(itemRenamed( QTreeWidgetItem* )), this, SLOT(renameObject( QTreeWidgetItem* )));
	
	
	setup();
	
	QGroupBox *buttons = new QGroupBox(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(0);
	
	DImageButton *addGC = new DImageButton(QPixmap(THEME_DIR+"/icons/plussign.png" ), 22, buttons);
	connect(addGC, SIGNAL(clicked()), this, SIGNAL(requestCurrentGraphic()));
	
	buttonLayout->addWidget(addGC);
	addGC->setToolTip(tr( "Add the current graphic to Library" ));
	
	DImageButton *delGC = new DImageButton(QPixmap(THEME_DIR+"/icons/minussign.png" ), 22, buttons);
	connect(delGC, SIGNAL(clicked()), this, SLOT(removeCurrentGraphic()));
	
	delGC->setToolTip(tr( "Remove the selected Symbol from Library" ));
	buttonLayout->addWidget(delGC);
	
	DImageButton *gctoDrawingArea = new DImageButton(QPixmap(THEME_DIR+"/icons/insert_cg.png" ), 22, buttons);
	connect(gctoDrawingArea, SIGNAL(clicked()), this, SLOT(emitSelectedComponent()));
	gctoDrawingArea->setToolTip(tr( "Inserts the selected symbol into the drawing area" ) );
	buttonLayout->addWidget(gctoDrawingArea);
	
	DImageButton *addFolderGC = new DImageButton(QPixmap(THEME_DIR+"/icons/addfolder.png" ), 22, buttons);
	connect(addFolderGC, SIGNAL(clicked()), d->libraryTree, SLOT(createFolder()));
	addFolderGC->setToolTip(tr( "Adds a folder to the symbol list" ));
	buttonLayout->addWidget(addFolderGC);
	
	buttons->setLayout(buttonLayout);
	
	addChild( d->display );
	addChild( buttons );
	addChild( d->libraryTree );
}

void KTLibraryWidget::setup()
{
// 	if ( d->libraryDir.exists() )
// 	{
// 		// Parse!
// 		
// 		QStringList files = d->libraryDir.entryList(QStringList() << "*.ktlbr");
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
// 			QFile libFile(d->libraryDir.path() +"/"+ file );
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
// 		d->libraryDir.mkdir(d->libraryDir.path() );
// 	}
// 	
// 	if ( !d->libraryDir.exists(d->libraryDir.path()+"/resources") )
// 	{
// 		d->libraryDir.mkdir(d->libraryDir.path()+"/resources" );
// 	}
}

KTLibraryWidget::~KTLibraryWidget()
{
	DEND;

// 	QList<QTreeWidgetItem *> folders = d->libraryTree->topLevelItems();
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
// 			root.appendChild( d->graphics[(*folderIterator)->child(index) ]->createXML(doc));
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
	
	delete d;
}

// void KTLibraryWidget::addGraphic(const KTGraphicComponent *graphic)
// {
// 	D_FUNCINFO;
// 	if ( !d->libraryTree->currentFolder() )
// 	{
// 		addFolder( tr("General") );
// 	}
// 	
// 	KTGraphicComponent *copy = new KTGraphicComponent(*graphic);
// 	
// 	d->display->addGraphicComponent( copy);
// 	
// 	QTreeWidgetItem *item = new QTreeWidgetItem(d->libraryTree->currentFolder() );
// 	
// 	if( graphic->componentName().isNull() )
// 	{
// 		item->setText(0, tr("Component #%1").arg(d->childCount++));
// 	}
// 	else
// 	{
// 		item->setText(0, graphic->componentName());
// 	}
// 	
// 	d->graphics.insert(item, copy);
// 	d->libraryTree->setCurrentItem(item);
// }

void KTLibraryWidget::addFolder(const QString &name)
{
	d->libraryTree->createFolder(name);
}

void KTLibraryWidget::previewItem(QTreeWidgetItem *item, int)
{
	D_FUNCINFO;
	if ( item )
	{
		KTLibraryObject *object = d->library->findObject(item->text(0));
		
		if ( !object )
		{
			dDebug("library") << "Cannot find the object";
			return;
		}
		
		switch (object->type() )
		{
			case KTLibraryObject::Svg:
			case KTLibraryObject::Image:
			case KTLibraryObject::Item:
			{
				if ( object->data().canConvert<QGraphicsItem *>() )
				{
					d->display->render( qvariant_cast<QGraphicsItem *>(object->data()));
					
#if 0 // test
					KTSymbolEditor *editor = new KTSymbolEditor;
					editor->setSymbol(object);
					emit postPage(editor);
#endif
				}
			}
			break;
			case KTLibraryObject::Sound:
			{
				DAudioPlayer::instance()->setCurrentPlayer(d->currentPlayerId);
				DAudioPlayer::instance()->stop();
				
				d->currentPlayerId = DAudioPlayer::instance()->load(object->data().toString());
				DAudioPlayer::instance()->play(0);
			}
			break;
			default:
			{
				dDebug("library") << "Unknown symbol id: " << object->type();
			}
			break;
		}
	}
}

void KTLibraryWidget::emitSelectedComponent()
{
	if ( !d->libraryTree->currentItem() ) return;
	
	QString symKey = d->libraryTree->currentItem()->text(0);
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::AddSymbolToProject, symKey, 0, 0, d->currentFrame.scene, d->currentFrame.layer, d->currentFrame.frame);
	
	emit requestTriggered( &request);
}

void KTLibraryWidget::removeCurrentGraphic()
{
	if ( !d->libraryTree->currentItem() ) return;
	QString symKey = d->libraryTree->currentItem()->text(0);
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Remove, symKey, 0, 0 );
	
	emit requestTriggered( &request );
	
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
// 	KTGraphicComponent *gc = d->graphics.take(d->libraryTree->currentItem());
// 	if ( gc )
// 	{
// 		QTreeWidgetItem *item = d->libraryTree->currentItem();
// 		if ( item )
// 		{
// 			delete item;
// 			d->libraryTree->setCurrentItem(d->libraryTree->currentFolder());
// 			d->display->removeGraphic();
// 		}
// 	}
// 	else
// 	{
// 		QTreeWidgetItem *currentFolder = d->libraryTree->currentFolder();
// 		
// 		if ( currentFolder )
// 		{
// 			for(int item = 0; item < currentFolder->childCount(); item++)
// 			{
// 				QTreeWidgetItem *child = currentFolder->child(item);
// 				d->graphics.remove(child);
// 			}
// 			
// 			QString folder = d->libraryDir.path()+"/"+currentFolder->text(0)+".ktlbr";
// 			
// 			QFile::remove(folder);
// 			d->libraryTree->removeCurrentFolder();
// 		}
// 	}
}

void KTLibraryWidget::renameObject( QTreeWidgetItem* item)
{
// 	if ( item )
// 	{
// 		KTGraphicComponent *graphic = d->graphics[item];
// 		
// 		if ( graphic )
// 		{
// 			graphic->setComponentName(item->text(0));
// 		}
// 		else // A Folder
// 		{
// 			foreach( QTreeWidgetItem *folder, d->libraryTree->topLevelItems() )
// 			{
// 				if ( folder != item && folder->text(0) == item->text(0) )
// 				{
// 					// Invalid name
// 					item->setFlags(item->flags() | Qt::ItemIsEditable );
// 					item->setText(0, item->text(0)+QString::number(() % 999) );
// 					d->libraryTree->editItem( item, 0);
// 					break;
// 				}
// 			}
// 		}
// 	}
}

void KTLibraryWidget::importBitmap()
{
	QString image = QFileDialog::getOpenFileName ( this, tr("Import an image..."), QDir::homePath(),  tr("Images")+" (*.png *.xpm *.jpg)" );
	
	if( image.isEmpty() ) return;
	
	QFile f(image);
	QFileInfo finfo(f);
	
	QString symName = finfo.baseName();
	
	if( f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		f.close();
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName, data, KTLibraryObject::Image);
		
		emit requestTriggered(&request);
	}
	else
	{
		DOsd::self()->display(tr("Cannot open file: %1").arg(image));
	}
}

void KTLibraryWidget::importSound()
{
	QString sound = QFileDialog::getOpenFileName ( this, tr("Import an audio file..."), QDir::homePath(),  tr("Sound file")+" (*.ogg *.wav *.mp3)" );
	
	if( sound.isEmpty() ) return;
	
	QFile f(sound);
	
	QFileInfo finfo(f);
	QString symName = finfo.baseName();
	
	if( f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		f.close();
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName, data, KTLibraryObject::Sound);
		
		emit requestTriggered(&request);
	}
	else
	{
		DOsd::self()->display(tr("Cannot open file: %1").arg(sound));
	}
}


void KTLibraryWidget::libraryResponse(KTLibraryResponse *response)
{
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			QString key = response->arg().toString();
			KTLibraryObject *obj = d->library->findObject(key);
			
			QTreeWidgetItem *item = new QTreeWidgetItem(d->libraryTree);
			item->setText(0, key);
			
			if( obj )
			{
				switch(obj->type())
				{
					case KTLibraryObject::Item:
					{
						item->setIcon(1, QIcon(THEME_DIR+"/icons/shape_brush.png"));
					}
					break;
					case KTLibraryObject::Sound:
					{
						item->setIcon(1, QIcon(THEME_DIR+"/icons/sound_widget.png"));
					}
					break;
					
				}
			}
		}
		break;
		case KTProjectRequest::Remove:
		{
			QString key = response->arg().toString();
			QTreeWidgetItemIterator it(d->libraryTree);
			while( (*it) )
			{
						if( key == (*it)->text(0) )
						{
								delete (*it);
								break;
						}
						++it;
			}
			
			QList<QTreeWidgetItem *> selecteds = d->libraryTree->selectedItems();
			if( !selecteds.isEmpty() )
			{
				d->display->render( qvariant_cast<QGraphicsItem *>(d->library->findObject( selecteds[0]->text(0))->data() ));
			}
			else
			{
				d->display->render(0);
			}
			
		}
		break;
		default:
		{
			qWarning("ktlibrarywidget.cpp IMPLEMENT ME");
		}
		break;
	}
}

void KTLibraryWidget::frameResponse(KTFrameResponse *response)
{
	if( response->action() == KTProjectRequest::Select )
	{
		d->currentFrame.frame = response->frameIndex();
		d->currentFrame.layer = response->layerIndex();
		d->currentFrame.scene = response->sceneIndex();
	}
}


