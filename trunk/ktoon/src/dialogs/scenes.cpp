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

#include <qtooltip.h>
#include <qmessagebox.h>
#include <qheader.h>
#include <qlayout.h>

#include "scenes.h"
// #include "images.h"
// #include "ktoon.h"
#include "scene.h"
#include "ktapplication.h"
#include "ktdebug.h"
#include "status.h"

//--------------- CONSTRUCTOR --------------------

Scenes::Scenes( QWidget *parent)
	: KTDialogBase( QDockWindow::OutsideDock, parent, "Scenes")
{
    Q_CHECK_PTR( parent );
    KTINIT;
    //Initializations
    setCaption( tr( "Scenes" ) );

    move( 13, 300 );

    scene_max_value = 1;
    number_of_scenes = 1;
    
//     k_toon = ( KToon * )parent;

    //Icon Initializations
    m_imgs <<	QPixmap(KTOON_HOME+"/images/icons/plussign.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/minussign.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/arrowup.xpm" ) <<
		QPixmap(KTOON_HOME+"/images/icons/arrowdown.xpm" );
    setupButtons();
    setupTableScens();
    adjustSize();
}

//-------------- DESTRUCTOR -----------------

Scenes::~Scenes()
{
//     delete insert_scene; 
//     delete remove_scene;
//     delete move_scene_up;
//     delete move_scene_down;
	m_imgs.clear();
	delete table_scenes;
	delete text_name;
	delete value_name;
	KTEND;
}

void Scenes::setupButtons()
{
	//------------- Operations on the Buttons -----------------
	buttonsPanel = new QButtonGroup(this);
	QHBoxLayout *layout = new QHBoxLayout(buttonsPanel);
	layout->setAutoAdd(true);
	connect(buttonsPanel, SIGNAL(clicked(int )), this, SLOT(applyAction(int)));
	QStringList toolTips;
	toolTips << tr("Insert Scene") << tr("Remove Scene") << tr("Move Scene Up") << tr("Move Scene Down");
	
	for(int i = 0; i < toolTips.count(); i++)
	{
		QPushButton *tmpButton;
		tmpButton = new QPushButton(m_imgs[i], QString::null, buttonsPanel);
		tmpButton-> setAutoDefault( false );
		tmpButton-> setFlat( true );
		tmpButton-> setFocusPolicy( QWidget::NoFocus );
		tmpButton->adjustSize();
		tmpButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
		QToolTip::add( tmpButton, toolTips[i]  );
	}
	buttonsPanel->setMaximumHeight( buttonsPanel->sizeHint().height());
	addChild(buttonsPanel);
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
			number_of_scenes++;
			QCheckListItem *new_scene = new QCheckListItem( table_scenes, table_scenes -> lastItem(), tr( "Scene" ) + scene_number, QCheckListItem::CheckBox );
			new_scene -> setOn( true );
			
			Scene *n_scene = new Scene();

			
			n_scene -> setNameScene( tr( "Scene" ) + scene_number );
			QPtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
			sc.append( n_scene );
			KTStatus->currentDocument()->getAnimation()->setScenes( sc );
			KTStatus->currentDrawingArea()->modifyDocument( true );
			emit sceneInserted();
			table_scenes->setSelected( new_scene, true );
			ktDebug() << "Scenes::applyAction(int InsertScene) finished" << endl;
			break;
			
		}
		case RemoveScene:
		{
			QCheckListItem *scene_to_delete = ( QCheckListItem * )table_scenes -> selectedItem();
			int scene_pos = table_scenes -> itemPos( scene_to_delete );

			if ( number_of_scenes > 1 )
			{
				if ( QMessageBox::warning( this, tr( "Confirm Scene Elimination" ), tr( "Are you sure you want to delete the selected scene?" ),
				     QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton ) == QMessageBox::Yes )
				{
					delete scene_to_delete;
					table_scenes -> setSelected( table_scenes -> currentItem(), true );
					table_scenes -> triggerUpdate();
					number_of_scenes--;

					QPtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
					sc.setAutoDelete( true );
					sc.remove( scene_pos / 16 );
					sc.setAutoDelete( false );
					KTStatus->currentDocument() -> getAnimation() -> setScenes( sc );

					emit sceneRemoved( scene_pos / 16 );

					QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();
					scene_pos = table_scenes -> itemPos( selected_scene );
					QPtrList<Scene> sc1 = KTStatus->currentDocument() -> getAnimation() -> getScenes();
					KTStatus -> setCurrentScene( sc1.at( scene_pos / 16 ) );
					KTStatus->currentDrawingArea() -> modifyDocument( true );
				}
			}
			break;
		}
		case MoveSceneUp:
		{
			QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

			if ( selected_scene == ( QCheckListItem * )table_scenes -> firstChild() )
				return;

			int scene_pos = table_scenes -> itemPos( selected_scene );
			if ( selected_scene == ( QCheckListItem * )table_scenes -> firstChild() -> itemBelow() )
			{
				table_scenes -> takeItem( selected_scene );
				table_scenes -> insertItem( selected_scene );
				table_scenes -> setCurrentItem( selected_scene );
				QPtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
				Scene *ms = sc.take( scene_pos / 16 );
				sc.insert( scene_pos / 16 - 1, ms );
			}
			else
				selected_scene -> moveItem( selected_scene -> itemAbove() -> itemAbove() );
			QPtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
			Scene *ms = sc.take( scene_pos / 16 );
			sc.insert( scene_pos / 16 - 1, ms );
			KTStatus->currentDrawingArea() -> modifyDocument( true );

			emit sceneMovedUp( scene_pos / 16 );
			break;
		}
		case MoveSceneDown:
		{
			QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

			int scene_pos = table_scenes -> itemPos( selected_scene );
			if ( selected_scene != table_scenes -> lastItem() )
			{
				selected_scene -> moveItem( selected_scene -> itemBelow() );
				QPtrList<Scene> sc = KTStatus->currentDocument() -> getAnimation() -> getScenes();
				Scene *ms = sc.take( scene_pos / 16 );
				sc.insert( scene_pos / 16 + 1, ms );
				emit sceneMovedDown( scene_pos / 16 );
				KTStatus->currentDrawingArea() -> modifyDocument( true );
			}
			break;
		}
		default:
		{
// 			ktError() << 
			break;
		}
	}
}

void Scenes::setupTableScens()
{
	//------------ Operations on the scene table -------------
	containerTableScens = new QVBox(this);
	table_scenes = new QListView( containerTableScens, "", Qt::WStyle_NoBorder );
	table_scenes->setSelectionMode( QListView::Single );
	table_scenes->addColumn( tr( "Name" ), 105 ); //Single column for the name
	table_scenes->setResizeMode( QListView::AllColumns );
	table_scenes->setColumnWidthMode ( 0, QListView::Maximum );
	table_scenes->setSorting( 10 ); //Not automatic sorting (10 > 1)
	table_scenes->header() -> hide();
	connect( table_scenes, SIGNAL( selectionChanged() ), SLOT( slotSelectScene() ) );

	QCheckListItem *default_scene = new QCheckListItem( table_scenes, tr( "Scene" ) + QString( "1" ), QCheckListItem::CheckBox );
	default_scene->setOn( true );
	
	   //------------- Operations on the Textfields -------------

	value_name = new QLineEdit( tr( "Scene1" ), containerTableScens );
	value_name -> setMaxLength( 10 );
	connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
	connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );

	//------------- Operations on the static texts ----------
	text_name = new QLabel( tr( "Scene Name" ), containerTableScens );
// 	text_name -> setAlignment( Qt::AlignTop );
	addChild(containerTableScens);
}
//--------------- PUBLIC MEMBERS ------------------

void Scenes::loadScenes( QPtrList<Scene> scenes )
{
	ktDebug() << "Scenes::loadScenes init" << endl;
	scene_max_value = 0;
	number_of_scenes = 0;
	
	table_scenes -> clear();
	
	Scene *s_it = scenes.first();
	QCheckListItem *s = new QCheckListItem( table_scenes, s_it->nameScene(), QCheckListItem::CheckBox );
	s -> setOn( true );
	s_it = scenes.next();
	scene_max_value++;
	number_of_scenes++;
	for ( ; s_it; s_it = scenes.next() )
	{
		QCheckListItem *s1 = new QCheckListItem( table_scenes, table_scenes -> lastItem(),s_it -> nameScene(), QCheckListItem::CheckBox );
		s1 -> setOn( true );
		scene_max_value++;
		number_of_scenes++;
	}
	ktDebug() << "Scenes::loadScenes( ) finished" << endl;
}

void Scenes::selectFirstScene()
{
	Q_CHECK_PTR(table_scenes);
	Q_CHECK_PTR(table_scenes -> firstChild());
	
	if ( table_scenes -> firstChild() )
    		table_scenes -> setSelected( table_scenes -> firstChild(), true );
}

int Scenes::exportAnimation( const QString &file_name, const QString &format )
{
    QListViewItemIterator it( table_scenes, QListViewItemIterator::Checked );
    int acc = 1, flag = 0;
    while ( it.current() )
    {
	table_scenes -> setSelected( it.current(), true );
// 	flag = k_toon -> timeline() -> exportAnimation( file_name, acc, format );
	if ( flag == 0 )
	    return 0;
	acc = flag;
	++it;
    }

    return acc;
}

//------------------- SLOTS ----------------------------

void Scenes::slotInsertScene()
{
	applyAction(InsertScene);
}

void Scenes::slotRemoveScene()
{
	applyAction(RemoveScene);
}

void Scenes::slotChangeValueName()
{
	QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

    //If the new name is the empty string, leave the value that it had before
	if ( value_name -> text() == "" )
	{
		selected_scene -> setText( 0, current_name );
		value_name -> setText( current_name );
		KTStatus -> currentScene() -> setNameScene( current_name );
	}
	else
	{
		selected_scene -> setText( 0, value_name -> text() );
		KTStatus -> currentScene() -> setNameScene( value_name -> text() );
	}
	KTStatus->currentDrawingArea() -> modifyDocument( true );
}

void Scenes::slotSelectScene()
{
	ktDebug() << "Scenes::slotSelectScene() Init" <<  endl;
	QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();
    //Don't allow deselection
	if ( selected_scene == 0 )
	{
		table_scenes->setSelected( table_scenes -> currentItem(), true );
		return;
	}
	
	int scene_pos = table_scenes -> itemPos( selected_scene );
	ktDebug() << "Scenes::scene_pos " << scene_pos/16  << endl;
	current_name = selected_scene->text( 0 );
	value_name -> setText( selected_scene -> text( 0 ) );

	QPtrList<Scene> sc = KTStatus->currentDocument()->getAnimation() -> getScenes();
	ktDebug() << "Scenes::sc.count() " << sc.count()  << endl;
	
	
	//se optiene la posiscion de la escena seleccionada, y se coloca como la currentScene em KTStatus
	if(sc.at( scene_pos / 16 ))
	{
		KTStatus -> setCurrentScene( sc.at( scene_pos / 16 ) );
		ktDebug() << "Scenes KTStatus -> setCurrentScene( sc.at( scene_pos / 16 ) );" ;
	}


	emit sceneSelected( scene_pos / 16 );
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
	ktDebug() << "Scenes::slotSelectScene() finished" <<  endl;
}

void Scenes::slotMoveSceneUp()
{
	applyAction(MoveSceneUp);
}

void Scenes::slotMoveSceneDown()
{
	applyAction(MoveSceneDown);
}
