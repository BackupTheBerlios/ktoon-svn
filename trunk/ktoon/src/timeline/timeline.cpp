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

#include "timeline.h"
#include "layer.h"
#include "keyframe.h"
#include "ktoon.h"

#include <qimage.h>

//--------------- CONSTRUCTOR --------------------

Timeline::Timeline( QWidget *parent, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )

	: KTDialogBase( QDockWindow::OutsideDock, parent, "Timeline")
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( in_assigned_menu );
    Q_CHECK_PTR( assig_tb_button );
    
    //Initializations
    setCaption( tr( "Timeline" ) );

    resize( 800, 150 );
    move( 20, 88 );
    parent_widget = parent;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
    k_toon = ( KToon * )parent_widget;

    //----------- Component Initialization --------------

    splitter = new QSplitter( this );
    splitter -> resize( 790, 130 );
    splitter -> move( 10, 5 );
    QValueList<int> sizes;
    sizes.append( 200 );
    sizes.append( 590 );
    splitter -> setSizes( sizes );

    layer_manager = new TLLayerManager( splitter, this );
    splitter -> setResizeMode( layer_manager, QSplitter::KeepSize );

    frame_sequence_manager =  new TLFrameSequenceManager( splitter, this );
    splitter -> setResizeMode( frame_sequence_manager, QSplitter::KeepSize );

    sound_label = new QLabel( tr( "Sound File-name:" ), this );
    sound_label -> setFont( QFont( font().family(), 7 ) );
    sound_label -> resize( layer_manager -> width(), 25 );
    sound_label -> move( splitter -> x(), splitter -> y() + splitter -> height() + 10 + 125 );

    sound_widget_container = new QFrame( this );
    sound_widget_container -> setFrameStyle( QFrame::Panel | QFrame::Sunken );
    sound_widget_container -> setLineWidth( 2 );
    sound_widget_container -> resize( frame_sequence_manager -> width(), 25 );
    sound_widget_container -> move( sound_label -> x() + sound_label -> width(), sound_label -> y() );

    sound_widget = new SoundWidget( sound_widget_container );

    //Connections of global signal and slots
    connect( frame_sequence_manager, SIGNAL( frameSelected( TLFrameSequence* ) ), layer_manager, SIGNAL( frameSelected( TLFrameSequence* ) ) );
    connect( layer_manager, SIGNAL( layerSelected( TLLayer* ) ), frame_sequence_manager, SIGNAL( layerSelected( TLLayer* ) ) );
    connect( layer_manager, SIGNAL( layerReleasedAbove( int, int ) ), frame_sequence_manager, SIGNAL( layerReleasedAbove( int, int ) ) );
    connect( layer_manager, SIGNAL( layerReleasedBelow( int, int ) ), frame_sequence_manager, SIGNAL( layerReleasedBelow( int, int ) ) );
    connect( layer_manager -> layerSequence(), SIGNAL( layerVisibilityChanged( int, bool ) ), frame_sequence_manager -> frameLayout(), SLOT( slotChangeLayerVisibility( int, bool ) ) );
    connect( frame_sequence_manager -> verticalScrollBar(), SIGNAL( valueChanged( int ) ), layer_manager -> verticalScrollBar(), SLOT( setValue( int ) ) );
    connect( layer_manager -> verticalScrollBar(), SIGNAL( valueChanged( int ) ), frame_sequence_manager -> verticalScrollBar(), SLOT( setValue( int ) ) );
    connect( layer_manager, SIGNAL( widthChanged( int ) ), SLOT( changeSoundLabelWidth( int ) ) );
    connect( frame_sequence_manager, SIGNAL( widthChanged( int ) ), SLOT( changeSoundContainerWidth( int ) ) );
    connect( frame_sequence_manager -> horizontalScrollBar(), SIGNAL( valueChanged( int ) ), sound_widget, SLOT( slotMoveSoundWidget( int ) ) );
    connect( frame_sequence_manager -> getRuler(), SIGNAL( offsetDragged( int ) ), sound_widget, SLOT( slotChangeDragOffset( int ) ) );
    connect( frame_sequence_manager -> getRuler(), SIGNAL( offsetChanged( int ) ), sound_widget, SLOT( slotChangeOffset( int ) ) );
    connect( frame_sequence_manager -> getRuler(), SIGNAL( offsetDragged( int ) ), SLOT( slotUpdateCurrentTime( int ) ) );
    connect( frame_sequence_manager -> getRuler(), SIGNAL( offsetChanged( int ) ), SLOT( slotUpdateCurrentTime( int ) ) );
    connect( frame_sequence_manager -> getRuler(), SIGNAL( maxOffsetChanged( int ) ), SLOT( slotUpdateTotalTime( int ) ) );
    
}

//-------------- DESTRUCTOR -----------------

Timeline::~Timeline()
{
    delete layer_manager;
    delete frame_sequence_manager;
    delete splitter;
    delete sound_label;
    delete sound_widget;
    delete sound_widget_container;
}

//----------------- PUBLIC MEMBERS -------------------

TLFrameSequenceManager *Timeline::frameSequenceManager()
{
    Q_CHECK_PTR( frame_sequence_manager );
    return frame_sequence_manager;
}

TLLayerManager *Timeline::layerManager()
{
    Q_CHECK_PTR( layer_manager );
    return layer_manager;
}

void Timeline::loadLayersAndKeyframes( QPtrList<Layer> layers )
{
    layer_manager -> layerSequence() -> loadLayers( layers );
    frame_sequence_manager -> frameLayout() -> loadFrames( layers );
}

void Timeline::loadSound( const QString &file_name )
{
    sound_label -> setText( tr( "Sound File-name:\n" ) + file_name );
}

int Timeline::exportAnimation( const QString &file_name, int iter_begin, const QString &format )
{
    int i, j;
    QString extension;

    if ( format == "PNG" )
        extension = ".png";
    else if ( format == "JPEG" )
        extension = ".jpg";
    else
        //TODO: Throw an exception
	;

    //layer_manager -> setAllLayersVisible( true );

    for ( i = iter_begin, j = 1; j <= frame_sequence_manager -> getRuler() -> getMaxOffset(); i++, j++ )
    {
        QString iterator = ( QString::number( i ) ).rightJustify( 3, '0' );
	QString new_file_name = file_name + iterator + extension;
	frame_sequence_manager -> getRuler() -> slotSetOffset( j );
	
	emit saveImage(new_file_name);
	
// 	QPixmap exp;
	//FIXME: Emitir el nombre del archivo y la extension y conectarlo con render camera para grabar
// 	exp.convertFromImage( k_toon -> renderCameraPreview() -> grabFrameBuffer() );
// 	if ( !exp.save( new_file_name, format.latin1() ) )
// 	{
// 	    qDebug( tr( "Could not save the file: %1" ).arg( new_file_name ) );
// 	    return 0;
// 	}
    }

    return i;
}

//------------------- SLOTS -------------------------

void Timeline::changeSoundLabelWidth( int width )
{
    sound_label -> resize( width + 5, sound_label -> height() );
    sound_widget_container -> move( sound_label -> x() + sound_label -> width(), sound_label -> y() );
}

void Timeline::changeSoundContainerWidth( int width )
{
    sound_widget_container -> resize( width, sound_widget_container -> height() );
}

void Timeline::slotUpdateCurrentTime( int offset )
{
    int frame_rate = KTStatus -> currentFrameRate();
    float new_time = ( float )( offset - 1 ) / ( float )frame_rate;
    layer_manager -> slotSetCurrentTime( new_time );
}

void Timeline::slotUpdateTotalTime( int max_offset )
{
    int frame_rate = KTStatus -> currentFrameRate();
    float new_time = ( float )( max_offset ) / ( float )frame_rate;
    layer_manager -> slotSetTotalTime( new_time );
}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

void Timeline::closeEvent( QCloseEvent *close_event )
{
    Q_CHECK_PTR( close_event );
    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}

void Timeline::resizeEvent( QResizeEvent *resize_event )
{
    Q_CHECK_PTR( resize_event );
    //Update the size of the splitter in order to fit into the dialog box
    QSize new_size = resize_event -> size();
    if ( frame_sequence_manager -> soundButton() -> isOn() )
    {
        new_size.setWidth( new_size.width() - 10 );
        new_size.setHeight( new_size.height() - 55 );
        splitter -> resize( new_size );
    }
    else
    {
        new_size.setWidth( new_size.width() - 10 );
        new_size.setHeight( new_size.height() - 10 );
        splitter -> resize( new_size );
    }
    sound_label -> move( splitter -> x(), splitter -> y() + splitter -> height() + 10 );
    sound_widget_container -> move( sound_widget_container -> x(), sound_label -> y() );
}

void Timeline::keyPressEvent( QKeyEvent *key_event )
{
    Q_CHECK_PTR( key_event );
    switch( key_event -> key() )
    {
	case Qt::Key_F5:
	    if ( key_event -> state() == Qt::ShiftButton )
	        frame_sequence_manager -> frameLayout() -> currentFrameSequence() -> slotRemoveFrame();
	    else
	        frame_sequence_manager -> frameLayout() -> currentFrameSequence() -> slotInsertFrame();
	    key_event -> accept();
	    break;
	/*case Qt::Key_F6: frame_sequence_manager -> frameLayout() -> currentFrameSequence() -> slotInsertKeyframe();
	    key_event -> accept();
	    break;*/
	case Qt::Key_M:
	    if ( key_event -> state() == Qt::ControlButton )
	    {
	        frame_sequence_manager -> frameLayout() -> currentFrameSequence() -> slotCreateMotionTween();
		key_event -> accept();
	    }
	    else if ( key_event -> state() == Qt::ShiftButton )
	    {
	        frame_sequence_manager -> frameLayout() -> currentFrameSequence() -> slotRemoveMotionTween();
		key_event -> accept();
	    }
	    else
	        key_event -> ignore();
	    break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
	    if ( frame_sequence_manager -> getRuler() -> isAnimationPlaying() )
	        frame_sequence_manager -> getRuler() -> slotStop();
	    else
	    {
	        if ( frame_sequence_manager -> getRuler() -> getOffset() == frame_sequence_manager -> getRuler() -> getMaxOffset() )
		    frame_sequence_manager -> getRuler() -> slotSetOffset( 1 );
	        frame_sequence_manager -> getRuler() -> slotPlay();
	    }
	    break;
	case Qt::Key_Period:
	    if ( key_event -> state() == Qt::ControlButton )
	    {
		frame_sequence_manager -> getRuler() -> slotSetOffset( frame_sequence_manager -> getRuler() -> getMaxOffset() );
	    }
	    else
	    {
	        frame_sequence_manager -> getRuler() -> slotStepForward();
	        key_event -> accept();
	    }
	    break;
	case Qt::Key_Comma:
	    if ( key_event -> state() == Qt::ControlButton )
	    {
		frame_sequence_manager -> getRuler() -> slotSetOffset( 1 );
	    }
	    else
	    {
	        frame_sequence_manager -> getRuler() -> slotStepBackward();
	        key_event -> accept();
	    }
	    break;
	default: key_event -> ignore();
	    break;
    }
}
