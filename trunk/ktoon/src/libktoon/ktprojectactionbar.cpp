/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktprojectactionbar.h"

#include <QVBoxLayout>
#include <QVariant>

#include <ktglobal.h>
#include <dcore/ddebug.h>

#include <dgui/dseparator.h>
#include <dcore/dconfig.h>
#include <dgui/doptionaldialog.h>
#include <dgui/dimagebutton.h>

struct KTProjectActionBar::Private
{
	Private(Qt::Orientation orientation) : orientation(orientation), isAnimated(true) {}
	Qt::Orientation orientation;
	
	int fixedSize;
	QButtonGroup actions;
	
	QBoxLayout *buttonLayout;
	
	bool isAnimated;
};

KTProjectActionBar::KTProjectActionBar(Actions actions, Qt::Orientation orientation, QWidget *parent) : QWidget(parent ), d(new Private(orientation) )
{
	connect(&d->actions, SIGNAL(buttonClicked(int)), this, SLOT(emitActionSelected(int)));
	
	setup(actions);
	
	setFixedSize( 22 );
}


KTProjectActionBar::~KTProjectActionBar()
{
}


void KTProjectActionBar::setFixedSize(int s)
{
	d->fixedSize = s;
	
// 	switch(d->orientation )
// 	{
// 		case Qt::Horizontal:
// 		{
// 			setMaximumHeight( s );
// 		}
// 		break;
// 		case Qt::Vertical:
// 		{
// 			setMaximumWidth( s );
// 		}
// 		break;
// 	}
	
// 	foreach( QAbstractButton *button, d->actions.buttons() )
// 	{
// 		button->setIconSize(QSize(d->fixedSize, d->fixedSize));
// 	}
}


void KTProjectActionBar::setup(Actions actions)
{
	QBoxLayout *mainLayout = 0;
	
	switch(d->orientation)
	{
		case Qt::Vertical:
		{
			mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
			d->buttonLayout = new QBoxLayout(QBoxLayout::TopToBottom);

		}
		break;
		case Qt::Horizontal:
		{
			mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
			d->buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
			
		}
		break;
	}
	
	mainLayout->setSpacing( 0);
	mainLayout->setMargin( 1);
	
	d->buttonLayout->setSpacing(1  );
	d->buttonLayout->setMargin( 1);
	
	d->buttonLayout->addStretch();
	
	int size = 16;
	
	if ( actions & InsertFrame )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/add_frame.png" ), size);
		button->setToolTip(tr("Insert frame") );
		
		button->setShortcut(QKeySequence(Qt::Key_Plus));
		
		d->actions.addButton(button, InsertFrame);
		
		d->buttonLayout->addWidget( button );
		
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & RemoveFrame )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/remove_frame.png"), size);
		button->setToolTip(tr("Remove the frame"));
		
		d->actions.addButton(button, RemoveFrame);
		
		button->setShortcut(QKeySequence(Qt::Key_Minus));
		
		d->buttonLayout->addWidget( button );
		
		button->setAnimated(d->isAnimated);
	}
	 
	if ( actions & MoveFrameUp )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_frame_up.png"), size);
		button->setToolTip( tr("Move frame up"));
		
		d->actions.addButton(button, MoveFrameUp);
		
		d->buttonLayout->addWidget( button );
		
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & MoveFrameDown )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_frame_down.png"), size);
		button->setToolTip(tr("Move frame down") );
		
		d->actions.addButton( button, MoveFrameDown);
		
		d->buttonLayout->addWidget( button );
		
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & LockFrame )
	{
		DImageButton *button = new DImageButton(QIcon(HOME_DIR+"/themes/default/icons/kilit_pic.png"), size);
		button->setToolTip(tr("Lock frame") );
		
		d->actions.addButton( button, LockFrame);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & InsertLayer )
	{
		DImageButton *button = new DImageButton(QIcon(HOME_DIR+"/themes/default/icons/add_layer.png"), size);
		button->setToolTip(tr("Insert a layer"));
		
		d->actions.addButton(button, InsertLayer);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	 
	if ( actions & RemoveLayer )
	{
		DImageButton *button = new DImageButton(QIcon( HOME_DIR+"/themes/default/icons/remove_layer.png"), size);
		button->setToolTip(tr("Remove the layer"));
		
		d->actions.addButton(button, RemoveLayer);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
		
	}
	 
	if ( actions & MoveLayerUp )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_up.png" ), size);
		
		button->setToolTip(tr("Move layer up")  );
		
		d->actions.addButton(button, MoveLayerUp);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(true);
	}
	 
	if ( actions & MoveLayerDown )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_down.png" ), size);
		
		button->setToolTip( tr("Move layer down"));
		
		d->actions.addButton(button, MoveLayerDown);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & LockLayer )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_layer_down.png" ), 22);
		button->setToolTip(tr("Lock layer") );
		
		d->actions.addButton( button, LockLayer );
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & InsertScene )
	{
		DImageButton *button = new DImageButton(QIcon( HOME_DIR+"/themes/default/icons/add_scene.png"), size);  // TODO
		
		button->setToolTip(tr("Insert a scene"));
		
		d->actions.addButton(button, InsertScene);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	 
	if ( actions & RemoveScene )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/remove_scene.png" ), size);  // TODO
		
		button->setToolTip(tr("Remove the scene"));
		d->actions.addButton(button, RemoveScene);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	 
	if ( actions & MoveSceneUp )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/move_scene_up.png" ), size);
		
		button->setToolTip(tr("Move scene up")  );
		
		d->actions.addButton(button, MoveSceneUp);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	 
	if ( actions & MoveSceneDown )
	{
		DImageButton *button = new DImageButton(QIcon(THEME_DIR+"/icons/move_scene_down.png" ), size);
		
		button->setToolTip( tr("Move scene down"));
		
		d->actions.addButton(button, MoveSceneDown);
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	
	if ( actions & LockScene )
	{
		DImageButton *button = new DImageButton(QIcon( THEME_DIR+"/icons/lock_scene.png"), size);
		button->setToolTip(tr("Lock scene") );
		
		d->actions.addButton( button, LockScene );
		
		d->buttonLayout->addWidget( button );
		button->setAnimated(d->isAnimated);
	}
	
	d->buttonLayout->addStretch();
	
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
	mainLayout->addLayout( d->buttonLayout );
	mainLayout->addWidget( new DSeparator(Qt::Horizontal));
	
}


void KTProjectActionBar::insertSeparator(int position)
{
	Qt::Orientation sepOrientation = Qt::Vertical;
	
	switch(d->orientation)
	{
		case Qt::Vertical:
		{
			sepOrientation = Qt::Horizontal;
		}
		break;
		case Qt::Horizontal:
		{
			sepOrientation = Qt::Vertical;
		}
		break;
	}
	
	d->buttonLayout->insertWidget(position+1, new DSeparator(sepOrientation), 1, Qt::AlignCenter);
}


DImageButton *KTProjectActionBar::button(Action action)
{
	return qobject_cast<DImageButton *>(d->actions.button(action));
}

void KTProjectActionBar::emitActionSelected(int action)
{
	switch(action)
	{
		case RemoveFrame:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskFrame", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this frame?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskFrame", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
		case RemoveLayer:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskLayer", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this layer?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskLayer", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
		case RemoveScene:
		{
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskScene", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this scene?"), tr("Remove?"), this);
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskScene", dialog.shownAgain());
				DCONFIG->sync();
			}
		}
		break;
	}
	
	emit actionSelected( action );
}

