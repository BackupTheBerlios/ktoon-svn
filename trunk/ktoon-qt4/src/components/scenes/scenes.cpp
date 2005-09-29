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
#include "scenes.h"
#include "scene.h"
#include "ktapplication.h"
#include "ktdebug.h"
#include "ktimagebutton.h"
// #include "status.h"

#include <QToolTip>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>



//--------------- CONSTRUCTOR --------------------

Scenes::Scenes( QWidget *parent)
	: KTModuleWidgetBase( parent, "Scenes")
{
    Q_CHECK_PTR( parent );
    KTINIT;
    //Initializations
    setCaption( tr( "Scenes" ) );

    move( 13, 300 );

    scene_max_value = 1;
    number_of_scenes = 1;
    

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

Scenes::~Scenes()
{
	KTEND;
	m_imgs.clear();
	delete m_textName;
	delete m_valueName;
}

void Scenes::setupButtons()
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

void Scenes::applyAction(int action)
{
	switch(action)
	{
		case InsertScene:
		{
			ktDebug() << "Scenes::applyAction(int InsertScene) init" << endl;
			
			QString scene_number;
			scene_max_value++;

			scene_number.setNum( scene_max_value );
			m_tableScenes->addScene(tr( "Scene" ) + scene_number);
			
			emit sceneInserted(m_tableScenes->nameCurrentScene(), m_tableScenes->indexCurrentScene() );
			
			number_of_scenes++;
// 			Q3CheckListItem *new_scene = new Q3CheckListItem( table_scenes, table_scenes -> lastItem(), tr( "Scene" ) + scene_number, Q3CheckListItem::CheckBox );
// 			new_scene -> setOn( true );
// 			
// 			Scene *n_scene = new Scene();
// 			n_scene -> setNameScene( tr( "Scene" ) + scene_number );
// 			Q3PtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 			sc.append( n_scene );
// 			KTStatus->currentDocument()->getAnimation()->setScenes( sc );
// 			KTStatus->currentDrawingArea()->modifyDocument( true );
// 			emit sceneInserted();
// 			table_scenes->setSelected( new_scene, true );
			ktDebug() << "Scenes::applyAction(int InsertScene) finished" << endl;
			break;
			
		}
		case RemoveScene:
		{
// 			Q3CheckListItem *scene_to_delete = ( Q3CheckListItem * )table_scenes -> selectedItem();
// 			int scene_pos = table_scenes -> itemPos( scene_to_delete );

			if ( number_of_scenes > 1 )
			{
				if ( QMessageBox::warning( this, tr( "Confirm Scene Elimination" ), tr( "Are you sure you want to delete the selected scene?" ),
				     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
				{
					int indexScene = m_tableScenes->removeCurrentScene();
					
					emit sceneRemoved(indexScene);
// 					delete scene_to_delete;
// 					table_scenes -> setSelected( table_scenes -> currentItem(), true );
// 					table_scenes -> triggerUpdate();
					number_of_scenes--;

// 					Q3PtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 					sc.setAutoDelete( true );
// 					sc.remove( scene_pos / 16 );
// 					sc.setAutoDelete( false );
// 					KTStatus->currentDocument() -> getAnimation() -> setScenes( sc );

// 					emit sceneRemoved( scene_pos / 16 );

// 					Q3CheckListItem *selected_scene = ( Q3CheckListItem * )table_scenes -> selectedItem();
// // 					scene_pos = table_scenes -> itemPos( selected_scene );
// 					Q3PtrList<Scene> sc1 = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 					KTStatus -> setCurrentScene( sc1.at( scene_pos / 16 ) );
// 					KTStatus->currentDrawingArea() -> modifyDocument( true );
				}
			}
			break;
		}
		case MoveSceneUp:
		{
			int indexScene = m_tableScenes->moveCurrentSceneUp();
			emit sceneMovedUp(indexScene);
// 			Q3CheckListItem *selected_scene = ( Q3CheckListItem * )table_scenes -> selectedItem();

// 			if ( selected_scene == ( Q3CheckListItem * )table_scenes -> firstChild() )
// 				return;

// 			int scene_pos = table_scenes -> itemPos( selected_scene );
// 			if ( selected_scene == ( Q3CheckListItem * )table_scenes -> firstChild() -> itemBelow() )
// 			{
// 				table_scenes -> takeItem( selected_scene );
// 				table_scenes -> insertItem( selected_scene );
// 				table_scenes -> setCurrentItem( selected_scene );
// 				Q3PtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 				Scene *ms = sc.take( scene_pos / 16 );
// 				sc.insert( scene_pos / 16 - 1, ms );
// 			}
// 			else
// 				selected_scene -> moveItem( selected_scene -> itemAbove() -> itemAbove() );
// 			Q3PtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 			Scene *ms = sc.take( scene_pos / 16 );
// 			sc.insert( scene_pos / 16 - 1, ms );
// 			KTStatus->currentDrawingArea() -> modifyDocument( true );

// 			emit sceneMovedUp( scene_pos / 16 );
			break;
		}
		case MoveSceneDown:
		{
			int indexScene = m_tableScenes->moveCurrentSceneDown();
			emit sceneMovedUp(indexScene);
			
// 			Q3CheckListItem *selected_scene = ( Q3CheckListItem * )table_scenes -> selectedItem();
// 
// 			int scene_pos = table_scenes -> itemPos( selected_scene );
// 			if ( selected_scene != table_scenes -> lastItem() )
// 			{
// 				selected_scene -> moveItem( selected_scene -> itemBelow() );
// 				Q3PtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
// 				Scene *ms = sc.take( scene_pos / 16 );
// 				sc.insert( scene_pos / 16 + 1, ms );
// 				emit sceneMovedDown( scene_pos / 16 );
// 				KTStatus->currentDrawingArea() -> modifyDocument( true );
// 			}
// 			break;
		}
		default:
		{
// 			ktError() << 
			break;
		}
	}
}

void Scenes::setupTableScenes()
{
	//------------ Operations on the scene table -------------
	
	m_tableScenes = new KTScenesList(this);
	m_tableScenes->addScene(tr( "Scene" ) + QString( "1" ));
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
// 	   //------------- Operations on the Textfields -------------
// 
	m_valueName = new QLineEdit( tr( "Scene1" ), this );
	m_valueName -> setMaxLength( 10 );
	connect( m_valueName, SIGNAL( lostFocus() ), SLOT( changeValueName() ) );
	connect( m_valueName, SIGNAL( returnPressed() ), SLOT( changeValueName() ) );
	addChild( m_valueName);
// 
// 	//------------- Operations on the static texts ----------
	m_textName = new QLabel( tr( "Scene Name" ), this );
	m_textName -> setAlignment( Qt::AlignTop );
	addChild(m_textName);
// 	addChild(containerTableScens);
}
//--------------- PUBLIC MEMBERS ------------------

void Scenes::loadScenes( QList<Scene*> scenes )
{
	ktDebug() << "Scenes::loadScenes init" << endl;
	scene_max_value = 0;
	number_of_scenes = 0;
	
// 	table_scenes -> clear();
	
// 	Scene *s_it = scenes.first();
// // 	Q3CheckListItem *s = new Q3CheckListItem( table_scenes, s_it->nameScene(), Q3CheckListItem::CheckBox );
// 	s -> setOn( true );
// 	s_it = scenes.next();
	scene_max_value++;
	number_of_scenes++;
// 	for ( ; s_it; s_it = scenes.next() )
// 	{
// 		Q3CheckListItem *s1 = new Q3CheckListItem( table_scenes, table_scenes -> lastItem(),s_it -> nameScene(), Q3CheckListItem::CheckBox );
// 		s1 -> setOn( true );
		scene_max_value++;
		number_of_scenes++;
// 	}
// 	ktDebug() << "Scenes::loadScenes( ) finished" << endl;
}

void Scenes::selectFirstScene()
{
// 	Q_CHECK_PTR(table_scenes);
// 	Q_CHECK_PTR(table_scenes -> firstChild());
	
// 	if ( table_scenes -> firstChild() )
//     		table_scenes -> setSelected( table_scenes -> firstChild(), true );
}

int Scenes::exportAnimation( const QString &file_name, const QString &format )
{
//     Q3ListViewItemIterator it( table_scenes, Q3ListViewItemIterator::Checked );
    int acc = 1, flag = 0;
//     while ( it.current() )
//     {
// 	table_scenes -> setSelected( it.current(), true );
// // 	flag = k_toon -> timeline() -> exportAnimation( file_name, acc, format );
// 	if ( flag == 0 )
// 	    return 0;
// 	acc = flag;
// 	++it;
//     }

    return acc;
}

//------------------- SLOTS ----------------------------

void Scenes::insertScene()
{
	applyAction(InsertScene);
}

void Scenes::removeScene()
{
	applyAction(RemoveScene);
}

void Scenes::changeValueName()
{
// 	
	
	
// 	Q3CheckListItem *selected_scene = dynamic_cast<Q3CheckListItem *>(table_scenes -> selectedItem());
	
// 	if( ! selected_scene )
// 	{
// 		return;
// 	}

    //If the new name is the empty string, leave the value that it had before

// 	if ( m_valueName -> text() == "" )
// 	{
// 		selected_scene -> setText( 0, current_name );
// 		m_valueName -> setText( current_name );
// // 		m_tableScenes->changeCurrentName( current_name);
// 		
// 		if ( KTStatus -> currentScene() )
// 		{
// 			KTStatus -> currentScene() -> setNameScene( current_name );
// 		}
// 	}
// 	else
// 	{
		m_tableScenes->changeCurrentName( m_valueName -> text());
		emit(sceneChangeName( m_valueName -> text(), m_tableScenes->indexCurrentScene()));
		
// 		selected_scene -> setText( 0, m_valueName -> text() );
// 		if ( KTStatus -> currentScene() )
// 		{
// 			KTStatus -> currentScene() -> setNameScene( m_valueName -> text() );
// 		}
// 	}
// 	if ( KTStatus->currentDrawingArea() )
// 	{
// 		KTStatus->currentDrawingArea() -> modifyDocument( true );
// 	}
}

void Scenes::selectScene(QString name, int index)
{
	ktDebug() << "Scenes::selectScene() Init" <<  endl;
	m_valueName->setText(name);
	emit sceneSelected(index);
	
	
// 	Q3CheckListItem *selected_scene = ( Q3CheckListItem * )table_scenes -> selectedItem();
    //Don't allow deselection
// 	if ( selected_scene == 0 )
// 	{
// 		table_scenes->setSelected( table_scenes -> currentItem(), true );
// 		return;
// 	}
// 	
// 	int scene_pos = table_scenes -> itemPos( selected_scene );
// 	ktDebug() << "Scenes::scene_pos " << scene_pos/16  << endl;
// 	current_name = selected_scene->text( 0 );
// 	m_valueName -> setText( selected_scene -> text( 0 ) );
// 
// 	Q3PtrList<Scene> sc = KTStatus->currentDocument()->getAnimation() -> getScenes();
// 	ktDebug() << "Scenes::sc.count() " << sc.count()  << endl;
// 	
	
	//se optiene la posiscion de la escena seleccionada, y se coloca como la currentScene em KTStatus
// 	if(sc.at( scene_pos / 16 ))
// 	{
// 		KTStatus -> setCurrentScene( sc.at( scene_pos / 16 ) );
// 		ktDebug() << "Scenes KTStatus -> setCurrentScene( sc.at( scene_pos / 16 ) );" ;
// 	}


// 	emit sceneSelected( scene_pos / 16 );
    //FIXME:
//     QPtrList<ESLayer> esly = k_toon-> exposureSheet() -> getLayers();
//     ESLayer *escl = k_toon -> exposureSheet() -> currentLayer();
//     int l_pos = esly.find( escl );
	
	//int l_pos es la posicion del currentLayer
	
	//FIXME: esto produce un crash cuando la escena es la 4 o 5 
// 	QPtrList<Layer> ly = KTStatus->currentScene()->getLayers();
	//se optienen los layers de la actual escena. se optiene el layer de actual 
	
// 	emit changedScene();
	
// 	Layer *cl = ly.at( 1 );
// 	KTStatus -> setCurrentLayer( cl );

//     ILayer *il = k_toon -> exposureSheet() -> currentLayerObj();
//     if ( il -> selectedFrame() != NULL && il -> selectedFrame() -> isUsed() )
//     {
// 	QPtrList<KeyFrame> kf = KTStatus -> currentLayer() -> keyFrames();
// 	QPtrList<ESFrame> esf;
// 	il -> availableFrames( &esf );
// 	KTStatus -> setCurrentKeyFrame( kf.at( esf.find( il -> selectedFrame() ) ) );
//     }
//     else
//         KTStatus -> setCurrentKeyFrame( NULL );

// 	k_toon -> slotActivateCursor();
// 	KTStatus->currentDrawingArea() -> updateGL();
// 	k_toon -> timeline() -> frameSequenceManager() -> getRuler() -> slotSetOffset( 1 );
// 	k_toon -> renderCameraPreview() -> updateGL();
	ktDebug() << "Scenes::selectScene() finished" <<  endl;
}

void Scenes::moveSceneUp()
{
	applyAction(MoveSceneUp);
}

void Scenes::moveSceneDown()
{
	applyAction(MoveSceneDown);
}

void Scenes::actionButton( QAbstractButton *b)
{
	applyAction(m_buttonGroup->buttons().indexOf(b));
}

void Scenes::sceneDobleClick(QTreeWidgetItem * item, int )
{
	int index = m_tableScenes->indexOfTopLevelItem(item);
	emit( esceneDobleClicked(index));
}
