/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#define RETURN_IF_NOT_LIBRARY if( !k->library ) return;

/**
 * This class defines the Library widget.
 * Here is where events around the Library widget are handled.
 * @author David Cuadrado <krawek@toonka.com>
*/

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

KTLibraryWidget::KTLibraryWidget(QWidget *parent) : KTModuleWidgetBase(parent), k(new Private)
{
	#ifdef K_DEBUG
		KINIT;
	#endif
	k->childCount = 0;
	
	setWindowIcon(QPixmap(THEME_DIR+"/icons/library.png"));
	setWindowTitle(tr("&Library"));
	
	
	k->libraryDir = QDir(CONFIG_DIR+"/libraries");
	
	k->display = new KTItemPreview(this);
	
	k->libraryTree = new KTGCTable(this);

	connect(k->libraryTree, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(previewItem(QTreeWidgetItem *,
													int)));
	connect(k->libraryTree, SIGNAL(itemRenamed( QTreeWidgetItem* )), this, SLOT(renameObject( QTreeWidgetItem* )));
	
	QGroupBox *buttons = new QGroupBox(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(0);
	
	KImageButton *addGC = new KImageButton(QPixmap(THEME_DIR+"/icons/plussign.png" ), 22, buttons);
	connect(addGC, SIGNAL(clicked()), this, SIGNAL(requestCurrentGraphic()));
	
	buttonLayout->addWidget(addGC);
	addGC->setToolTip(tr( "Add the current graphic to Library" ));
	
	KImageButton *delGC = new KImageButton(QPixmap(THEME_DIR+"/icons/minussign.png" ), 22, buttons);
	connect(delGC, SIGNAL(clicked()), this, SLOT(removeCurrentGraphic()));
	
	delGC->setToolTip(tr( "Remove the selected Symbol from Library" ));
	buttonLayout->addWidget(delGC);
	
	KImageButton *gctoDrawingArea = new KImageButton(QPixmap(THEME_DIR+"/icons/insert_cg.png" ), 22, buttons);
	connect(gctoDrawingArea, SIGNAL(clicked()), this, SLOT(emitSelectedComponent()));
	gctoDrawingArea->setToolTip(tr( "Inserts the selected symbol into the drawing area" ) );
	buttonLayout->addWidget(gctoDrawingArea);
	
	KImageButton *addFolderGC = new KImageButton(QPixmap(THEME_DIR+"/icons/addfolder.png" ), 22, buttons);
	connect(addFolderGC, SIGNAL(clicked()), k->libraryTree, SLOT(createFolder()));
	addFolderGC->setToolTip(tr( "Adds a folder to the symbol list" ));
	buttonLayout->addWidget(addFolderGC);
	
	buttons->setLayout(buttonLayout);
	
	addChild( k->display );
	addChild( buttons );
	addChild( k->libraryTree );
}


KTLibraryWidget::~KTLibraryWidget()
{
	#ifdef K_DEBUG
		KEND;
	#endif
	delete k;
}

void KTLibraryWidget::setLibrary(const KTLibrary *library)
{
	k->library = library;
}

void KTLibraryWidget::addFolder(const QString &name)
{
	k->libraryTree->createFolder(name);
}

void KTLibraryWidget::previewItem(QTreeWidgetItem *item, int)
{
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	
	RETURN_IF_NOT_LIBRARY;
	
	if ( item )
	{
		KTLibraryObject *object = k->library->findObject(item->text(0));
		
		if ( !object )
		{
			#ifdef K_DEBUG
				kDebug("library") << "Cannot find the object";
			#endif
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
					k->display->render( qvariant_cast<QGraphicsItem *>(object->data()));
					
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
				KAudioPlayer::instance()->setCurrentPlayer(k->currentPlayerId);
				KAudioPlayer::instance()->stop();
				
				k->currentPlayerId = KAudioPlayer::instance()->load(object->dataPath());
				KAudioPlayer::instance()->play(0);
			}
			break;
			default:
			{
				#ifdef K_DEBUG
					kDebug("library") << "Unknown symbol id: " << object->type();
				#endif
			}
			break;
		}
	}
}

void KTLibraryWidget::emitSelectedComponent()
{
	if ( !k->libraryTree->currentItem() ) 
	{	
		return;
	}
	
	QString symKey = k->libraryTree->currentItem()->text(0);
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::AddSymbolToProject, symKey,
					KTLibraryObject::Type(k->libraryTree->currentItem()->data(0, 3216).toInt()), 0, 
					k->currentFrame.scene, k->currentFrame.layer, k->currentFrame.frame);
	
	emit requestTriggered( &request);
}

void KTLibraryWidget::removeCurrentGraphic()
{
	if ( !k->libraryTree->currentItem() ) 
	{
		return;
	}

	QString symKey = k->libraryTree->currentItem()->text(0);
	
	KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Remove, symKey,
					KTLibraryObject::Type(k->libraryTree->currentItem()->data(0, 3216).toInt()), 0 );
	
	emit requestTriggered( &request );
}

void KTLibraryWidget::renameObject( QTreeWidgetItem* item)
{
// 	if ( item )
// 	{
// 		KTGraphicComponent *graphic = k->graphics[item];
// 		
// 		if ( graphic )
// 		{
// 			graphic->setComponentName(item->text(0));
// 		}
// 		else // A Folder
// 		{
// 			foreach( QTreeWidgetItem *folder, k->libraryTree->topLevelItems() )
// 			{
// 				if ( folder != item && folder->text(0) == item->text(0) )
// 				{
// 					// Invalid name
// 					item->setFlags(item->flags() | Qt::ItemIsEditable );
// 					item->setText(0, item->text(0)+QString::number(() % 999) );
// 					k->libraryTree->editItem( item, 0);
// 					break;
// 				}
// 			}
// 		}
// 	}
}

void KTLibraryWidget::importBitmap()
{
	QString image = QFileDialog::getOpenFileName ( this, tr("Import an image..."), QDir::homePath(),  
							tr("Images") + " (*.png *.xpm *.jpg)" );
	
	if( image.isEmpty() ) 
	{
		return;
	}	
	
	QFile f(image);
	QFileInfo finfo(f);
	
	QString symName = finfo.baseName();
	
	if( f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		f.close();
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
								KTLibraryObject::Image, data);
		
		emit requestTriggered(&request);
	}
	else
	{
		KOsd::self()->display(tr("Cannot open file: %1").arg(image));
	}
}

void KTLibraryWidget::importSound()
{
	QString sound = QFileDialog::getOpenFileName ( this, tr("Import an audio file..."), QDir::homePath(),
							tr("Sound file")+" (*.ogg *.wav *.mp3)" );
	
	if( sound.isEmpty() ) 
	{
		return;
	}
	
	QFile f(sound);
	
	QFileInfo finfo(f);
	QString symName = finfo.baseName();
	
	if( f.open(QIODevice::ReadOnly))
	{
		QByteArray data = f.readAll();
		f.close();
		
		KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,
										KTLibraryObject::Sound, data);
		
		emit requestTriggered(&request);
	}
	else
	{
		KOsd::self()->display(tr("Cannot open file: %1").arg(sound));
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
			KTLibraryObject *obj = k->library->findObject(key);
			
			QTreeWidgetItem *item = new QTreeWidgetItem(k->libraryTree);
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
			QTreeWidgetItemIterator it(k->libraryTree);
			while( (*it) )
			{
						if( key == (*it)->text(0) )
						{
								delete (*it);
								break;
						}
						++it;
			}
			
			QList<QTreeWidgetItem *> selectedGroup = k->libraryTree->selectedItems();
			if( !selectedGroup.isEmpty() )
			{
				k->display->render( qvariant_cast<QGraphicsItem *>(k->library->findObject(
										selectedGroup[0]->text(0))->data() ));
			}
			else
			{
				k->display->render(0);
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
		k->currentFrame.frame = response->frameIndex();
		k->currentFrame.layer = response->layerIndex();
		k->currentFrame.scene = response->sceneIndex();
	}
}
