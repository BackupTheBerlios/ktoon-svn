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
#include "ktapplication.h"
#include "ktdebug.h"
#include "ktoptionaldialog.h"

#include "ktconfig.h"

#include <QGroupBox>

KTLibraryWidget::KTLibraryWidget(QWidget *parent) : KTModuleWidgetBase(parent)
{
	setCaption(tr("Library"));
	
	m_displayPath = new KTDisplayPath(this);
	
	m_libraryTree = new KTGCTable(this);

	connect(m_libraryTree, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(drawCurrentItem(QTreeWidgetItem *, int)));
	
	m_libraryTree->createFolder( tr("General") );
	
	QGroupBox *m_buttons = new QGroupBox(this);
	QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttons);
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(0);
	
	KTImageButton *addGC = new KTImageButton(QPixmap(KTOON_THEME_DIR+"/icons/plussign.png" ), 22, m_buttons);
	connect(addGC, SIGNAL(clicked()), this, SIGNAL(requestCurrentGraphic()));
	
	buttonLayout->addWidget(addGC);
	addGC->setToolTip(tr( "Add the current graphic to Library" ));
	
	KTImageButton *delGC = new KTImageButton(QPixmap(KTOON_THEME_DIR+"/icons/minussign.png" ), 22, m_buttons);
	connect(delGC, SIGNAL(clicked()), this, SLOT(removeCurrentGraphic()));
	
	delGC->setToolTip(tr( "Remove the selected Symbol from Library" ));
	buttonLayout->addWidget(delGC);
	
	KTImageButton *gctoDrawingArea = new KTImageButton(QPixmap(KTOON_THEME_DIR+"/icons/insert_cg.png" ), 22, m_buttons);
	connect(gctoDrawingArea, SIGNAL(clicked()), this, SLOT(emitSelectedComponent()));
	gctoDrawingArea->setToolTip(tr( "Inserts the selected symbol into the drawing area" ) );
	buttonLayout->addWidget(gctoDrawingArea);
	
	KTImageButton *addFolderGC = new KTImageButton(QPixmap(KTOON_THEME_DIR+"/icons/addfolder.png" ), 22, m_buttons);
	connect(addFolderGC, SIGNAL(clicked()), m_libraryTree, SLOT(createFolder()));
	addFolderGC->setToolTip(tr( "Adds a folder to the symbol list" ));
	buttonLayout->addWidget(addFolderGC);
	
	m_buttons->setLayout(buttonLayout);
	
	addChild( m_displayPath );
	addChild( m_buttons );
	addChild( m_libraryTree );
}

KTLibraryWidget::~KTLibraryWidget()
{
}

void KTLibraryWidget::addGraphic(const AGraphicComponent *graphic)
{
	KT_FUNCINFO;
	if ( m_libraryTree->currentFolder() )
	{
		AGraphicComponent *copy = new AGraphicComponent(*graphic);
		
		m_displayPath->setBrush(copy->brush());
		m_displayPath->setPen(copy->pen());
		m_displayPath->setPath( copy->path() );
		
		QTreeWidgetItem *item = new QTreeWidgetItem(m_libraryTree->currentFolder() );
		item->setText(0, tr("Component #"));
		
// 		m_libraryTree->openPersistentEditor(item, 0);
		
		m_graphics.insert(item, copy);
	}
}

void KTLibraryWidget::drawCurrentItem(QTreeWidgetItem *item, int)
{
	KT_FUNCINFO;
	if ( item )
	{
		AGraphicComponent *gc = m_graphics[item];
		if ( gc )
		{
			m_displayPath->setBrush(gc->brush());
			m_displayPath->setPen(gc->pen());
			m_displayPath->setPath( gc->path() );
		}
		else
		{
			m_libraryTree->setCurrentFolder(item);
		}
	}
}

void KTLibraryWidget::emitSelectedComponent()
{
	AGraphicComponent *gc = m_graphics[ m_libraryTree->currentItem() ];
	
	if ( gc )
	{
		emit sendCurrentGraphic( gc );
	}
}

void KTLibraryWidget::removeCurrentGraphic()
{
	KTCONFIG->beginGroup("Library");
	bool noAsk = qvariant_cast<bool>(KTCONFIG->value("RemoveWithoutAsk", false));
	
	if ( ! noAsk )
	{
		// TODO: preguntar si quiere borrar
		KTOptionalDialog dialog(tr("Do you want remove this component?"),tr("Remove?"), this);
		
		if( dialog.exec() == QDialog::Rejected )
		{
			return;
		}
		
		KTCONFIG->setValue("RemoveWithoutAsk", dialog.checked());
		
		KTCONFIG->sync();
	}
	
	AGraphicComponent *gc = m_graphics.take(m_libraryTree->currentItem());
	if ( gc )
	{
		QTreeWidgetItem *item = m_libraryTree->currentItem();
		if ( item )
		{
			delete item;
			m_libraryTree->setCurrentItem(m_libraryTree->currentFolder());
			m_displayPath->setPath( QPainterPath() );
		}
	}
	else
	{
		QTreeWidgetItem *currentFolder = m_libraryTree->currentFolder();
		
		if ( currentFolder )
		{
			for(int item = 0; item < currentFolder->childCount(); item++)
			{
				QTreeWidgetItem *child = currentFolder->child(item);
				m_graphics.remove(child);
			}
			
			m_libraryTree->removeCurrentFolder();
		}
	}
}


