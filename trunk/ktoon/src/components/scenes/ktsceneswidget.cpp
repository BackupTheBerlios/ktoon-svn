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

#include "ktsceneswidget.h"

#include "dglobal.h"
#include "ddebug.h"
#include "dimagebutton.h"
#include "doptionaldialog.h"
#include "dconfig.h"

#include <QToolTip>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>

#include "ktsceneevent.h"


//--------------- CONSTRUCTOR --------------------

KTScenesWidget::KTScenesWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTScenesWidget")
{
	DINIT;
	
	setCaption( tr( "Scenes manager" ) );
	
	setupButtons();
	setupTableScenes();
}

KTScenesWidget::~KTScenesWidget()
{
	DEND;
}

void KTScenesWidget::setupButtons()
{
	m_buttonsPanel = new QGroupBox(this);
	
	QHBoxLayout *layout = new QHBoxLayout(m_buttonsPanel);
	layout->setMargin(0);

	DImageButton *insertButton = new DImageButton(QPixmap(HOME_DIR+"/themes/default/icons/plussign.png" ) , 22, m_buttonsPanel);
	layout->addWidget(insertButton);
	insertButton->setToolTip(tr("Insert scene"));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(emitRequestInsertScene()));
	
	DImageButton *removeButton = new DImageButton(QPixmap(HOME_DIR+"/themes/default/icons/minussign.png" ) , 22, m_buttonsPanel);
	layout->addWidget(removeButton);
	removeButton->setToolTip(tr("Remove scene"));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(emitRequestRemoveScene()));

	addChild(m_buttonsPanel);
}

void KTScenesWidget::setupTableScenes()
{
	m_tableScenes = new KTScenesList(this);
	
	DTreeWidgetSearchLine *searcher = new DTreeWidgetSearchLine(this, m_tableScenes);
	searcher->setClickMessage( tr("Filter here..."));
	
	addChild(searcher);
	
	addChild( m_tableScenes);
	connect(m_tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(m_tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));
	
}

void KTScenesWidget::insertScene(const QString &name, bool addedToEnd)
{
	D_FUNCINFO;
	m_tableScenes->addScene(name);
	emit sendToStatus( tr("Scene added"));
}

void KTScenesWidget::removeScene()
{
	m_tableScenes->removeCurrentScene();
	emit sendToStatus( tr("Scene removed"));
}

void KTScenesWidget::selectScene(const QString & name, int index)
{
	emit changeCurrentScene(index);
}
void KTScenesWidget::actionButton( QAbstractButton *b)
{
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
}

void KTScenesWidget::emitRequestInsertScene()
{
	D_FUNCINFO;
	
	int index = m_tableScenes->indexCurrentScene() + 1;
	
	if ( index == 0 )
	{
		index = m_tableScenes->scenesCount();
	}
	
	
	KTSceneEvent event(KTProjectEvent::Add,  index);
	
	emit eventTriggered( &event );
	
// 	emit requestInsertScene();
// 	emit requestInsertLayer();
// 	emit requestInsertFrame();
}

void KTScenesWidget::emitRequestRemoveScene()
{
	DCONFIG->beginGroup("Scene");
	bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskScene", false));
	if ( ! noAsk )
	{
		DOptionalDialog dialog(tr("Do you want to remove this scene?"),tr("Remove?"), this);
		
		if( dialog.exec() == QDialog::Rejected )
		{
			return;
		}
		DCONFIG->setValue("RemoveWithoutAskScene", dialog.shownAgain());
		DCONFIG->sync();
	}
	
	KTSceneEvent event(KTProjectEvent::Remove,  m_tableScenes->indexCurrentScene() );
	
	emit eventTriggered( &event );
}



void KTScenesWidget::setScene(int index)
{
// m_tableScenes->setCurrentItem(m_tableScenes->topLevelItem ( index ) ); // FIXME
}

void KTScenesWidget::closeAllScenes()
{
	m_tableScenes->removeAll();
}

void KTScenesWidget::sceneEvent(KTSceneEvent *e)
{
	switch(e->action() )
	{
		case KTProjectEvent::Add:
		{
			m_tableScenes->insertScene(e->sceneIndex(), e->partName());
		}
		break;
		case KTProjectEvent::Remove:
		{
			m_tableScenes->removeScene(e->sceneIndex());
		}
		break;
	}
}

