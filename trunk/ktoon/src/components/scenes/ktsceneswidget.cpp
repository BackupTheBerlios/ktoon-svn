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



//--------------- CONSTRUCTOR --------------------

KTSceneManagersWidget::KTSceneManagersWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTSceneManagersWidget")
{
	DINIT;
	
	setCaption( tr( "Scenes manager" ) );
	
	setupButtons();
	setupTableScenes();
}

KTSceneManagersWidget::~KTSceneManagersWidget()
{
	DEND;
}

void KTSceneManagersWidget::setupButtons()
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

void KTSceneManagersWidget::setupTableScenes()
{
	m_tableScenes = new KTSceneManagersList(this);
	
	addChild( m_tableScenes);
	connect(m_tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(m_tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));

}

void KTSceneManagersWidget::insertScene(const QString &name, bool addedToEnd)
{
	D_FUNCINFO;
	m_tableScenes->addScene(name);
	emit sendToStatus( tr("Scene added"));
}

void KTSceneManagersWidget::removeScene()
{
	m_tableScenes->removeCurrentScene();
	emit sendToStatus( tr("Scene removed"));
}

void KTSceneManagersWidget::selectScene(const QString & name, int index)
{
	emit changeCurrentScene(index);
}
void KTSceneManagersWidget::actionButton( QAbstractButton *b)
{
}

void KTSceneManagersWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
}

void KTSceneManagersWidget::emitRequestInsertScene()
{
	emit requestInsertScene();
	emit requestInsertLayer();
	emit requestInsertFrame();
}

void KTSceneManagersWidget::emitRequestRemoveScene()
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
	emit requestRemoveScene();
}



void KTSceneManagersWidget::setScene(int index)
{
	// 	m_tableScenes->setCurrentItem(m_tableScenes->topLevelItem ( index ) ); // FIXME
}

void KTSceneManagersWidget::closeAllScenes()
{
	m_tableScenes->removeAll();
}

