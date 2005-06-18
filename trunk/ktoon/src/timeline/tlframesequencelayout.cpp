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

#include "tlframesequencelayout.h"
#include "tllayer.h"
#include "ktoon.h"

//--------------- CONSTRUCTOR --------------------

TLFrameSequenceLayout::TLFrameSequenceLayout( QWidget *parent )
    : QScrollView( parent )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    enableClipper( true );
    parent_widget = parent;
    setHScrollBarMode( QScrollView::AlwaysOff );
    number_of_frame_sequences = 1;
    max_used_frames = 1;
    current_frame = NULL;
    k_toon = ( KToon * )( parent_widget -> parentWidget() -> parentWidget() -> parentWidget() );

    //Add a default frame sequence to the default layer
    TLFrameSequence *default_frame_sequence = new TLFrameSequence( 1, viewport(), this );
    addChild( default_frame_sequence );
    connect( default_frame_sequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
    connect( default_frame_sequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
    connect( default_frame_sequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
    connect( default_frame_sequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
    connect( default_frame_sequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
    list_of_frame_sequences.append( default_frame_sequence );
    current_frame_sequence = default_frame_sequence;
    last_frame_sequence = default_frame_sequence;
}

//--------------- DESTRUCTOR --------------------

TLFrameSequenceLayout::~TLFrameSequenceLayout()
{

}

//-------------- PUBLIC MEMBERS ----------------

void TLFrameSequenceLayout::moveFrames( int x )
{
    //Move each frame sequence
    TLFrameSequence *frame_sequence_iterator;
    for ( frame_sequence_iterator = list_of_frame_sequences.first(); frame_sequence_iterator; frame_sequence_iterator = list_of_frame_sequences.next() )
    {
	moveChild( frame_sequence_iterator, x, childY( frame_sequence_iterator ) );
    }
}

QPtrList<TLFrameSequence> TLFrameSequenceLayout::listOfFrameSequences()
{
    return list_of_frame_sequences;
}

TLFrame *TLFrameSequenceLayout::currentFrame()
{
    return current_frame;
}

TLFrameSequence *TLFrameSequenceLayout::currentFrameSequence()
{
    Q_CHECK_PTR( current_frame_sequence );
    return current_frame_sequence;
}

void TLFrameSequenceLayout::loadFrames( QPtrList<Layer> layers )
{
    number_of_frame_sequences = 0;
    max_used_frames = 0;
    delete current_frame_sequence;
    list_of_frame_sequences.clear();

    Layer *l_it;
    for ( l_it = layers.first(); l_it; l_it = layers.next() )
    {
	QPtrList<KeyFrame> keyframes = l_it -> keyFrames();
        number_of_frame_sequences++;

    	TLFrameSequence *new_frame_sequence = new TLFrameSequence( number_of_frame_sequences, viewport(), this );
	if ( l_it == layers.getFirst() )
	    current_frame_sequence = new_frame_sequence;
    	addChild( new_frame_sequence, 0, ( number_of_frame_sequences - 1 ) * new_frame_sequence -> height() );
    	connect( new_frame_sequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
    	connect( new_frame_sequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
    	connect( new_frame_sequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
    	connect( new_frame_sequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
    	connect( new_frame_sequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
	list_of_frame_sequences.append( new_frame_sequence );
    	last_frame_sequence = new_frame_sequence;
    	updateContentSize();

	new_frame_sequence -> loadFrames( keyframes );
    }
}

//-------------- SLOTS --------------------

void TLFrameSequenceLayout::slotSelectFrame()
{
    TLFrame *selected_frame = ( TLFrame * )sender();
    if ( current_frame != NULL )
        current_frame -> setSelected( false );
    current_frame = selected_frame;
    current_frame_sequence = ( TLFrameSequence * )selected_frame -> parentWidget();
    current_frame -> setSelected( true );

    emit frameSelected( current_frame_sequence );

    TLFrameSequenceManager *fsm = ( TLFrameSequenceManager * )( parent_widget );
    fsm -> getRuler() -> slotSetOffset( current_frame_sequence -> framePosition( selected_frame ) );
}

void TLFrameSequenceLayout::slotInsertFrameSequence()
{
    number_of_frame_sequences++;
    TLFrameSequence *new_frame_sequence = new TLFrameSequence( number_of_frame_sequences, viewport(), this );
    addChild( new_frame_sequence, 0, ( number_of_frame_sequences - 1 ) * new_frame_sequence -> height() );
    connect( new_frame_sequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
    connect( new_frame_sequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
    connect( new_frame_sequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
    connect( new_frame_sequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
    connect( new_frame_sequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
    new_frame_sequence -> show();
    list_of_frame_sequences.append( new_frame_sequence );
    last_frame_sequence = new_frame_sequence;
    updateContentSize();
}

void TLFrameSequenceLayout::slotRemoveFrameSequence()
{
   if ( number_of_frame_sequences > 1 )
   {
      TLFrameSequence *bridge_frame_sequence;
      number_of_frame_sequences--;

      //Case 1: When the sequence of frames is the last within the list
      if ( current_frame_sequence == last_frame_sequence )
      {
	  list_of_frame_sequences.remove( current_frame_sequence );
	  bridge_frame_sequence = list_of_frame_sequences.getLast();
	  delete current_frame_sequence;
	  current_frame_sequence = bridge_frame_sequence;
	  last_frame_sequence = current_frame_sequence;
	  current_frame = NULL;
      }
      //Case 2: When the sequence of frames is any except the last
      else
      {
	  bridge_frame_sequence = list_of_frame_sequences.at( list_of_frame_sequences.find( current_frame_sequence ) + 1 );

	  //Reaccomodate every frame_sequence next to the frame_sequence that is going to be deleted
	  TLFrameSequence *frame_sequence_iterator;
	  for ( frame_sequence_iterator = bridge_frame_sequence; frame_sequence_iterator; frame_sequence_iterator = list_of_frame_sequences.next() )
	  {
	      moveChild( frame_sequence_iterator, childX( frame_sequence_iterator ), childY( frame_sequence_iterator ) - frame_sequence_iterator -> height() );
	      frame_sequence_iterator -> setPosition( frame_sequence_iterator -> position() - 1 );
	  }

	  list_of_frame_sequences.remove( current_frame_sequence );
	  delete current_frame_sequence;
	  current_frame_sequence = bridge_frame_sequence;
	  current_frame = NULL;
      }
      updateContentSize();
   }
}

void TLFrameSequenceLayout::slotConsequencesOfALayerSelection( TLLayer *selected_layer )
{
    Q_CHECK_PTR( selected_layer );
    if ( current_frame != NULL && current_frame_sequence -> position() != selected_layer -> position() )
    {
    	current_frame -> setSelected( false );
    	current_frame = NULL;
    }

    //Set the current frame sequence
    TLFrameSequence *fs_iterator;
    for ( fs_iterator = list_of_frame_sequences.first(); fs_iterator; fs_iterator = list_of_frame_sequences.next() )
    {
	if ( fs_iterator -> position() == selected_layer -> position() )
	{
	    current_frame_sequence = fs_iterator;
	    break;
	}
    }
}

void TLFrameSequenceLayout::slotUpdateMaxUsedFrames()
{
    TLFrameSequence *fs_iterator;

    fs_iterator = list_of_frame_sequences.first();
    max_used_frames = fs_iterator -> numberOfUsedFrames();

    for ( ; fs_iterator; fs_iterator = list_of_frame_sequences.next() )
    {
        if ( fs_iterator -> numberOfUsedFrames() > max_used_frames )
	    max_used_frames = fs_iterator -> numberOfUsedFrames();
    }

    emit maxUsedFramesUpdated( max_used_frames );
}

void TLFrameSequenceLayout::slotChangeOffset( int current_offset )
{
	qDebug(tr("Changing offset to %1").arg(current_offset));
    TLFrameSequence *fs_iterator;
    Layer *l_it;
    KeyFrame *to_add;
    QPtrList<KeyFrame> kf, to_display;
    QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
    Timeline *tl = ( Timeline * )( parent_widget -> parentWidget() -> parentWidget() );
    QPtrList<TLLayer> layers = tl -> layerManager() -> layerSequence() -> listOfLayers();
    TLLayer *tll_it;
    for ( tll_it = layers.first(), l_it = ly.first(), fs_iterator = list_of_frame_sequences.first(); fs_iterator; tll_it = layers.next(), l_it = ly.next(), fs_iterator = list_of_frame_sequences.next() )
    {
    	fs_iterator -> changeOffsetFrame( current_offset );
	kf = l_it -> keyFrames();
	if ( fs_iterator -> offsetFrame() -> isUsed() && tll_it -> isVisible() )
	{
	    to_add = kf.at( fs_iterator -> keyframePosition( fs_iterator -> offsetFrame() ) - 1 );
	    to_display.append( to_add );
	}
    }
    KTStatus -> setRenderKeyframes( to_display );
    k_toon -> renderCameraPreview() -> updateGL();
}

void TLFrameSequenceLayout::slotChangeDragOffset( int current_offset )
{
    TLFrameSequence *fs_iterator;
    Layer *l_it;
    KeyFrame *to_add;
    QPtrList<KeyFrame> kf, to_display;
    QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
    Timeline *tl = ( Timeline * )( parent_widget -> parentWidget() -> parentWidget() );
    QPtrList<TLLayer> layers = tl -> layerManager() -> layerSequence() -> listOfLayers();
    TLLayer *tll_it;
    for ( tll_it = layers.first(), l_it = ly.first(), fs_iterator = list_of_frame_sequences.first(); fs_iterator; tll_it = layers.next(), l_it = ly.next(), fs_iterator = list_of_frame_sequences.next() )
    {
    	fs_iterator -> changeDragOffsetFrame( current_offset );
	kf = l_it -> keyFrames();
	if ( fs_iterator -> offsetFrame() -> isUsed() && tll_it -> isVisible() )
	{
	    to_add = kf.at( fs_iterator -> keyframePosition( fs_iterator -> offsetFrame() ) - 1 );
	    to_display.append( to_add );
	}
    }
    KTStatus -> setRenderKeyframes( to_display );
    k_toon -> renderCameraPreview() -> updateGL();
}

void TLFrameSequenceLayout::slotMoveFrameSequenceUp()
{
    //If the current frame sequence is the first, do nothing
    if ( current_frame_sequence == list_of_frame_sequences.getFirst() )
        return;

    //Find the frame sequence above the current and reinsert it into the list in its new position
    TLFrameSequence *frame_sequence_above = list_of_frame_sequences.take( list_of_frame_sequences.find( current_frame_sequence ) - 1 );
    current_frame_sequence -> setPosition( current_frame_sequence -> position() - 1 );
    frame_sequence_above -> setPosition( frame_sequence_above -> position() + 1 );
    list_of_frame_sequences.insert( frame_sequence_above -> position() - 1, frame_sequence_above );
    if ( current_frame_sequence == last_frame_sequence )
        last_frame_sequence = frame_sequence_above;

    //Swap both frame_sequences in the scroll view
    moveChild( frame_sequence_above, childX( frame_sequence_above ), childY( frame_sequence_above ) + 24 );
    moveChild( current_frame_sequence, childX( current_frame_sequence ), childY( current_frame_sequence ) - 24 );
    updateContentSize();
}

void TLFrameSequenceLayout::slotMoveFrameSequenceDown()
{
    //If the current frame sequence is the last, do nothing
    if ( current_frame_sequence == last_frame_sequence )
        return;

    //Find the frame sequence above the current and reinsert it into the list in its new position
    TLFrameSequence *frame_sequence_below = list_of_frame_sequences.take( list_of_frame_sequences.find( current_frame_sequence ) + 1 );
    current_frame_sequence -> setPosition( current_frame_sequence -> position() + 1 );
    frame_sequence_below -> setPosition( frame_sequence_below -> position() - 1 );
    list_of_frame_sequences.insert( frame_sequence_below -> position() - 1, frame_sequence_below );
    if ( frame_sequence_below == last_frame_sequence )
        last_frame_sequence = current_frame_sequence;

    //Swap both frame sequences in the scroll view
    moveChild( frame_sequence_below, childX( frame_sequence_below ), childY( frame_sequence_below ) - 24 );
    moveChild( current_frame_sequence, childX( current_frame_sequence ), childY( current_frame_sequence ) + 24 );
    updateContentSize();
}

void TLFrameSequenceLayout::slotSwapFrameSequences( int c_pos, int r_pos )
{
    Q_ASSERT( c_pos > 0 && r_pos > 0 );
    TLFrameSequence *release_frame_sequence;

    release_frame_sequence = NULL;

    //Above
    if ( c_pos > r_pos )
    {
        current_frame_sequence = list_of_frame_sequences.take( list_of_frame_sequences.find( current_frame_sequence ) );
        release_frame_sequence = list_of_frame_sequences.take( r_pos - 1 );
        current_frame_sequence -> setPosition( r_pos );
        release_frame_sequence -> setPosition( c_pos );
        list_of_frame_sequences.insert( current_frame_sequence -> position() - 1, current_frame_sequence );
        list_of_frame_sequences.insert( release_frame_sequence -> position() - 1, release_frame_sequence );
    }
    //Below
    else if ( c_pos < r_pos )
    {
        release_frame_sequence = list_of_frame_sequences.take( r_pos - 1 );
        current_frame_sequence = list_of_frame_sequences.take( list_of_frame_sequences.find( current_frame_sequence ) );
        current_frame_sequence -> setPosition( r_pos );
        release_frame_sequence -> setPosition( c_pos );
        list_of_frame_sequences.insert( release_frame_sequence -> position() - 1, release_frame_sequence );
        list_of_frame_sequences.insert( current_frame_sequence -> position() - 1, current_frame_sequence );
    }
    last_frame_sequence = list_of_frame_sequences.getLast();

    int cfs_old_x, cfs_old_y, rfs_old_x, rfs_old_y;
    cfs_old_x = childX( current_frame_sequence );
    cfs_old_y = childY( current_frame_sequence );
    rfs_old_x = childX( release_frame_sequence );
    rfs_old_y = childY( release_frame_sequence );
    moveChild( current_frame_sequence, rfs_old_x, rfs_old_y );
    moveChild( release_frame_sequence, cfs_old_x, cfs_old_y );
    updateContentSize();
}

void TLFrameSequenceLayout::slotInsertKeyframesIntoTheCurrentFS( int number )
{
    Q_ASSERT( number > 0 );
    if ( current_frame != NULL )
        current_frame -> setSelected( false );
    current_frame = NULL;

    for ( int i = 1; i <= number; i++ )
        current_frame_sequence -> slotInsertKeyframe();
}

void TLFrameSequenceLayout::slotRemoveKeyframeBlockFromTheCurrentFS( int key )
{
    current_frame_sequence -> slotRemoveKeyframeBlock( key );
}

void TLFrameSequenceLayout::slotMoveKeyframeLeftInTheCurrentFS( int key )
{
    current_frame_sequence -> slotMoveKeyframeLeft( key );
}

void TLFrameSequenceLayout::slotMoveKeyframeRightInTheCurrentFS( int key )
{
    current_frame_sequence -> slotMoveKeyframeRight( key );
}

void TLFrameSequenceLayout::slotChangeLayerVisibility( int, bool )
{
    TLFrameSequence *fs_iterator;
    Layer *l_it;
    KeyFrame *to_add;
    QPtrList<KeyFrame> kf, to_display;
    QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
    Timeline *tl = ( Timeline * )( parent_widget -> parentWidget() -> parentWidget() );
    QPtrList<TLLayer> layers = tl -> layerManager() -> layerSequence() -> listOfLayers();
    TLLayer *tll_it;
    for ( tll_it = layers.first(), l_it = ly.first(), fs_iterator = list_of_frame_sequences.first(); fs_iterator; tll_it = layers.next(), l_it = ly.next(), fs_iterator = list_of_frame_sequences.next() )
    {
	kf = l_it -> keyFrames();
	if ( fs_iterator -> offsetFrame() -> isUsed() && tll_it -> isVisible() )
	{
	    to_add = kf.at( fs_iterator -> keyframePosition( fs_iterator -> offsetFrame() ) - 1 );
	    to_display.append( to_add );
	}
    }
    KTStatus -> setRenderKeyframes( to_display );
    k_toon -> renderCameraPreview() -> updateGL();
}

void TLFrameSequenceLayout::slotFrameHasDrawing( bool state )
{
    KeyFrame *ckf = KTStatus -> currentKeyFrame();
    Layer *cl = KTStatus -> currentLayer();
    QPtrList<KeyFrame> kf = cl -> keyFrames();
    int kf_pos = kf.find( ckf ) + 1;
    current_frame_sequence -> setKeyframeHasDrawing( kf_pos, state );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLFrameSequenceLayout::resizeEvent( QResizeEvent *resize_event )
{
    Q_CHECK_PTR( resize_event );
    QSize new_size = resize_event -> size();

    QScrollView::resizeEvent( resize_event );

    updateContentSize();
}

void TLFrameSequenceLayout::updateContentSize()
{
    if ( contentsHeight() < number_of_frame_sequences * 24 )
    {
    	resizeContents( contentsWidth(), number_of_frame_sequences * 24 );
    }
    else if ( contentsHeight() > number_of_frame_sequences * 24 && contentsHeight() > 90 )
    {
        resizeContents( contentsWidth(), contentsHeight() - 24 );
    }
    updateContents();
    updateScrollBars();
    parent_widget -> update();
}
