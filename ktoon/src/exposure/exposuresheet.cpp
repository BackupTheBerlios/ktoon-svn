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
#include <qfont.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qheader.h>

#include "../ktoon.h"
#include "../timeline/timeline.h"
#include "../images/images.h"
#include "../store/scene.h"

//--------------- CONSTRUCTOR --------------------

ExposureSheet::ExposureSheet( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
    : QDialog( parent, "Exposure Sheet", false, style )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( in_assigned_menu );
    Q_CHECK_PTR( assig_tb_button );

    //Initializations
    setCaption( tr( "Exposure Sheet" ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 230, 440 );
    parent_widget = parent;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
    layer_max_value = 1;
    number_of_layers = 1;
    k_toon = ( Ktoon * )parent_widget;
    to_copy = NULL;

    //Icon Initializations
    i_insert_layer = QPixmap( layerplus_xpm );
    i_remove_layer = QPixmap( layerminus_xpm );
    i_lock_frame = QPixmap( lock_xpm );
    i_insert_frame = QPixmap( frameplus_xpm );
    i_remove_frame = QPixmap( frameminus_xpm );
    i_move_frame_up = QPixmap( arrowup_xpm );
    i_move_frame_down = QPixmap( arrowdown_xpm );
    i_layer_visibility = QPixmap( show_hide_all_layers_xpm );

    //------------- Operations on the Buttons -----------------

    insert_layer = new QPushButton( i_insert_layer, tr( "" ), this );
    insert_layer -> resize( 20, 20 );
    insert_layer -> move( 5, 5 );
    insert_layer -> setAutoDefault( false );
    insert_layer -> setFlat( true );
    insert_layer -> setFocusPolicy( QWidget::NoFocus );
    connect( insert_layer, SIGNAL( clicked() ), SLOT( slotInsertLayer() ) );
    QToolTip::add( insert_layer, tr( "Insert Layer" ) );

    remove_layer = new QPushButton( i_remove_layer, tr( "" ), this );
    remove_layer -> resize( 20, 20 );
    remove_layer -> move( 30, 5 );
    remove_layer -> setAutoDefault( false );
    remove_layer -> setFlat( true );
    remove_layer -> setFocusPolicy( QWidget::NoFocus );
    connect( remove_layer, SIGNAL( clicked() ), SLOT( slotRemoveLayer() ) );
    QToolTip::add( remove_layer, tr( "Remove Layer" ) );

    layer_visibility = new QPushButton( i_layer_visibility, tr( "" ), this );
    layer_visibility -> resize( 20, 20 );
    layer_visibility -> move( 55, 5 );
    layer_visibility -> setAutoDefault( false );
    layer_visibility -> setFlat( true );
    layer_visibility -> setAutoRepeat( false );
    layer_visibility -> setFocusPolicy( QWidget::NoFocus );
    layer_visibility -> setToggleButton( true );
    connect( layer_visibility, SIGNAL( toggled( bool ) ), SLOT( slotLayerVisibility( bool ) ) );
    QToolTip::add( layer_visibility, tr( "Manage the layer visibility" ) );

    insert_frame = new QPushButton( i_insert_frame, tr( "" ), this );
    insert_frame -> resize( 20, 20 );
    insert_frame -> move( 80, 5 );
    insert_frame -> setAutoDefault( false );
    insert_frame -> setFlat( true );
    insert_frame -> setFocusPolicy( QWidget::NoFocus );
    connect( insert_frame, SIGNAL( clicked() ), SLOT( slotInsertFrame() ) );
    QToolTip::add( insert_frame, tr( "Insert Keyframes" ) );

    remove_frame = new QPushButton( i_remove_frame, tr( "" ), this );
    remove_frame -> resize( 20, 20 );
    remove_frame -> move( 105, 5 );
    remove_frame -> setAutoDefault( false );
    remove_frame -> setFlat( true );
    remove_frame -> setFocusPolicy( QWidget::NoFocus );
    connect( remove_frame, SIGNAL( clicked() ), SLOT( slotRemoveFrame() ) );
    QToolTip::add( remove_frame, tr( "Remove Keyframe" ) );

    lock_frame = new QPushButton( i_lock_frame, tr( "" ), this );
    lock_frame -> resize( 20, 20 );
    lock_frame -> move( 130, 5 );
    lock_frame -> setAutoDefault( false );
    lock_frame -> setFlat( true );
    lock_frame -> setFocusPolicy( QWidget::NoFocus );
    connect( lock_frame, SIGNAL( clicked() ), SLOT( slotLockFrame() ) );
    QToolTip::add( lock_frame, tr( "Lock Frame" ) );

    move_frame_up = new QPushButton( i_move_frame_up, tr( "" ), this );
    move_frame_up -> resize( 20, 20 );
    move_frame_up -> move( 155, 5 );
    move_frame_up -> setAutoDefault( false );
    move_frame_up -> setFlat( true );
    move_frame_up -> setAutoRepeat( true );
    move_frame_up -> setFocusPolicy( QWidget::NoFocus );
    connect( move_frame_up, SIGNAL( clicked() ), SLOT( slotMoveFrameUp() ) );
    QToolTip::add( move_frame_up, tr( "Move Keyframe Up" ) );

    move_frame_down = new QPushButton( i_move_frame_down, tr( "" ), this );
    move_frame_down -> resize( 20, 20 );
    move_frame_down -> move( 180, 5 );
    move_frame_down -> setAutoDefault( false );
    move_frame_down -> setFlat( true );
    move_frame_down -> setAutoRepeat( true );
    move_frame_down -> setFocusPolicy( QWidget::NoFocus );
    connect( move_frame_down, SIGNAL( clicked() ), SLOT( slotMoveFrameDown() ) );
    QToolTip::add( move_frame_down, tr( "Move Keyframe Down" ) );

    //--------------- Operations on the Scroll Area -------------------------

    scroll_area_container = new QScrollView( this );
    scroll_area = new QFrame( scroll_area_container -> viewport() );
    scroll_area -> resize( 2000, ( MAX_NUMBER_OF_FRAMES * 25 ) + 50 );
    scroll_area_container -> addChild( scroll_area );
    scroll_area_container -> resize( 190, 400 );
    scroll_area_container -> move( 5, remove_layer -> y() + remove_layer -> height() + 5 );

    //--------------- Operations on the Default Components ----------------------

    ESLayer *default_layer = new ESLayer( tr( "Layer" ) + QString( "1" ), scroll_area, this );
    default_layer -> move( remove_layer -> x(), 10 );
    last_layer = default_layer;
    current_layer = default_layer;
    connect( default_layer, SIGNAL( clicked() ), SLOT( slotSelectLayer() ) );
    connect( default_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
    ILayer *default_layer_obj = new ILayer( 1, default_layer );
    list_of_layers.append( default_layer_obj );
    QPtrList<ESFrame> default_layer_frame_list;
    first_layer = default_layer;

    QLabel *frame_enumeration = new QLabel( "1", scroll_area );
    frame_enumeration -> setFont( QFont( "Helvetica", 10, QFont::Bold ) );
    frame_enumeration -> setAlignment( Qt::AlignCenter );
    frame_enumeration -> resize( 25, 25 );
    frame_enumeration -> move( insert_layer -> x(), default_layer -> y() + default_layer -> height() );

    ESFrame *first_frame = new ESFrame( tr( "Drawing " ) + QString( "1-1" ), scroll_area, this );
    first_frame -> move( default_layer -> x(), default_layer -> y() + default_layer -> height() );
    first_frame -> setUsed( true );
    connect( first_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
    connect( first_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
    default_layer_obj -> setLastFrame( first_frame );
    default_layer_frame_list.append( first_frame );

    for ( int i = 2; i <= MAX_NUMBER_OF_FRAMES; i++ )
    {
        QString frame_number;
	frame_number.setNum( i );
    	QLabel *frame_enumeration = new QLabel( frame_number, scroll_area );
    	frame_enumeration -> setFont( QFont( "Helvetica", 10, QFont::Bold ) );
    	frame_enumeration -> setAlignment( Qt::AlignCenter );
    	frame_enumeration -> resize( 25, 25 );
    	frame_enumeration -> move( insert_layer -> x(), default_layer -> y() + default_layer -> height() * i );

    	ESFrame *next_frame = new ESFrame( "", scroll_area, this );
    	next_frame -> move( default_layer -> x(), default_layer -> y() + default_layer -> height() * i );
	connect( next_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
        connect( next_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
	default_layer_frame_list.append( next_frame );
    }

    default_layer_obj -> setAvailableFrames( default_layer_frame_list );
    f_frame = first_frame;

    //------------ Operations on the visibility list -------------

    visibility_list = new QListView( this, "", Qt::WStyle_NoBorder );
    visibility_list -> resize( 110, 120 );
    visibility_list -> move( layer_visibility -> x(), layer_visibility -> y() + layer_visibility -> height() );
    visibility_list -> setSelectionMode( QListView::Single );
    visibility_list -> setFont( QFont( "helvetica", 8 ) );
    visibility_list -> addColumn( tr( "Name" ), 105 ); //Single column for the name
    visibility_list -> setResizeMode( QListView::NoColumn );
    visibility_list -> setSorting( 10 ); //Not automatic sorting (10 > 1)
    visibility_list -> header() -> hide();
    visibility_list -> hide();

    QCheckListItem *default_layer_v = new QCheckListItem( visibility_list, tr( "Layer" ) + QString( "1" ), QCheckListItem::CheckBox );
    default_layer_v -> setVisible( true );
    default_layer_v -> setOn( true );
}

//-------------- DESTRUCTOR -----------------

ExposureSheet::~ExposureSheet()
{
    delete insert_layer;
    delete remove_layer;
    delete lock_frame;
    delete insert_frame;
    delete remove_frame;
    delete move_frame_up;
    delete move_frame_down;
    delete layer_visibility;
    delete visibility_list;
    delete scroll_area;
    delete scroll_area_container;
}

//------------- PUBLIC MEMBERS ------------------------

ESLayer *ExposureSheet::currentLayer()
{
    Q_CHECK_PTR( current_layer );
    return current_layer;
}

ILayer *ExposureSheet::currentLayerObj()
{
    return findCurrentLayerObj();
}

QPtrList<ESLayer> ExposureSheet::getLayers()
{
    QPtrList<ESLayer> res;
    ILayer *it;
    for ( it = list_of_layers.first(); it; it = list_of_layers.next() )
        res.append( it -> interfaceElement() );
    return res;
}

QPtrList<ILayer> ExposureSheet::getILayers()
{
    return list_of_layers;
}

void ExposureSheet::touchFirstFrame()
{
    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( f_frame, &mouse_event );
    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( f_frame, &mouse_event2 );
}

void ExposureSheet::touchFirstLayer()
{
    first_layer -> animateClick();
}

void ExposureSheet::loadLayersAndKeyframes( QPtrList<Layer> layers )
{
    layer_max_value = 0;
    number_of_layers = 0;
    delete first_layer;
    list_of_layers.setAutoDelete( true );
    list_of_layers.clear();
    list_of_layers.setAutoDelete( false );
    visibility_list -> clear();

    Layer *l_it;
    for ( l_it = layers.first(); l_it; l_it = layers.next() )
    {
        layer_max_value++;
        QPtrList<ESFrame> frame_list;

        ESLayer *new_layer = new ESLayer( l_it -> nameLayer(), scroll_area, this );
        if ( l_it == layers.getFirst() )
	{
	    new_layer -> move( remove_layer -> x(), 10 );
	    first_layer = new_layer;
	    current_layer = new_layer;
	}
	else
	    new_layer -> move( last_layer -> x() + last_layer -> width(), last_layer -> y() );
        last_layer = new_layer;
	connect( new_layer, SIGNAL( clicked() ), SLOT( slotSelectLayer() ) );
	connect( new_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
	ILayer *new_layer_obj = new ILayer( 0, new_layer );
	list_of_layers.append( new_layer_obj );
	number_of_layers++;

    	QCheckListItem *new_layer_v = new QCheckListItem( visibility_list, visibility_list -> lastItem(), l_it -> nameLayer(), QCheckListItem::CheckBox );
    	new_layer_v -> setOn( true );

	for ( int i = 1; i <= MAX_NUMBER_OF_FRAMES; i++ )
	{
    	    ESFrame *next_frame = new ESFrame( "", scroll_area, this );
	    if ( i == 1 && l_it == layers.getFirst() )
	        f_frame = next_frame;
	    next_frame -> move( new_layer -> x(), new_layer -> y() + new_layer -> height() * i );
	    connect( next_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
	    connect( next_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
	    frame_list.append( next_frame );
	}

	new_layer_obj -> setAvailableFrames( frame_list );

	//Load Keyframes
	QPtrList<KeyFrame> keyframes = l_it -> keyFrames();
	ESFrame *f_it;
	KeyFrame *k_it;
	for ( k_it = keyframes.first(), f_it = frame_list.first(); k_it; k_it = keyframes.next(), f_it = frame_list.next() )
	{
	    new_layer_obj -> addFrame();
	    new_layer_obj -> setLastFrame( f_it );
	    f_it -> setName( k_it -> nameKeyFrame() );
	    f_it -> setUsed( true );
	    f_it -> setMotion( k_it -> motionKeyFrame() );
	}
    }
    visibility_list -> show();
    visibility_list -> hide();
}

QPtrList<Layer> ExposureSheet::visibleLayers()
{
    QPtrList<Layer> whole = k_toon -> currentStatus() -> currentScene() -> getLayers();
    QPtrList<Layer> to_return;
    Layer *layer_iterator;
    QListViewItemIterator it( visibility_list );

    for ( layer_iterator = whole.first(); it.current() && layer_iterator; layer_iterator = whole.next(), it++ )
    {
	if ( ( ( QCheckListItem * )it.current() ) -> isOn() )
	    to_return.append( layer_iterator );
    }

    return to_return;
}

//------------------- SLOTS ----------------------------

void ExposureSheet::slotInsertLayer()
{
    QString layer_number;
    layer_max_value++;
    layer_number.setNum( layer_max_value );
    number_of_layers++;

    ESLayer *new_layer = new ESLayer( tr( "Layer" ) + layer_number, scroll_area, this );
    new_layer -> move( last_layer -> x() + last_layer -> width(), last_layer -> y() );
    new_layer -> show();
    last_layer = new_layer;
    connect( new_layer, SIGNAL( clicked() ), SLOT( slotSelectLayer() ) );
    connect( new_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
    ILayer *new_layer_obj = new ILayer( 1, new_layer );
    list_of_layers.append( new_layer_obj );
    QPtrList<ESFrame> new_layer_frame_list;

    //The only active frame
    ESFrame *associated_frame = new ESFrame( tr( "Drawing " ) + QString::number( number_of_layers ) + QString( "-1" ), scroll_area, this );
    associated_frame -> move( new_layer -> x(), new_layer -> y() + new_layer -> height() );
    associated_frame -> show();
    associated_frame -> setUsed( true );
    connect( associated_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
    connect( associated_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
    new_layer_obj -> setLastFrame( associated_frame );
    new_layer_frame_list.append( associated_frame );

    //Loop for creation of the default zones that will enclose frames of the layer
    for ( int i = 2; i <= MAX_NUMBER_OF_FRAMES; i++ )
    {
    	ESFrame *next_frame = new ESFrame( "", scroll_area, this );
    	next_frame -> move( new_layer -> x(), new_layer -> y() + new_layer -> height() * i );
    	next_frame -> show();
	connect( next_frame, SIGNAL( selected() ), SLOT( slotSelectFrame() ) );
        connect( next_frame, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameFrame( const QString & ) ) );
	new_layer_frame_list.append( next_frame );
    }

    new_layer_obj -> setAvailableFrames( new_layer_frame_list );

    Layer *n_layer = new Layer();
    n_layer -> setIndexLayer( number_of_layers );
    n_layer -> setNameLayer( tr( "Layer" ) + layer_number );
    QPtrList<KeyFrame> kf = n_layer -> keyFrames();
    kf.first() -> setNameKeyFrame( tr( "Drawing " ) + QString::number( number_of_layers ) + QString( "-1" ) );
    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    ly.append( n_layer );
    k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
    k_toon -> drawingArea() -> modifyDocument( true );

    if ( ( Timeline * )sender() != k_toon -> timeline() )
        emit layerInserted();

    updateIndicators( new_layer_obj );

    QCheckListItem *new_layer_v = new QCheckListItem( visibility_list, visibility_list -> lastItem(), tr( "Layer" ) + layer_number, QCheckListItem::CheckBox );
    new_layer_v -> setOn( true );
    visibility_list -> show();
    visibility_list -> hide();
}

void ExposureSheet::slotRemoveLayer()
{
   if ( number_of_layers > 1 )
   {
      ILayer *current_layer_obj;
      ESLayer *another_layer;

      current_layer_obj = findCurrentLayerObj();

      number_of_layers--;
      current_layer_obj -> deleteAllFrames();

      int r_pos = list_of_layers.find( current_layer_obj );

      //Case 1: When the layer is the last within the list of layers
      if ( current_layer_obj == list_of_layers.getLast() )
      {
	  list_of_layers.remove( current_layer_obj );
	  another_layer = list_of_layers.getLast() -> interfaceElement();
	  delete current_layer;
	  current_layer = another_layer;
	  last_layer = current_layer;

	  current_layer -> animateClick();
      }
      //Case 2: When the layer is any except the last
      else
      {
	  another_layer = list_of_layers.at( list_of_layers.find( current_layer_obj ) + 1 ) -> interfaceElement();
	  list_of_layers.remove( current_layer_obj );
	  delete current_layer;
	  current_layer = another_layer;
	  current_layer -> animateClick();

	  //Reaccomodate every layer next to the deleted layer
	  ILayer *layer_iterator;
	  for ( layer_iterator = list_of_layers.first(); current_layer != layer_iterator -> interfaceElement(); layer_iterator = list_of_layers.next() );
	  for ( ; layer_iterator; layer_iterator = list_of_layers.next() )
	  {
	      layer_iterator -> reacommodate( ILayer::LEFT );
	  }
      }

      QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
      ly.setAutoDelete( true );
      ly.remove( r_pos );
      ly.setAutoDelete( false );
      /* This is a brute-force way to set the layer indexes after a layer deletion but i am lazy to think of
       * another way. Moreover, the iterations over a QPtrList are very efficient.
       */
      Layer *l_it;
      for ( l_it = ly.first(); l_it; l_it = ly.next() )
      {
          l_it -> setIndexLayer( ly.find( l_it ) + 1 );
      }
      k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
      k_toon -> drawingArea() -> modifyDocument( true );

      if ( ( Timeline * )sender() != k_toon -> timeline() )
          emit layerRemoved();

      QListViewItem *to_delete = visibility_list -> itemAt( QPoint( 0, r_pos * 16 ) );
      delete to_delete;
   }
}

void ExposureSheet::slotLockFrame()
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    if ( selected_frame != NULL )
    {
        if ( selected_frame -> isLocked() )
	    selected_frame -> setLocked( false );
	else
	    selected_frame -> setLocked( true );
    }
}

void ExposureSheet::slotSelectLayer()
{
    ESLayer *selected_layer = ( ESLayer * )sender(); //Button that emitted the signal
    current_layer -> setSelected( false );
    current_layer = selected_layer;
    current_layer -> setSelected( true );

    //Deselect all frames that don't belong to the current layer
    ILayer *layer_iterator, *sel_layer;
    ESFrame *frame_iterator;
    QPtrList<ESFrame> current_list_of_frames;

    sel_layer = NULL;

    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
        if ( current_layer == layer_iterator -> interfaceElement() )
	{
	    sel_layer = layer_iterator;
	    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
	    Layer *sl = ly.at( list_of_layers.find( sel_layer ) );
	    k_toon -> currentStatus() -> setCurrentLayer( sl );
	}
        layer_iterator -> availableFrames( &current_list_of_frames );
	for ( frame_iterator = current_list_of_frames.first(); frame_iterator; frame_iterator = current_list_of_frames.next() )
	{
	    if ( selected_layer != layer_iterator -> interfaceElement() )
	    {
	        frame_iterator -> setSelected( false );
	        frame_iterator -> clearTextfieldFocus();
		layer_iterator -> setSelectedFrame( NULL );
	    }
	    else if ( frame_iterator -> isSelected() )
	        layer_iterator -> setSelectedFrame( frame_iterator );
	}
	( layer_iterator -> interfaceElement() ) -> clearTextfieldFocus();
    }

    if ( sel_layer -> selectedFrame() != NULL )
    {
        QPtrList<ESFrame> ef;
	sel_layer -> availableFrames( &ef );
	QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	k_toon -> currentStatus() -> setCurrentKeyFrame( kf.at( ef.find( sel_layer -> selectedFrame() ) ) );
    }
    else
	k_toon -> currentStatus() -> setCurrentKeyFrame( NULL );

    emit layerSelected( list_of_layers.find( sel_layer ) + 1 );
    emit frameSelected();
}

void ExposureSheet::slotSelectLayerFromTL( int pos )
{
    ESLayer *selected_layer = list_of_layers.at( pos ) -> interfaceElement();
    selected_layer -> animateClick();
}

void ExposureSheet::slotRenameLayer( const QString &name )
{
    ESLayer *renamed_layer = ( ESLayer * )sender();
    ILayer *layer_iterator, *renamed_layer_obj;
    int pos;

    renamed_layer_obj = NULL;

    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
        if ( layer_iterator -> interfaceElement() == renamed_layer )
	{
	    renamed_layer_obj = layer_iterator;
	    break;
	}
    }

    pos = list_of_layers.find( renamed_layer_obj );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *rl = ly.at( pos );
    rl -> setNameLayer( name );
    k_toon -> drawingArea() -> modifyDocument( true );

    QListViewItem *to_rename = visibility_list -> itemAt( QPoint( 0, pos * 16 ) );
    to_rename -> setText( 0, name );

    emit layerRenamed( pos, name );
}

void ExposureSheet::slotRenameLayerFromTL( int pos, const QString &name )
{
    ESLayer *renamed_layer = list_of_layers.at( pos ) -> interfaceElement();
    renamed_layer -> setText( name );
    k_toon -> drawingArea() -> modifyDocument( true );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *rl = ly.at( pos );
    rl -> setNameLayer( name );

    QListViewItem *to_rename = visibility_list -> itemAt( QPoint( 0, pos * 16 ) );
    to_rename -> setText( 0, name );
}

void ExposureSheet::slotMoveLayerLeft()
{
    ILayer *current_layer_obj, *prev_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    if ( current_layer_obj == list_of_layers.getFirst() )
        return;

    int pos_cl, pos_pl;
    pos_cl = list_of_layers.find( current_layer_obj );
    pos_pl = pos_cl - 1;
    current_layer_obj = list_of_layers.take( pos_cl );
    prev_layer_obj = list_of_layers.take( pos_pl );

    current_layer_obj -> reacommodate( ILayer::LEFT );
    prev_layer_obj -> reacommodate( ILayer::RIGHT );
    list_of_layers.insert( pos_pl, current_layer_obj );
    list_of_layers.insert( pos_cl, prev_layer_obj );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *cl = ly.take( pos_cl );
    ly.insert( pos_pl, cl );
    /* This is a brute-force way to set the layer indexes after a layer deletion but i am lazy to think of
     * another way. Moreover, the iterations over a QPtrList are very efficient.
     */
    Layer *l_it;
    for ( l_it = ly.first(); l_it; l_it = ly.next() )
        l_it -> setIndexLayer( ly.find( l_it ) + 1 );
    k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
    k_toon -> drawingArea() -> modifyDocument( true );

    if ( current_layer_obj -> interfaceElement() == last_layer )
        last_layer = prev_layer_obj -> interfaceElement();

    visibility_list -> show();
    visibility_list -> hide();
    QListViewItem *v_current = visibility_list -> itemAt( QPoint( 0, pos_cl * 16 ) );
    QListViewItem *v_prev = visibility_list -> itemAt( QPoint( 0, pos_pl * 16 ) );
    v_prev -> moveItem( v_current );
}

void ExposureSheet::slotMoveLayerRight()
{
    ILayer *current_layer_obj, *next_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    if ( current_layer_obj == list_of_layers.getLast() )
        return;

    int pos_cl, pos_nl;
    pos_cl = list_of_layers.find( current_layer_obj );
    pos_nl = pos_cl + 1;
    next_layer_obj = list_of_layers.take( pos_nl );
    current_layer_obj = list_of_layers.take( pos_cl );

    current_layer_obj -> reacommodate( ILayer::RIGHT );
    next_layer_obj -> reacommodate( ILayer::LEFT );
    list_of_layers.insert( pos_cl, next_layer_obj );
    list_of_layers.insert( pos_nl, current_layer_obj );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *cl = ly.take( pos_cl );
    ly.insert( pos_nl, cl );
    /* This is a brute-force way to set the layer indexes after a layer deletion but i am lazy to think of
     * another way. Moreover, the iterations over a QPtrList are very efficient.
     */
    Layer *l_it;
    for ( l_it = ly.first(); l_it; l_it = ly.next() )
        l_it -> setIndexLayer( ly.find( l_it ) + 1 );
    k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
    k_toon -> drawingArea() -> modifyDocument( true );

    if ( next_layer_obj -> interfaceElement() == last_layer )
        last_layer = current_layer_obj -> interfaceElement();

    visibility_list -> show();
    visibility_list -> hide();
    QListViewItem *v_current = visibility_list -> itemAt( QPoint( 0, pos_cl * 16 ) );
    QListViewItem *v_next = visibility_list -> itemAt( QPoint( 0, pos_nl * 16 ) );
    v_current -> moveItem( v_next );
}

void ExposureSheet::slotSwapWithLeftLayer( int cur_pos, int rel_pos )
{
    ILayer *current_layer_obj, *release_layer_obj;
    current_layer_obj = list_of_layers.take( cur_pos - 1 );
    release_layer_obj = list_of_layers.take( rel_pos - 1 );

    int distance = cur_pos - rel_pos;
    for ( int i = 1; i <= distance; i++ )
    {
	current_layer_obj -> reacommodate( ILayer::LEFT );
	release_layer_obj -> reacommodate( ILayer::RIGHT );
    }
    list_of_layers.insert( rel_pos - 1, current_layer_obj );
    list_of_layers.insert( cur_pos - 1, release_layer_obj );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *cl = ly.take( cur_pos - 1 );
    Layer *rl = ly.take( rel_pos - 1 );
    ly.insert( rel_pos - 1, cl );
    ly.insert( cur_pos - 1, rl );
    /* This is a brute-force way to set the layer indexes after a layer deletion but i am lazy to think of
     * another way. Moreover, the iterations over a QPtrList are very efficient.
     */
    Layer *l_it;
    for ( l_it = ly.first(); l_it; l_it = ly.next() )
        l_it -> setIndexLayer( ly.find( l_it ) + 1 );
    k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
    k_toon -> drawingArea() -> modifyDocument( true );

    if ( current_layer_obj -> interfaceElement() == last_layer )
        last_layer = release_layer_obj -> interfaceElement();

    visibility_list -> show();
    visibility_list -> hide();
    QListViewItem *v_cur = visibility_list -> itemAt( QPoint( 0, ( cur_pos - 1 ) * 16 ) );
    QListViewItem *v_rel = visibility_list -> itemAt( QPoint( 0, ( rel_pos - 1 ) * 16 ) );
    QListViewItem *v_cur_prev = visibility_list -> itemAt( QPoint( 0, ( cur_pos - 2 ) * 16 ) );
    QListViewItem *v_rel_prev;
    if ( rel_pos - 1 > 0 )
        v_rel_prev = visibility_list -> itemAt( QPoint( 0, ( rel_pos - 2 ) * 16 ) );
    else
        v_rel_prev = 0;
    if ( v_rel_prev == 0 )
    {
        visibility_list -> takeItem( v_cur );
        visibility_list -> insertItem( v_cur );
    }
    else
        v_cur -> moveItem( v_rel_prev );
    v_rel -> moveItem( v_cur_prev );
}

void ExposureSheet::slotSwapWithRightLayer( int cur_pos, int rel_pos )
{
    ILayer *current_layer_obj, *release_layer_obj;
    release_layer_obj = list_of_layers.take( rel_pos - 1 );
    current_layer_obj = list_of_layers.take( cur_pos - 1 );

    int distance = rel_pos - cur_pos;
    for ( int i = 1; i <= distance; i++ )
    {
	current_layer_obj -> reacommodate( ILayer::RIGHT );
	release_layer_obj -> reacommodate( ILayer::LEFT );
    }
    list_of_layers.insert( cur_pos - 1, release_layer_obj );
    list_of_layers.insert( rel_pos - 1, current_layer_obj );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *rl = ly.take( rel_pos - 1 );
    Layer *cl = ly.take( cur_pos - 1 );
    ly.insert( cur_pos - 1, rl );
    ly.insert( rel_pos - 1, cl );
    /* This is a brute-force way to set the layer indexes after a layer deletion but i am lazy to think of
     * another way. Moreover, the iterations over a QPtrList are very efficient.
     */
    Layer *l_it;
    for ( l_it = ly.first(); l_it; l_it = ly.next() )
    {
        l_it -> setIndexLayer( ly.find( l_it ) + 1 );
    }
    k_toon -> currentStatus() -> currentScene() -> setLayers( ly );
    k_toon -> drawingArea() -> modifyDocument( true );

    if ( release_layer_obj -> interfaceElement() == last_layer )
        last_layer = current_layer_obj -> interfaceElement();

    visibility_list -> show();
    visibility_list -> hide();
    QListViewItem *v_cur = visibility_list -> itemAt( QPoint( 0, ( cur_pos - 1 ) * 16 ) );
    QListViewItem *v_rel = visibility_list -> itemAt( QPoint( 0, ( rel_pos - 1 ) * 16 ) );
    QListViewItem *v_rel_prev = visibility_list -> itemAt( QPoint( 0, ( rel_pos - 2 ) * 16 ) );
    QListViewItem *v_cur_prev;
    if ( cur_pos - 1 > 0 )
        v_cur_prev = visibility_list -> itemAt( QPoint( 0, ( cur_pos - 2 ) * 16 ) );
    else
        v_cur_prev = 0;
    if ( v_cur_prev == 0 )
    {
        visibility_list -> takeItem( v_rel );
        visibility_list -> insertItem( v_rel );
    }
    else
        v_rel -> moveItem( v_cur_prev );
    v_cur -> moveItem( v_rel_prev );
}

void ExposureSheet::slotInsertFrame()
{
    ILayer *current_layer_obj = findCurrentLayerObj();
    int l_pos = list_of_layers.find( current_layer_obj ) + 1;

    //---------- Insert frames until the selected frame slot --------------

    if ( current_layer_obj -> selectedFrame() != NULL )
    {
        QPtrList<ESFrame> tmp_list_of_frames;
	current_layer_obj -> availableFrames( &tmp_list_of_frames );
	ESFrame *tmp_frame;

	for ( tmp_frame = tmp_list_of_frames.first(); tmp_frame; tmp_frame = tmp_list_of_frames.next() )
	{
	    //Performs this action only if the selected frame isn't one of the used frames
            if ( tmp_frame -> isSelected() && !( tmp_frame -> isUsed() ) )
	    {
	        ESFrame *current_layer_last_frame = current_layer_obj -> lastFrame();
		int number_of_insertions = 0;
	        while ( current_layer_last_frame != tmp_frame && !( tmp_frame -> isUsed() ) )
		{
		    current_layer_obj -> addFrame();
		    number_of_insertions++;

		    //Get the list of frames available and the last frame used
		    QPtrList<ESFrame> current_layer_frame_list;
		    current_layer_obj -> availableFrames( &current_layer_frame_list );
		    current_layer_last_frame = current_layer_obj -> lastFrame();

		    //Iterate over the list in order to find the next frame to the last frame and set it as the last frame
		    ESFrame *frame_iterator, *new_last_frame;

     	            new_last_frame = NULL;

		    for ( frame_iterator = current_layer_frame_list.first(); frame_iterator; frame_iterator = current_layer_frame_list.next() )
		    {
        		if ( frame_iterator == current_layer_last_frame )
			{
		            new_last_frame = current_layer_frame_list.next();
			    break;
			}
    		    }

		    int i = current_layer_frame_list.find( new_last_frame ) + 1;
		    new_last_frame -> setUsed( true );
    		    new_last_frame -> setText( tr( "Drawing " ) + QString::number( l_pos ) + QString( "-" ) + QString::number( i ) );
		    current_layer_obj -> setLastFrame( new_last_frame );

    		    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
		    KeyFrame *nkf = new KeyFrame();
    		    nkf -> setNameKeyFrame( tr( "Drawing " ) + QString::number( l_pos ) + QString( "-" ) + QString::number( i ) );
		    kf.append( nkf );
		    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );
		    k_toon -> currentStatus() -> setCurrentKeyFrame( nkf );
		}

	        k_toon -> slotActivateCursor();
	        k_toon -> drawingArea() -> modifyDocument( true );
		emit framesInsertedAtTheEnd( number_of_insertions );
		updateIndicators( current_layer_obj );
	        return;
	    }
	}
    }

    //---------- Insert a single frame at the end ------------------

    //Get the list of frames available and the last frame used
    QPtrList<ESFrame> current_layer_frame_list;
    current_layer_obj -> availableFrames( &current_layer_frame_list );
    ESFrame *current_layer_last_frame = current_layer_obj -> lastFrame();
    int i;

    ESFrame *frame_iterator, *new_last_frame;
    new_last_frame = NULL;

    //Iterate over the list in order to find the next frame to the last frame and set it as the last frame
    for ( i = 1, frame_iterator = current_layer_frame_list.first(); frame_iterator; i++, frame_iterator = current_layer_frame_list.next() )
    {
        if ( i == MAX_NUMBER_OF_FRAMES )
	    return;

        if ( frame_iterator == current_layer_last_frame )
	{
            new_last_frame = current_layer_frame_list.next();
	    break;
	}
    }

    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *nkf = new KeyFrame();
    nkf -> setNameKeyFrame( tr( "Drawing " ) + QString::number( l_pos ) + QString( "-" ) + QString::number( i + 1 ) );
    kf.append( nkf );
    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );

    current_layer_obj -> addFrame();
    emit framesInsertedAtTheEnd( 1 );
    new_last_frame -> setUsed( true );
    new_last_frame -> setText( tr( "Drawing " ) + QString::number( l_pos ) + QString( "-" ) + QString::number( i + 1 ) );
    current_layer_obj -> setLastFrame( new_last_frame );

    k_toon -> drawingArea() -> modifyDocument( true );

    //-------------- Insert a frame after the selected frame ----------------------

    if ( current_layer_obj -> selectedFrame() != NULL && current_layer_obj -> selectedFrame() -> isUsed() )
    {
	ESFrame *old_selected_frame = current_layer_obj -> selectedFrame();

    	QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    	QApplication::sendEvent( new_last_frame, &mouse_event );
    	QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    	QApplication::sendEvent( new_last_frame, &mouse_event2 );

	QPtrList<ESFrame> keyframe_list;
	current_layer_obj -> availableFrames( &keyframe_list );
	int distance = keyframe_list.find( new_last_frame ) - keyframe_list.find( old_selected_frame );

	for ( int j = 1; j < distance; j++ )
	    slotMoveFrameUp();
    }

    updateIndicators( current_layer_obj );
}

void ExposureSheet::slotRemoveFrame()
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    if ( current_layer_obj -> numberOfFrames() > 1 )
    {
    	//Case 1: if no frame is selected or it's selected an unused frame remove the last frame used
    	if ( current_layer_obj -> selectedFrame() == NULL || !( ( current_layer_obj -> selectedFrame() ) -> isUsed() ) )
    	{
    	    current_layer_obj -> removeFrame();
	    ( current_layer_obj -> lastFrame() ) -> setUsed( false );
	    ( current_layer_obj -> lastFrame() ) -> setMotion( false );
	    ( current_layer_obj -> lastFrame() ) -> setName( "" );

	    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
	    KeyFrame *lkf = kf.last();
	    if ( ( QPushButton * )sender() == remove_frame )
	    {
	        KeyFrame *pkf = kf.prev();
	        pkf -> setLengthKeyFrame( pkf -> lengthKeyFrame() + lkf -> lengthKeyFrame() );
	        emit frameRemoved( current_layer_obj -> numberOfFrames() );
	    }
	    kf.setAutoDelete( true );
	    kf.remove( lkf );
	    kf.setAutoDelete( false );
	    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );

	    //Iterate over this layer's list of frames in order to find the previous frame to the last frame and set it as the last frame
	    QPtrList<ESFrame> current_layer_frame_list;
	    current_layer_obj -> availableFrames( &current_layer_frame_list );
    	    ESFrame *frame_iterator, *new_last_frame;

	    new_last_frame = NULL;

    	    for ( frame_iterator = current_layer_frame_list.first(); frame_iterator; frame_iterator = current_layer_frame_list.next() )
    	    {
                if ( frame_iterator == current_layer_obj -> lastFrame() )
	        {
                    new_last_frame = current_layer_frame_list.prev();
	            break;
	        }
    	    }

            current_layer_obj -> setLastFrame( new_last_frame );
	}
    	//Case 2: if it's selected an used frame remove it (Is not a physical removing, but logical by reacommodating the frames)
    	else if ( current_layer_obj -> selectedFrame() -> isUsed() )
    	{
	    current_layer_obj -> removeFrame();
	    int i;

	    //Another frame iteration
	    QPtrList<ESFrame> current_layer_frame_list, current_layer_frame_list2;
	    current_layer_obj -> availableFrames( &current_layer_frame_list );
	    current_layer_obj -> availableFrames( &current_layer_frame_list2 );
    	    ESFrame *frame_iterator, *frame_iterator2, *new_last_frame;

	    new_last_frame = NULL;

	    for ( i = 1, frame_iterator = current_layer_frame_list.first(), frame_iterator2 = current_layer_frame_list2.first();
	          frame_iterator != current_layer_obj -> selectedFrame();
		  i++, frame_iterator = current_layer_frame_list.next(), frame_iterator2 = current_layer_frame_list2.next() );
	    frame_iterator2 = current_layer_frame_list2.next();
	    for ( ; frame_iterator2; frame_iterator = current_layer_frame_list.next(), frame_iterator2 = current_layer_frame_list2.next() )
	    {
	        //Set all the properties of the next frame to the current frame
	        frame_iterator -> setAllProperties( frame_iterator2 );

                if ( frame_iterator == current_layer_obj -> lastFrame() )
	        {
                    new_last_frame = current_layer_frame_list.prev();
	            break;
	        }
    	    }

	    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
            KeyFrame *pkf = kf.at( i - 2 );
	    KeyFrame *tkf = kf.at( i - 1 );
	    KeyFrame *nkf = kf.at( i );
	    if ( ( QPushButton * )sender() == remove_frame )
	    {
	        if ( pkf != 0 )
	            pkf -> setLengthKeyFrame( pkf -> lengthKeyFrame() + tkf -> lengthKeyFrame() );
	        else
		{
	            nkf -> setLengthKeyFrame( tkf -> lengthKeyFrame() + nkf -> lengthKeyFrame() );
		    nkf -> setOffsetKeyFrame( 1 );
		}
		emit frameRemoved( i );
	    }
	    else
	    {
	        KeyFrame *kf_iter;
		for ( kf_iter = kf.at( kf.find( nkf ) ); kf_iter; kf_iter = kf.next() )
		    kf_iter -> setOffsetKeyFrame( kf_iter -> offsetKeyFrame() - 1 );
	    }
	    kf.setAutoDelete( true );
	    kf.remove( i - 1 );
	    kf.setAutoDelete( false );
	    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );

	    ( current_layer_obj -> lastFrame() ) -> setUsed( false );
	    ( current_layer_obj -> lastFrame() ) -> setName( "" );
            current_layer_obj -> setLastFrame( new_last_frame );

    	    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    	    QApplication::sendEvent( current_layer_obj -> selectedFrame(), &mouse_event );
     	    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
     	    QApplication::sendEvent( current_layer_obj -> selectedFrame(), &mouse_event2 );
 	}
        k_toon -> drawingArea() -> modifyDocument( true );
	k_toon -> drawingArea() -> updateGL();
	k_toon -> slotActivateCursor();
	updateIndicators( current_layer_obj );
    }
}

void ExposureSheet::slotSelectFrame()
{
    //Deselect all frames other than selected and emit the selection signal on the layer
    ILayer *layer_iterator;
    ESFrame *frame_iterator;
    QPtrList<ESFrame> current_list_of_frames;
    ESLayer *selected_layer;

    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
        layer_iterator -> availableFrames( &current_list_of_frames );
	for ( frame_iterator = current_list_of_frames.first(); frame_iterator; frame_iterator = current_list_of_frames.next() )
	{
	    if ( frame_iterator != ( ESFrame * )sender() && frame_iterator -> isSelected() )
	        frame_iterator -> setSelected( false );
	    else if ( frame_iterator == ( ESFrame * )sender() )
	    {
	        selected_layer = layer_iterator -> interfaceElement();
		selected_layer -> animateClick();
		layer_iterator -> setSelectedFrame( frame_iterator );

		QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
		k_toon -> currentStatus() -> setCurrentLayer( ly.at( list_of_layers.find( layer_iterator ) ) );

		if ( frame_iterator -> isUsed() )
		{
		    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
		    k_toon -> currentStatus() -> setCurrentKeyFrame( kf.at( current_list_of_frames.find( frame_iterator ) ) );
		}
		else
		    k_toon -> currentStatus() -> setCurrentKeyFrame( NULL );
	    }
	    frame_iterator -> clearTextfieldFocus();
	}
    }
    emit frameSelected();
}

void ExposureSheet::slotRenameFrame( const QString &new_name )
{
    ESFrame *renamed_frame = ( ESFrame * )sender();
    ILayer *current_layer_obj;

    current_layer_obj = associatedLayer( renamed_frame );
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );

    QPtrList<Layer> ly = k_toon -> currentStatus() -> currentScene() -> getLayers();
    Layer *l = ly.at( list_of_layers.find( current_layer_obj ) );
    QPtrList<KeyFrame> kf = l -> keyFrames();
    kf.at( current_list_of_frames.find( renamed_frame ) ) -> setNameKeyFrame( new_name );
    k_toon -> drawingArea() -> modifyDocument( true );
}

void ExposureSheet::slotMoveFrameUp()
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    //If no frame is selected, do nothing
    if ( current_layer_obj -> selectedFrame() == NULL || !( current_layer_obj -> selectedFrame() -> isUsed() ) )
        return;

    ESFrame *selected_frame, *frame_above, *frame_iterator, *bridge_frame;

    frame_above = NULL;

    selected_frame = current_layer_obj -> selectedFrame();
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );

    //If the selected frame is the first, do nothing
    if ( selected_frame == current_list_of_frames.getFirst() )
        return;

    int i;
    //Find the frame that is above the selected frame
    for ( i = 1, frame_iterator = current_list_of_frames.first(); frame_iterator; i++, frame_iterator = current_list_of_frames.next() )
    {
	if ( selected_frame == frame_iterator )
	{
	    frame_above = current_list_of_frames.prev();
	    break;
	}
    }


    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *ck = kf.take( i - 1 );
    kf.insert( i - 2, ck );
    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );

    //Swap both frames
    bridge_frame = new ESFrame( "", scroll_area, this );
    bridge_frame -> setAllProperties( frame_above );
    frame_above -> setAllProperties( selected_frame );
    selected_frame -> setAllProperties( bridge_frame );
    current_layer_obj -> setSelectedFrame( frame_above );
    delete bridge_frame;
    emit frameMovedUp( i );

    k_toon -> drawingArea() -> modifyDocument( true );
    updateIndicators( current_layer_obj );
}

void ExposureSheet::slotMoveFrameDown()
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    //If no frame is selected, do nothing
    if ( current_layer_obj -> selectedFrame() == NULL || !( current_layer_obj -> selectedFrame() -> isUsed() ) )
        return;

    ESFrame *selected_frame, *frame_below, *frame_iterator, *bridge_frame;

    frame_below = NULL;

    selected_frame = current_layer_obj -> selectedFrame();
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );

    //If the selected frame is the last used frame, do nothing
    if ( selected_frame == current_layer_obj -> lastFrame() )
        return;

    //Find the frame that is below the selected frame
    int i;
    for ( i = 1, frame_iterator = current_list_of_frames.first(); frame_iterator; i++, frame_iterator = current_list_of_frames.next() )
    {
	if ( selected_frame == frame_iterator )
	{
	    frame_below = current_list_of_frames.next();
	    break;
	}
    }

    QPtrList<KeyFrame> kf = k_toon -> currentStatus() -> currentLayer() -> keyFrames();
    KeyFrame *ck = kf.take( i - 1 );
    kf.insert( i, ck );
    k_toon -> currentStatus() -> currentLayer() -> setKeyFrames( kf );

    //Swap both frames
    bridge_frame = new ESFrame( "", scroll_area, this );
    bridge_frame -> setAllProperties( frame_below );
    frame_below -> setAllProperties( selected_frame );
    selected_frame -> setAllProperties( bridge_frame );
    current_layer_obj -> setSelectedFrame( frame_below );
    delete bridge_frame;
    emit frameMovedDown( i );

    k_toon -> drawingArea() -> modifyDocument( true );
    updateIndicators( current_layer_obj );
}

void ExposureSheet::slotLayerVisibility( bool show )
{
    if ( show )
        visibility_list -> show();
    else
        visibility_list -> hide();
}

void ExposureSheet::slotRemoveFrameFromTL( int key )
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    if ( selected_frame != NULL )
        selected_frame -> setSelected( false );
    current_layer_obj -> setSelectedFrame( NULL );

    ESFrame *frame_iterator;
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );
    int i;

    for ( i = 1, frame_iterator = current_list_of_frames.first(); frame_iterator; i++, frame_iterator = current_list_of_frames.next() )
    {
	frame_iterator -> setSelected( true );
	current_layer_obj -> setSelectedFrame( frame_iterator );
	if ( i == key )
	    this -> slotRemoveFrame();
	frame_iterator -> setSelected( false );
	current_layer_obj -> setSelectedFrame( NULL );
    }

    k_toon -> drawingArea() -> modifyDocument( true );
}

void ExposureSheet::slotCreateMotionTween( int key )
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    ESFrame *frame_iterator;
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );
    int i;

    for ( i = 1, frame_iterator = current_list_of_frames.first(); frame_iterator; i++, frame_iterator = current_list_of_frames.next() )
    {
	if ( i == key )
	    frame_iterator -> setMotion( true );
    }

    k_toon -> drawingArea() -> modifyDocument( true );
}

void ExposureSheet::slotRemoveMotionTween( int key )
{
    ILayer *current_layer_obj;

    current_layer_obj = findCurrentLayerObj();

    ESFrame *frame_iterator;
    QPtrList<ESFrame> current_list_of_frames;
    current_layer_obj -> availableFrames( &current_list_of_frames );
    int i;

    for ( i = 1, frame_iterator = current_list_of_frames.first(); frame_iterator; i++, frame_iterator = current_list_of_frames.next() )
    {
	if ( i == key )
	    frame_iterator -> setMotion( false );
    }

    k_toon -> drawingArea() -> modifyDocument( true );
}

void ExposureSheet::slotCopyFrame()
{
    if ( to_copy != NULL )
        delete to_copy;
    KeyFrame *current_kf = k_toon -> currentStatus() -> currentKeyFrame();
    to_copy = new GLDrawing( *( current_kf -> getDrawing() ) );
}

void ExposureSheet::slotPasteFrame()
{
    if ( to_copy == NULL )
	return;

    if ( !( findCurrentLayerObj() -> selectedFrame() -> isUsed() ) )
	slotInsertFrame();

    KeyFrame *current_kf = k_toon -> currentStatus() -> currentKeyFrame();
    current_kf -> setDrawing( new GLDrawing( *to_copy ) );
    k_toon -> drawingArea() -> updateGL();
}

void ExposureSheet::updateIndicators( ILayer *layer )
{
    Q_CHECK_PTR( layer );
    QPtrList<ESFrame> list_of_esframes;
    layer -> availableFrames( &list_of_esframes );
    QPtrList<KeyFrame> list_of_keyframes = ( k_toon -> currentStatus() -> currentScene() -> getLayers() ).at( list_of_layers.find( layer ) ) -> keyFrames();

    ESFrame *esframe_iterator;
    KeyFrame *keyframe_iterator;
    for ( esframe_iterator = list_of_esframes.first(), keyframe_iterator = list_of_keyframes.first();
    	  keyframe_iterator;
	  esframe_iterator = list_of_esframes.next(), keyframe_iterator = list_of_keyframes.next() )
    {
        int cur_offset = keyframe_iterator -> offsetKeyFrame();
	int cur_length = keyframe_iterator -> lengthKeyFrame();
	QString tooltip_text = tr( "Offset: " ) + QString::number( cur_offset ) + tr( "\nLength: " ) + QString::number( cur_length ) + " ( " + QString::number( cur_offset + cur_length - 1 ) + " )";
	QToolTip::add( esframe_iterator, tooltip_text );
    }
    for ( ; esframe_iterator; esframe_iterator = list_of_esframes.next() )
        QToolTip::remove( esframe_iterator );
}

//--------------------- EVENTS AND PROTECTED MEMBERS --------------------------------

void ExposureSheet::closeEvent( QCloseEvent* close_event )
{
    Q_CHECK_PTR( close_event );

    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}

void ExposureSheet::resizeEvent( QResizeEvent *resize_event )
{
    Q_CHECK_PTR( resize_event );

    //Update the size of the scroll area in order to fit into the dialog box
    QSize new_size = resize_event -> size();
    new_size.setWidth( new_size.width() - 10 );
    new_size.setHeight( new_size.height() - 40 );
    scroll_area_container -> resize( new_size );
}

void ExposureSheet::keyPressEvent( QKeyEvent *key_event )
{
    Q_CHECK_PTR( key_event );

    switch( key_event -> key() )
    {
	case Qt::Key_F6: slotInsertFrame();
	    key_event -> accept();
	    break;
	case Qt::Key_Up: selectUp();
	    key_event -> accept();
	    break;
	case Qt::Key_Down: selectDown();
	    key_event -> accept();
	    break;
	case Qt::Key_Right: selectRight();
	    key_event -> accept();
	    break;
	case Qt::Key_Left: selectLeft();
	    key_event -> accept();
	    break;
	case Qt::Key_C:
	    if ( key_event -> state() == Qt::ControlButton )
	    {
		slotCopyFrame();
	        key_event -> accept();
	    }
	    else
	        key_event -> ignore();
	case Qt::Key_V:
	    if ( key_event -> state() == Qt::ControlButton )
	    {
		slotPasteFrame();
	        key_event -> accept();
	    }
	    else
	        key_event -> ignore();
	default: key_event -> ignore();
    }
}

ILayer *ExposureSheet::associatedLayer( ESFrame *f )
{
    Q_CHECK_PTR( f );
    ILayer *layer_iterator;
    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
	QPtrList<ESFrame> frames;
	ESFrame *frame_iterator;
	layer_iterator -> availableFrames( &frames );
	for ( frame_iterator = frames.first(); frame_iterator; frame_iterator = frames.next() )
	{
	    if ( frame_iterator == f )
	        return layer_iterator;
	}
    }

    return NULL;
}

ILayer *ExposureSheet::findCurrentLayerObj()
{
    ILayer *current_layer_obj = NULL;
    ILayer *layer_iterator;

    //Find the Layer that is selected within the list of layers
    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
	if ( layer_iterator -> interfaceElement() == current_layer )
	      current_layer_obj = layer_iterator;
    }

    Q_CHECK_PTR( current_layer_obj );
    return current_layer_obj;
}

void ExposureSheet::selectUp()
{
    ILayer *current_layer_obj = findCurrentLayerObj();
    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    int selected_frame_position = current_layer_obj -> framePosition( selected_frame );
    if ( selected_frame == NULL || selected_frame_position == 0 )
        return;

    ESFrame *new_selected_frame = current_layer_obj -> frameAt( selected_frame_position - 1 );

    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event );
    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event2 );

    if ( scroll_area_container -> contentsToViewport( new_selected_frame -> pos() ).y() < 0 )
        scroll_area_container -> scrollBy( 0, -200 );
}

void ExposureSheet::selectDown()
{
    ILayer *current_layer_obj = findCurrentLayerObj();
    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    int selected_frame_position = current_layer_obj -> framePosition( selected_frame );
    if ( selected_frame == NULL || selected_frame_position == MAX_NUMBER_OF_FRAMES - 1 )
        return;

    ESFrame *new_selected_frame = current_layer_obj -> frameAt( selected_frame_position + 1 );

    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event );
    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event2 );

    if ( scroll_area_container -> contentsToViewport( new_selected_frame -> pos() ).y() > scroll_area_container -> visibleHeight() )
        scroll_area_container -> scrollBy( 0, 200 );
}

void ExposureSheet::selectRight()
{
    ILayer *current_layer_obj = findCurrentLayerObj();
    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    int selected_frame_position = current_layer_obj -> framePosition( selected_frame );
    if ( selected_frame == NULL || current_layer_obj -> interfaceElement() == last_layer )
        return;

    ILayer *new_selected_layer = list_of_layers.at( list_of_layers.find( current_layer_obj ) + 1 );
    ESFrame *new_selected_frame = new_selected_layer -> frameAt( selected_frame_position );

    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event );
    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event2 );

    if ( scroll_area_container -> contentsToViewport( new_selected_frame -> pos() ).x() > scroll_area_container -> visibleWidth() )
        scroll_area_container -> scrollBy( 100, 0 );
}

void ExposureSheet::selectLeft()
{
    ILayer *current_layer_obj = findCurrentLayerObj();
    ESFrame *selected_frame = current_layer_obj -> selectedFrame();
    int selected_frame_position = current_layer_obj -> framePosition( selected_frame );
    if ( selected_frame == NULL || current_layer_obj -> interfaceElement() == first_layer )
        return;

    ILayer *new_selected_layer = list_of_layers.at( list_of_layers.find( current_layer_obj ) - 1 );
    ESFrame *new_selected_frame = new_selected_layer -> frameAt( selected_frame_position );

    QMouseEvent mouse_event( QEvent::MouseButtonPress, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event );
    QMouseEvent mouse_event2( QEvent::MouseButtonRelease, QPoint( 10, 10 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( new_selected_frame, &mouse_event2 );

    if ( scroll_area_container -> contentsToViewport( new_selected_frame -> pos() ).x() < 0 )
        scroll_area_container -> scrollBy( -100, 0 );
}
