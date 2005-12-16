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

#include "ktapplication.h"
#include "ktdebug.h"
#include "ktimagebutton.h"

#include <QToolTip>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>



//--------------- CONSTRUCTOR --------------------

KTScenesWidget::KTScenesWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTScenesWidget")
{
	KTINIT;
	
	setCaption( tr( "Scenes Manager" ) );
	
	setupButtons();
	setupTableScenes();
}

//-------------- DESTRUCTOR -----------------

KTScenesWidget::~KTScenesWidget()
{
	KTEND;
	delete m_textName;
	delete m_valueName;
}

void KTScenesWidget::setupButtons()
{
	//------------- Operations on the Buttons -----------------
	m_buttonsPanel = new QGroupBox(this);
	
	QHBoxLayout *layout = new QHBoxLayout(m_buttonsPanel);
	layout->setMargin(0);

	KTImageButton *insertButton = new KTImageButton(QPixmap(KTOON_HOME+"/themes/default/icons/plussign.png" ) , 22, m_buttonsPanel);
	layout->addWidget(insertButton);
	insertButton->setToolTip(tr("Insert Scene"));
	connect(insertButton, SIGNAL(clicked()), this, SIGNAL(requestInsertScene()));
	
	KTImageButton *removeButton = new KTImageButton(QPixmap(KTOON_HOME+"/themes/default/icons/minussign.png" ) , 22, m_buttonsPanel);
	layout->addWidget(removeButton);
	insertButton->setToolTip(tr("Remove Scene"));
	connect(removeButton, SIGNAL(clicked()), this, SIGNAL(requestRemoveScene()));
	
	KTImageButton *moveupButton = new KTImageButton(QPixmap(KTOON_HOME+"/themes/default/icons/arrowup.png" ) , 22, m_buttonsPanel);
	layout->addWidget(moveupButton);
	moveupButton->setToolTip(tr("Move Scene Up"));
	connect(moveupButton, SIGNAL(clicked()), this, SIGNAL(requestMoveUpScene()));
	
	KTImageButton *movedownButton = new KTImageButton(QPixmap(KTOON_HOME+"/themes/default/icons/arrowdown.png" ) , 22, m_buttonsPanel);
	layout->addWidget(movedownButton);
	movedownButton->setToolTip(tr("Move Scene Down"));
	connect(movedownButton, SIGNAL(clicked()), this, SIGNAL(requestMoveDownScene()));

	addChild(m_buttonsPanel);
}


// 		case RemoveScene:
// 		{
// 			if ( m_tableScenes->scenesCount() > 1 )
// 			{
// 				if ( QMessageBox::warning( this, tr( "Confirm Scene Elimination" ), tr( "Are you sure you want to delete the selected scene?" ),
// 				     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
// 				{
// 					int indexScene = m_tableScenes->removeCurrentScene();
// 					
// 					emit sceneRemoved(indexScene);
// 				}
// 			}
// 			break;
// 		}
// 		case MoveSceneUp:
// 		{
// 			int indexScene = m_tableScenes->moveCurrentSceneUp();
// 			emit sceneMovedUp(indexScene);
// 
// 			break;
// 		}
// 		case MoveSceneDown:
// 		{
// 			int indexScene = m_tableScenes->moveCurrentSceneDown();
// 			emit sceneMovedUp(indexScene);
// 
// 		}
// 		default:
// 		{
// 			break;
// 		}
// 	}
// }

void KTScenesWidget::setupTableScenes()
{
	//------------ Operations on the scene table -------------
	
	m_tableScenes = new KTScenesList(this);
	
	addChild( m_tableScenes);
	connect(m_tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(m_tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));

	m_textName = new QLabel( tr( "Name" ), this );
	m_textName -> setAlignment( Qt::AlignTop );
	addChild(m_textName);
	
	m_valueName = new QLineEdit( tr( "" ), this );
	m_valueName -> setMaxLength( 10 );
	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( changeValueName() ) );
	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( changeValueName() ) );
	addChild( m_valueName);
// 
// 	addChild(containerTableScens);
}

//------------------- SLOTS ----------------------------

void KTScenesWidget::insertScene(const QString &name, bool addedToEnd)
{
	KT_FUNCINFO;
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
// 	ktDebug() << "selectScene( " << index << ")" ;
// 	if(index != m_tableScenes->indexCurrentScene())
// 	{
		emit changeCurrentScene(index);
// 	}
		
// 	ktDebug() << "KTScenesWidget::selectScene() Init" <<  endl;
// 	m_valueName->setText(name);
// 	emit sceneSelected(index);
// 	
// 	ktDebug() << "KTScenesWidget::selectScene() finished" <<  endl;
}
// 
// void KTScenesWidget::moveSceneUp()
// {
// 	applyAction(MoveSceneUp);
// }
// 
// void KTScenesWidget::moveSceneDown()
// {
// 	applyAction(MoveSceneDown);
// }
// 
void KTScenesWidget::actionButton( QAbstractButton *b)
{
// 	applyAction(m_buttonGroup->buttons().indexOf(b));
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
// 	int index = m_tableScenes->indexOfTopLevelItem(item);
// 	emit( esceneDobleClicked(index));
}


void KTScenesWidget::setScene(int index)
{
	m_tableScenes->setCurrentItem(m_tableScenes->topLevelItem ( index) );
}
