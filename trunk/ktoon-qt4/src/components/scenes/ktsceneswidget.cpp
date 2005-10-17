/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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
	Q_CHECK_PTR( parent );
	KTINIT;
    //Initializations
	setCaption( tr( "KTScenesWidget" ) );

	move( 13, 300 );

	scene_max_value = 0;

    	//Icon Initializations
	m_imgs <<	QPixmap(KTOON_HOME+"/images/icons/plussign.xpm" ) <<
			QPixmap(KTOON_HOME+"/images/icons/minussign.xpm" ) <<
			QPixmap(KTOON_HOME+"/images/icons/arrowup.xpm" ) <<
			QPixmap(KTOON_HOME+"/images/icons/arrowdown.xpm" );
	setupButtons();
	setupTableScenes();
	adjustSize();
}

//-------------- DESTRUCTOR -----------------

KTScenesWidget::~KTScenesWidget()
{
	KTEND;
	m_imgs.clear();
	delete m_textName;
	delete m_valueName;
}

void KTScenesWidget::setupButtons()
{
	//------------- Operations on the Buttons -----------------
	m_buttonsPanel = new QGroupBox(this);
	m_buttonGroup = new QButtonGroup(m_buttonsPanel);
	connect(m_buttonGroup, SIGNAL(buttonClicked ( QAbstractButton *)), this, SLOT(actionButton( QAbstractButton *)));
	
	QHBoxLayout *layout = new QHBoxLayout(m_buttonsPanel);
	layout->setMargin(0);
	connect(m_buttonsPanel, SIGNAL(clicked(int )), this, SLOT(applyAction(int)));
	QStringList toolTips;
	toolTips << tr("Insert Scene") << tr("Remove Scene") << tr("Move Scene Up") << tr("Move Scene Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton *tmpButton;
		tmpButton = new KTImageButton(m_imgs[i], 22, m_buttonsPanel);
		layout->addWidget(tmpButton);
		m_buttonGroup->addButton(tmpButton);
		QToolTip::add( tmpButton, toolTips[i]);
	}
	m_buttonsPanel->setMaximumHeight( m_buttonsPanel->sizeHint().height());
	addChild(m_buttonsPanel);
}

void KTScenesWidget::applyAction(int action)
{
	switch(action)
	{
		case InsertScene:
		{
			ktDebug() << "KTScenesWidget::applyAction(int InsertScene) init" << endl;
			
			QString scene_number;
			scene_max_value++;

			scene_number.setNum( scene_max_value );
			m_tableScenes->addScene(tr( "Scene" ) + scene_number);
			
			emit sceneInserted(m_tableScenes->nameCurrentScene(), m_tableScenes->indexCurrentScene() );
			
			ktDebug() << "KTScenesWidget::applyAction(int InsertScene) finished" << endl;
			break;
			
		}
		case RemoveScene:
		{
			if ( m_tableScenes->scenesCount() > 1 )
			{
				if ( QMessageBox::warning( this, tr( "Confirm Scene Elimination" ), tr( "Are you sure you want to delete the selected scene?" ),
				     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
				{
					int indexScene = m_tableScenes->removeCurrentScene();
					
					emit sceneRemoved(indexScene);
				}
			}
			break;
		}
		case MoveSceneUp:
		{
			int indexScene = m_tableScenes->moveCurrentSceneUp();
			emit sceneMovedUp(indexScene);

			break;
		}
		case MoveSceneDown:
		{
			int indexScene = m_tableScenes->moveCurrentSceneDown();
			emit sceneMovedUp(indexScene);

		}
		default:
		{
			break;
		}
	}
}

void KTScenesWidget::setupTableScenes()
{
	//------------ Operations on the scene table -------------
	
	m_tableScenes = new KTScenesList(this);
// 	m_tableScenes->addScene(tr( "Scene" ) + QString( "1" ));
	addChild( m_tableScenes);
	connect(m_tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(m_tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));
	
	
	
// 	containerTableScens = new Q3VBox(this);
// 	table_scenes = new Q3ListView( containerTableScens, "", Qt::WStyle_NoBorder );
// 	table_scenes->setSelectionMode( Q3ListView::Single );
// 	table_scenes->addColumn( tr( "Name" ), 105 ); //Single column for the name
// 	table_scenes->setResizeMode( Q3ListView::AllColumns );
// 	table_scenes->setColumnWidthMode ( 0, Q3ListView::Maximum );
// 	table_scenes->setSorting( 10 ); //Not automatic sorting (10 > 1)
// 	table_scenes->header() -> hide();
// 	connect( table_scenes, SIGNAL( selectionChanged() ), SLOT( selectScene() ) );
// 
// 	Q3CheckListItem *default_scene = new Q3CheckListItem( table_scenes, tr( "Scene" ) + QString( "1" ), Q3CheckListItem::CheckBox );
// 	default_scene->setOn( true );
// 	
	// 	//------------- Operations on the static texts ----------
	m_textName = new QLabel( tr( "Name" ), this );
	m_textName -> setAlignment( Qt::AlignTop );
	addChild(m_textName);
	
// 	   //------------- Operations on the Textfields -------------
// 
	m_valueName = new QLineEdit( tr( "" ), this );
	m_valueName -> setMaxLength( 10 );
	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( changeValueName() ) );
	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( changeValueName() ) );
	addChild( m_valueName);
// 
// 	addChild(containerTableScens);
}

//------------------- SLOTS ----------------------------

void KTScenesWidget::insertScene()
{
	applyAction(InsertScene);
}

void KTScenesWidget::removeScene()
{
	applyAction(RemoveScene);
}

void KTScenesWidget::changeValueName()
{
	m_tableScenes->changeCurrentName( m_valueName -> text());
	emit(sceneRenamed( m_valueName -> text(), m_tableScenes->indexCurrentScene()));
}

void KTScenesWidget::selectScene(const QString & name, int index)
{
	ktDebug() << "KTScenesWidget::selectScene() Init" <<  endl;
	m_valueName->setText(name);
	emit sceneSelected(index);
	
	ktDebug() << "KTScenesWidget::selectScene() finished" <<  endl;
}

void KTScenesWidget::moveSceneUp()
{
	applyAction(MoveSceneUp);
}

void KTScenesWidget::moveSceneDown()
{
	applyAction(MoveSceneDown);
}

void KTScenesWidget::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
	int index = m_tableScenes->indexOfTopLevelItem(item);
	emit( esceneDobleClicked(index));
}
