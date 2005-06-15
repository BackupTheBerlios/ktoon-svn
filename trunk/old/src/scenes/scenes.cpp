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

#include "scenes.h"
#include "../images/images.h"
#include "../ktoon.h"
#include "../store/scene.h"

//--------------- CONSTRUCTOR --------------------

Scenes::Scenes( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
    : QDialog( parent, "Scenes", false, style )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( in_assigned_menu );
    Q_CHECK_PTR( assig_tb_button );
    
    //Initializations
    setCaption( tr( "Scenes" ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 120, 200 );
    setMinimumSize( 120, 200 );
    setMaximumSize( 120, 200 );
    move( 13, 300 );
    parent_widget = parent;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
    scene_max_value = 1;
    number_of_scenes = 1;
    k_toon = ( Ktoon * )parent_widget;

    //Icon Initializations
    i_insert_scene = QPixmap( plussign_xpm );
    i_remove_scene = QPixmap( minussign_xpm );
    i_move_scene_up = QPixmap( arrowup_xpm );
    i_move_scene_down = QPixmap( arrowdown_xpm );

    //------------- Operations on the Buttons -----------------

    insert_scene = new QPushButton( i_insert_scene, tr( "" ), this );
    insert_scene -> resize( 20, 20 );
    insert_scene -> move( 5, 5 );
    insert_scene -> setAutoDefault( false );
    insert_scene -> setFlat( true );
    connect( insert_scene, SIGNAL( clicked() ), SLOT( slotInsertScene() ) );
    QToolTip::add( insert_scene, tr( "Insert Scene" ) );

    remove_scene = new QPushButton( i_remove_scene, tr( "" ), this );
    remove_scene -> resize( 20, 20 );
    remove_scene -> move( 30, 5 );
    remove_scene -> setAutoDefault( false );
    remove_scene -> setFlat( true );
    connect( remove_scene, SIGNAL( clicked() ), SLOT( slotRemoveScene() ) );
    QToolTip::add( remove_scene, tr( "Remove Scene" ) );

    move_scene_up = new QPushButton( i_move_scene_up, tr( "" ), this );
    move_scene_up -> resize( 20, 20 );
    move_scene_up -> move( 55, 5 );
    move_scene_up -> setAutoDefault( false );
    move_scene_up -> setFlat( true );
    connect( move_scene_up, SIGNAL( clicked() ), SLOT( slotMoveSceneUp() ) );
    QToolTip::add( move_scene_up, tr( "Move Scene Up" ) );

    move_scene_down = new QPushButton( i_move_scene_down, tr( "" ), this );
    move_scene_down -> resize( 20, 20 );
    move_scene_down -> move( 80, 5 );
    move_scene_down -> setAutoDefault( false );
    move_scene_down -> setFlat( true );
    connect( move_scene_down, SIGNAL( clicked() ), SLOT( slotMoveSceneDown() ) );
    QToolTip::add( move_scene_down, tr( "Move Scene Down" ) );

    //------------ Operations on the scene table -------------

    table_scenes = new QListView( this, "", Qt::WStyle_NoBorder );
    table_scenes -> resize( 110, 120 );
    table_scenes -> move( 5, 30 );
    table_scenes -> setSelectionMode( QListView::Single );
    table_scenes -> setFont( QFont( "helvetica", 8 ) );
    table_scenes -> addColumn( tr( "Name" ), 105 ); //Single column for the name
    table_scenes -> setResizeMode( QListView::NoColumn );
    table_scenes -> setSorting( 10 ); //Not automatic sorting (10 > 1)
    table_scenes -> header() -> hide();
    connect( table_scenes, SIGNAL( selectionChanged() ), SLOT( slotSelectScene() ) );

    QCheckListItem *default_scene = new QCheckListItem( table_scenes, tr( "Scene" ) + QString( "1" ), QCheckListItem::CheckBox );
    default_scene -> setOn( true );

   //------------- Operations on the Textfields -------------

    value_name = new QLineEdit( tr( "Scene1" ), this );
    value_name -> resize( 80, 20 );
    value_name -> move( 5, table_scenes -> height() + table_scenes -> y() + 10 );
    value_name -> setFont( QFont( "helvetica", 8 ) );
    value_name -> setMaxLength( 10 );
    connect( value_name, SIGNAL( lostFocus() ), SLOT( slotChangeValueName() ) );
    connect( value_name, SIGNAL( returnPressed() ), SLOT( slotChangeValueName() ) );

    //------------- Operations on the static texts ----------

    text_name = new QLabel( tr( "Scene Name" ), this );
    text_name -> setFont( QFont( "helvetica", 7 ) );
    text_name -> resize( 100, 20 );
    text_name -> setAlignment( Qt::AlignTop );
    text_name -> move( value_name -> x(), value_name -> height() + value_name -> y() + 1 );
}

//-------------- DESTRUCTOR -----------------

Scenes::~Scenes()
{
    delete insert_scene; 
    delete remove_scene;
    delete move_scene_up;
    delete move_scene_down;
    delete table_scenes;
    delete text_name;
    delete value_name;
}

//--------------- PUBLIC MEMBERS ------------------

void Scenes::loadScenes( QPtrList<Scene> scenes )
{
    scene_max_value = 0;
    number_of_scenes = 0;

    table_scenes -> clear();

    Scene *s_it = scenes.first();
    QCheckListItem *s = new QCheckListItem( table_scenes, s_it -> nameScene(), QCheckListItem::CheckBox );
    s -> setOn( true );
    s_it = scenes.next();
    scene_max_value++;
    number_of_scenes++;
    for ( ; s_it; s_it = scenes.next() )
    {
        QCheckListItem *s1 = new QCheckListItem( table_scenes, table_scenes -> lastItem(),
	                                         s_it -> nameScene(), QCheckListItem::CheckBox );
	s1 -> setOn( true );
        scene_max_value++;
        number_of_scenes++;
    }
}

void Scenes::selectFirstScene()
{
    table_scenes -> setSelected( table_scenes -> firstChild(), true );
}

int Scenes::exportAnimation( const QString &file_name, const QString &format )
{
    QListViewItemIterator it( table_scenes, QListViewItemIterator::Checked );
    int acc = 1, flag = 0;
    while ( it.current() )
    {
	table_scenes -> setSelected( it.current(), true );
	flag = k_toon -> timeline() -> exportAnimation( file_name, acc, format );
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
    QString scene_number;
    scene_max_value++;
    scene_number.setNum( scene_max_value );
    number_of_scenes++;

    QCheckListItem *new_scene = new QCheckListItem( table_scenes, table_scenes -> lastItem(), tr( "Scene" ) + scene_number, QCheckListItem::CheckBox );
    new_scene -> setOn( true );

    Scene *n_scene = new Scene();
    n_scene -> setNameScene( tr( "Scene" ) + scene_number );
    QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
    sc.append( n_scene );
    k_toon -> document() -> getAnimation() -> setScenes( sc );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit sceneInserted();

    table_scenes -> setSelected( new_scene, true );
}

void Scenes::slotRemoveScene()
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

          QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
	  sc.setAutoDelete( true );
   	  sc.remove( scene_pos / 16 );
	  sc.setAutoDelete( false );
    	  k_toon -> document() -> getAnimation() -> setScenes( sc );

	  emit sceneRemoved( scene_pos / 16 );

          QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();
          scene_pos = table_scenes -> itemPos( selected_scene );
          QPtrList<Scene> sc1 = k_toon -> document() -> getAnimation() -> getScenes();
          k_toon -> currentStatus() -> setCurrentScene( sc1.at( scene_pos / 16 ) );
          k_toon -> drawingArea() -> modifyDocument( true );
      }
   }
}

void Scenes::slotChangeValueName()
{
    QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

    //If the new name is the empty string, leave the value that it had before
    if ( value_name -> text() == "" )
    {
        selected_scene -> setText( 0, current_name );
	value_name -> setText( current_name );
	k_toon -> currentStatus() -> currentScene() -> setNameScene( current_name );
    }
    else
    {
        selected_scene -> setText( 0, value_name -> text() );
	k_toon -> currentStatus() -> currentScene() -> setNameScene( value_name -> text() );
    }
    k_toon -> drawingArea() -> modifyDocument( true );
}

void Scenes::slotSelectScene()
{
    QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

    //Don't allow deselection
    if ( selected_scene == 0 )
    {
        table_scenes -> setSelected( table_scenes -> currentItem(), true );
	return;
    }

    int scene_pos = table_scenes -> itemPos( selected_scene );
    current_name = selected_scene -> text( 0 );
    value_name -> setText( selected_scene -> text( 0 ) );

    QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
    k_toon -> currentStatus() -> setCurrentScene( sc.at( scene_pos / 16 ) );

    emit sceneSelected( scene_pos / 16 );

    QPtrList<ESLayer> esly = k_toon -> exposureSheet() -> getLayers();
    ESLayer *escl = k_toon -> exposureSheet() -> currentLayer();
    int l_pos = esly.find( escl );
    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *cl = ly.at( l_pos );
    k_toon -> currentStatus() -> setCurrentLayer( cl );

    ILayer *il = k_toon -> exposureSheet() -> currentLayerObj();
    if ( il -> selectedFrame() != NULL && il -> selectedFrame() -> isUsed() )
    {
	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	QPtrList<ESFrame> esf;
	il -> availableFrames( &esf );
	k_toon -> currentStatus() -> setCurrentKeyFrame( kf.at( esf.find( il -> selectedFrame() ) ) );
    }
    else
        k_toon -> currentStatus() -> setCurrentKeyFrame( NULL );

    k_toon -> slotActivateCursor();
    k_toon -> drawingArea() -> updateGL();
    k_toon -> timeline() -> frameSequenceManager() -> getRuler() -> slotSetOffset( 1 );
    k_toon -> renderCameraPreview() -> updateGL();
}

void Scenes::slotMoveSceneUp()
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
        QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
	Scene *ms = sc.take( scene_pos / 16 );
	sc.insert( scene_pos / 16 - 1, ms );
    }
    else
    	selected_scene -> moveItem( selected_scene -> itemAbove() -> itemAbove() );
    QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
    Scene *ms = sc.take( scene_pos / 16 );
    sc.insert( scene_pos / 16 - 1, ms );
    k_toon -> drawingArea() -> modifyDocument( true );

    emit sceneMovedUp( scene_pos / 16 );
}

void Scenes::slotMoveSceneDown()
{
    QCheckListItem *selected_scene = ( QCheckListItem * )table_scenes -> selectedItem();

    int scene_pos = table_scenes -> itemPos( selected_scene );
    if ( selected_scene != table_scenes -> lastItem() )
    {
    	selected_scene -> moveItem( selected_scene -> itemBelow() );
        QPtrList<Scene> sc = k_toon -> document() -> getAnimation() -> getScenes();
	Scene *ms = sc.take( scene_pos / 16 );
	sc.insert( scene_pos / 16 + 1, ms );
	emit sceneMovedDown( scene_pos / 16 );
        k_toon -> drawingArea() -> modifyDocument( true );
    }
}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

void Scenes::closeEvent( QCloseEvent *close_event )
{
    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}
