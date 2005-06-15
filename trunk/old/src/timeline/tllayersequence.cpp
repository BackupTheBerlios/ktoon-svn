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

#include "tllayersequence.h"
#include "../store/layer.h"
#include "../ktoon.h"

//--------------- CONSTRUCTOR --------------------

TLLayerSequence::TLLayerSequence( QWidget *parent )
    : QScrollView( parent )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    enableClipper( true );
    setHScrollBarMode( QScrollView::AlwaysOff );
    parent_widget = parent;
    number_of_layers = 1;
    layer_max_value = 1;
    dragging = false;
    dragged_layer = NULL;
    viewport() -> setMouseTracking( true );
    k_toon = ( Ktoon * )( parent -> parentWidget() -> parentWidget() -> parentWidget() );

    //Add a default layer
    TLLayer *default_layer = new TLLayer( 1, viewport(), this, tr( "Layer" ) + QString( "1" ) );
    default_layer -> resize( width(), 24 );
    connect( default_layer, SIGNAL( selected() ), SLOT( slotSelectLayer() ) );
    connect( default_layer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
    connect( default_layer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
    connect( default_layer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
    connect( default_layer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
    connect( default_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
    connect( default_layer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );
    addChild( default_layer );
    list_of_layers.append( default_layer );

    current_layer = default_layer;
    last_layer = current_layer;
    current_layer -> setSelected( true );
    current_layer -> setEdited( true );
}

//--------------- DESTRUCTOR --------------------

TLLayerSequence::~TLLayerSequence()
{

}

//-------------- PUBLIC MEMBERS ----------------

QPtrList<TLLayer> TLLayerSequence::listOfLayers()
{
    return list_of_layers;
}

TLLayer *TLLayerSequence::currentLayer()
{
    Q_CHECK_PTR( current_layer );
    return current_layer;
}

TLLayer *TLLayerSequence::lastLayer()
{
    Q_CHECK_PTR( last_layer );
    return last_layer;
}

int TLLayerSequence::numberOfLayers()
{
    return number_of_layers;
}

TLLayer *TLLayerSequence::layerAt( int pos )
{
    if ( pos > number_of_layers || pos < 1 )
	return NULL;

    return list_of_layers.at( pos - 1 );
}

void TLLayerSequence::clearDraggedLayers()
{
    TLLayer *layer_iterator;
    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
        layer_iterator -> setInDragWay( false );
}

void TLLayerSequence::loadLayers( QPtrList<Layer> layers )
{
    layer_max_value = 0;
    number_of_layers = 0;
    delete current_layer;
    list_of_layers.clear();

    Layer *l_it;
    for ( l_it = layers.first(); l_it; l_it = layers.next() )
    {
        layer_max_value++;
        number_of_layers++;

	TLLayer *new_layer = new TLLayer( number_of_layers, viewport(), this, l_it -> nameLayer() );
	if ( l_it == layers.getFirst() )
	    current_layer = new_layer;
        new_layer -> resize( width(), 24 );
        connect( new_layer, SIGNAL( selected() ), SLOT( slotSelectLayer() ) );
        connect( new_layer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
        connect( new_layer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
        connect( new_layer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
        connect( new_layer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
        connect( new_layer, SIGNAL( renamed( const QString& ) ), SLOT( slotRenameLayer( const QString & ) ) );
        connect( new_layer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );
        addChild( new_layer, 0, 24 * ( number_of_layers - 1 ) );
        last_layer = new_layer;
        list_of_layers.append( new_layer );
    }
    updateContentSize();
}

void TLLayerSequence::setDragging( bool drag )
{
    dragging = drag;
}

//-------------- SLOTS --------------------

void TLLayerSequence::slotInsertLayer()
{
    QString layer_number;
    layer_max_value++;
    number_of_layers++;
    layer_number.setNum( layer_max_value );

    TLLayer *new_layer = new TLLayer( number_of_layers, viewport(), this, tr( "Layer" ) + layer_number );
    new_layer -> resize( width(), 24 );
    connect( new_layer, SIGNAL( selected() ), SLOT( slotSelectLayer() ) );
    connect( new_layer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
    connect( new_layer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
    connect( new_layer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
    connect( new_layer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
    connect( new_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
    connect( new_layer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );
    addChild( new_layer, 0, 24 * ( number_of_layers - 1 ) );
    new_layer -> show();
    last_layer = new_layer;
    list_of_layers.append( new_layer );
    updateContentSize();
}

void TLLayerSequence::slotRemoveLayer()
{
   if ( number_of_layers > 1 )
   {
      number_of_layers--;
      TLLayer *bridge_layer;

      //Case 1: When the layer is the last within the list of layers
      if ( current_layer == last_layer )
      {
	  list_of_layers.remove( current_layer );
	  bridge_layer = list_of_layers.getLast();
	  delete current_layer;
	  current_layer = bridge_layer;
	  last_layer = current_layer;
	  current_layer -> setSelected( true );
	  current_layer -> setEdited( true );
      }
      //Case 2: When the layer is any except the last
      else
      {
	  bridge_layer = list_of_layers.at( list_of_layers.find( current_layer ) + 1 );

	  //Reaccomodate every layer next to the layer that is going to be deleted
	  TLLayer *layer_iterator;
	  for ( layer_iterator = bridge_layer; layer_iterator; layer_iterator = list_of_layers.next() )
	  {
	      moveChild( layer_iterator, childX( layer_iterator ), childY( layer_iterator ) - layer_iterator -> height() );
	      layer_iterator -> setPosition( layer_iterator -> position() - 1 );
	  }

	  list_of_layers.remove( current_layer );
	  delete current_layer;
	  current_layer = bridge_layer;
	  current_layer -> setSelected( true );
	  current_layer -> setEdited( true );
      }
      updateContentSize();
   }
}

void TLLayerSequence::slotSelectLayer()
{
    TLLayer *selected_layer = ( TLLayer * )sender();
    current_layer -> clearTextfieldFocus();
    current_layer -> setSelected( false );
    current_layer -> setEdited( false );
    current_layer = selected_layer;
    current_layer -> setSelected( true );
    current_layer -> setEdited( true );

    emit layerSelectedToES( list_of_layers.find( current_layer ) );
    emit layerSelected( current_layer );
}

void TLLayerSequence::slotRenameLayer( const QString &name )
{
    TLLayer *renamed_layer = ( TLLayer * )sender();
    int pos = list_of_layers.find( renamed_layer );
    emit layerRenamed( pos, name );
}

void TLLayerSequence::slotChangeVisibilityState( bool state )
{
    TLLayer *changed_layer = ( TLLayer * )sender();
    int pos = list_of_layers.find( changed_layer );
    emit layerVisibilityChanged( pos, state );
}

void TLLayerSequence::slotSelectLayerFromFrameSelected( TLFrameSequence *fs )
{
    Q_CHECK_PTR( fs );
    
    //Find the layer at the position of the frame sequence
    TLLayer *layer_iterator, *layer_to_select;

    layer_to_select = NULL;

    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
	if ( layer_iterator -> position() == fs -> position() )
	{
	    layer_to_select = layer_iterator;
	    break;
	}
    }

    current_layer -> clearTextfieldFocus();
    current_layer -> setSelected( false );
    current_layer -> setEdited( false );
    current_layer = layer_to_select;
    current_layer -> setSelected( true );
    current_layer -> setEdited( true );

    emit layerSelectedToES( list_of_layers.find( current_layer ) );
}

void TLLayerSequence::slotSelectLayerFromES( int pos )
{
    TLLayer *layer_iterator, *layer_to_select;

    layer_to_select = NULL;

    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
    {
	if ( layer_iterator -> position() == pos )
	{
	    layer_to_select = layer_iterator;
	    break;
	}
    }

    current_layer -> clearTextfieldFocus();
    current_layer -> setSelected( false );
    current_layer -> setEdited( false );
    current_layer = layer_to_select;
    current_layer -> setSelected( true );
    current_layer -> setEdited( true );

    emit layerSelected( current_layer );
}

void TLLayerSequence::slotRenameLayerFromES( int pos, const QString &name )
{
    TLLayer *layer_to_rename = list_of_layers.at( pos );
    layer_to_rename -> setName( name );
}

void TLLayerSequence::slotMoveLayerUp()
{
    //If the current layer is the first, do nothing
    if ( current_layer == list_of_layers.getFirst() )
        return;

    //Find the layer above the current and reinsert it into the list in its new position
    TLLayer *layer_above = list_of_layers.take( list_of_layers.find( current_layer ) - 1 );
    current_layer -> setPosition( current_layer -> position() - 1 );
    layer_above -> setPosition( layer_above -> position() + 1 );
    list_of_layers.insert( layer_above -> position() - 1, layer_above );
    if ( current_layer == last_layer )
        last_layer = layer_above;

    //Swap both layers in the scroll view
    moveChild( layer_above, childX( layer_above ), childY( layer_above ) + 24 );
    moveChild( current_layer, childX( current_layer ), childY( current_layer ) - 24 );
    updateContentSize();

    k_toon -> renderCameraPreview() -> updateGL();
}

void TLLayerSequence::slotMoveLayerDown()
{
    //If the current_layer is the last, do nothing
    if ( current_layer == last_layer )
        return;

    //Find the layer above the current and reinsert it into the list in its new position
    TLLayer *layer_below = list_of_layers.take( list_of_layers.find( current_layer ) + 1 );
    current_layer -> setPosition( current_layer -> position() + 1 );
    layer_below -> setPosition( layer_below -> position() - 1 );
    list_of_layers.insert( layer_below -> position() - 1, layer_below );
    if ( layer_below == last_layer )
        last_layer = current_layer;

    //Swap both layers in the scroll view
    moveChild( layer_below, childX( layer_below ), childY( layer_below ) - 24 );
    moveChild( current_layer, childX( current_layer ), childY( current_layer ) + 24 );
    updateContentSize();

    k_toon -> renderCameraPreview() -> updateGL();
}

void TLLayerSequence::slotDragLayerAbove( int y )
{
    TLLayer *drag_layer = layerAt( mapToPosition( y ) );
    clearDraggedLayers();
    if ( drag_layer == NULL )
        return;

    if ( y < 0 )
	scrollBy( 0, ( drag_layer -> height() ) * -1 );

    drag_layer -> setInDragWay( true );
}

void TLLayerSequence::slotDragLayerBelow( int y )
{
    TLLayer *drag_layer = layerAt( mapToPosition( y ) );
    clearDraggedLayers();
    if ( drag_layer == NULL )
        return;

    if ( y > drag_layer -> height() )
	scrollBy( 0, drag_layer -> height() );

    drag_layer -> setInDragWay( true );
}

void TLLayerSequence::slotReleaseLayerAbove( int y )
{
    TLLayer *release_layer = layerAt( mapToPosition( y ) );
    clearDraggedLayers();
    if ( release_layer == NULL )
        return;

    //---------- Swap the current layer and the release layer in both the list and scroll view ----------

    int cl_old_pos, rl_old_pos;
    cl_old_pos = current_layer -> position();
    rl_old_pos = release_layer -> position();

    emit layerReleasedAbove( cl_old_pos, rl_old_pos );

    current_layer = list_of_layers.take( list_of_layers.find( current_layer ) );
    release_layer = list_of_layers.take( list_of_layers.find( release_layer ) );
    current_layer -> setPosition( rl_old_pos );
    release_layer -> setPosition( cl_old_pos );
    list_of_layers.insert( current_layer -> position() - 1, current_layer );
    list_of_layers.insert( release_layer -> position() - 1, release_layer );
    if ( current_layer == last_layer )
        last_layer = list_of_layers.getLast();

    int cl_old_x, cl_old_y, rl_old_x, rl_old_y;
    cl_old_x = childX( current_layer );
    cl_old_y = childY( current_layer );
    rl_old_x = childX( release_layer );
    rl_old_y = childY( release_layer );
    moveChild( current_layer, rl_old_x, rl_old_y );
    moveChild( release_layer, cl_old_x, cl_old_y );
    updateContentSize();

    k_toon -> renderCameraPreview() -> updateGL();
}

void TLLayerSequence::slotReleaseLayerBelow( int y )
{
    TLLayer *release_layer = layerAt( mapToPosition( y ) );
    clearDraggedLayers();
    if ( release_layer == NULL )
        return;

    //----------- Swap the current layer and the release layer in both the list and scroll view ------

    int cl_old_pos, rl_old_pos;
    cl_old_pos = current_layer -> position();
    rl_old_pos = release_layer -> position();

    emit layerReleasedBelow( cl_old_pos, rl_old_pos );

    current_layer = list_of_layers.take( list_of_layers.find( current_layer ) );
    release_layer = list_of_layers.take( list_of_layers.find( release_layer ) );
    current_layer -> setPosition( rl_old_pos );
    release_layer -> setPosition( cl_old_pos );
    list_of_layers.insert( release_layer -> position() - 1, release_layer );
    list_of_layers.insert( current_layer -> position() - 1, current_layer );
    if ( release_layer == last_layer )
        last_layer = list_of_layers.getLast();

    int cl_old_x, cl_old_y, rl_old_x, rl_old_y;
    cl_old_x = childX( current_layer );
    cl_old_y = childY( current_layer );
    rl_old_x = childX( release_layer );
    rl_old_y = childY( release_layer );
    moveChild( current_layer, rl_old_x, rl_old_y );
    moveChild( release_layer, cl_old_x, cl_old_y );
    updateContentSize();

    k_toon -> renderCameraPreview() -> updateGL();
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLLayerSequence::resizeEvent( QResizeEvent *resize_event )
{
    Q_CHECK_PTR( resize_event );
    QSize new_size = resize_event -> size();

    QScrollView::resizeEvent( resize_event );

    TLLayer *layer_iterator;
    for ( layer_iterator = list_of_layers.first(); layer_iterator; layer_iterator = list_of_layers.next() )
	layer_iterator -> resize( new_size.width(), layer_iterator -> height() );

    updateContentSize();
}

void TLLayerSequence::updateContentSize()
{
    if ( contentsHeight() < number_of_layers * 24 )
    	resizeContents( contentsWidth(), number_of_layers * 24 );
    else if ( contentsHeight() > number_of_layers * 24 && contentsHeight() > 90 )
        resizeContents( contentsWidth(), contentsHeight() - 24 );

    updateContents();
    updateScrollBars();
    parent_widget -> update();
}

int TLLayerSequence::mapToPosition( int y )
{
    if ( y < 0 )
	return current_layer -> position() + ( y + 1 ) / 24 - 1;
    else
	return current_layer -> position() + y / 24 + 1;
}
