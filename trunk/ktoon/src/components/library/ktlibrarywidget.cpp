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

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kgui/koptionaldialog.h>
#include <kcore/kconfig.h>

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

#include <kgui/kosd.h>
#include <ksound/kaudioplayer.h>

#define RETURN_IF_NOT_LIBRARY if( !d->library ) return;

struct KTLibraryWidget::Private
{
	Private() : library(0)
	{
		currentFrame.frame = 0;
		currentFrame.layer = 0;
		currentFrame.scene = 0;
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

KTLibraryWidget::KTLibraryWidget(QWidget *parent) : KTModuleWidgetBase(parent), d(new Private)
{
	DINIT;
	
	d->childCount = 0;
	
	setWindowIcon(QPixmap(THEME_DIR+"/icons/library.png"));
	setWindowTitle(tr("&Library"));
	
	
	d->libraryDir = QDir(CONFIG_DIR+"/libraries");
	
	d->display = new KTItemPreview(this);
	
	d->libraryTree = new KTGCTable(this);

	connect(d->libraryTree, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(previewItem(QTreeWidgetItem *, int)));
	connect(d->libraryTree, SIGNAL(itemRenamed( QTreeWidgetItem* )), this, SLOT(renameObject( QTreeWidgetItem* )));
	
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


KTLibraryWidget::~KTLibraryWidget()
{
	DEND;
	delete d;
}

void KTLibraryWidget::setLibrary(const KTLibrary *library)
{
	d->library = library;
}

void KTLibraryWidget::addFolder(const QString &name)
{
	d->libraryTree->createFolder(name);
}

void KTLibraryWidget::previewItem(QTreeWidgetItem *item, int)
{
	D_FUNCINFO;
	
	RETURN_IF_NOT_LIBRARY;
	
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
				
				d->currentPlayerId = DAudioPlayer::instance()->load(object->dataPath());
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
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::AddSymbolToProject, symKey, KTLibraryObject::Type(d->libraryTree->currentItem()->data(0, 3216).toInt()), 0, d->currentFrame.scene, d->currentFrame.layer, d->currentFrame.frame);
	
	emit requestTriggered( &request);
}

void KTLibraryWidget::removeCurrentGraphic()
{
	if ( !d->libraryTree->currentItem() ) return;
	QString symKey = d->libraryTree->currentItem()->text(0);
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Remove, symKey, KTLibraryObject::Type(d->libraryTree->currentItem()->data(0, 3216).toInt()), 0 );
	
	emit requestTriggered( &request );
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
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName, KTLibraryObject::Image, data);
		
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
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName, KTLibraryObject::Sound, data);
		
		emit requestTriggered(&request);
	}
	else
	{
		DOsd::self()->display(tr("Cannot open file: %1").arg(sound));
	}
}


void KTLibraryWidget::libraryResponse(KTLibraryResponse *response)
{
	RETURN_IF_NOT_LIBRARY;
	
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			QString key = response->arg().toString();
			KTLibraryObject *obj = d->library->findObject(key);
			
			QTreeWidgetItem *item = new QTreeWidgetItem(d->libraryTree);
			item->setText(0, key);
			
			item->setData(0, 3216, obj->type());
			
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


