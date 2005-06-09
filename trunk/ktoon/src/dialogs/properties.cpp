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

#include <qvalidator.h>
#include <qcolordialog.h>

#include "properties.h"
#include "ktoon.h"

//--------------- CONSTRUCTOR --------------------

Properties::Properties( QWidget *parent ) : QDialog( parent, "Animation Properties", true )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    setCaption( tr( "Animation Properties" ) );
//     setFont( QFont( "helvetica", 10 ) );
//     setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    parent_widget = parent;
    k_toon = ( KToon * )parent_widget;
    c_background = k_toon -> currentStatus() -> currentBackgroundColor();
    c_grid = k_toon -> currentStatus() -> currentGridColor();
    c_ntsc = k_toon -> currentStatus() -> currentNTSCColor();

    //------------- Operations on the static texts ----------

    text_frame_rate = new QLabel( tr( "Frame Rate:" ), this );
    text_frame_rate -> resize( 130, 20 );
    text_frame_rate -> move( 10, 10 );

    text_camera_size = new QLabel( tr( "Camera Size:" ), this );
    text_camera_size -> resize( 130, 20 );
    text_camera_size -> move( 10, text_frame_rate -> y() + text_frame_rate -> height() + 10 );

    text_background_color = new QLabel( tr( "Background Color:" ), this );
    text_background_color -> resize( 130, 20 );
    text_background_color -> move( 10, text_camera_size -> y() + text_camera_size -> height() + 10 );

    text_grid_color = new QLabel( tr( "Grid Color:" ), this );
    text_grid_color -> resize( 130, 20 );
    text_grid_color -> move( 10, text_background_color -> y() + text_background_color -> height() + 10 );

    text_ntsc_color = new QLabel( tr( "NTSC Zone Color:" ), this );
    text_ntsc_color -> resize( 130, 20 );
    text_ntsc_color -> move( 10, text_grid_color -> y() + text_grid_color -> height() + 10 );

    //------------- Operations on the Textfields -------------

    value_frame_rate = new QLineEdit( "", this );
    value_frame_rate -> resize( 60, 20 );
    value_frame_rate -> move( text_frame_rate -> x() + text_frame_rate -> width() + 10, text_frame_rate -> y() );
    value_frame_rate -> setValidator( new QIntValidator( 1, 999, this ) );
    value_frame_rate -> setText( QString::number( k_toon -> currentStatus() -> currentFrameRate() ) );

    value_camera_size_width = new QLineEdit( "", this );
    value_camera_size_width -> resize( 60, 20 );
    value_camera_size_width -> move( text_camera_size -> x() + text_camera_size -> width() + 10, text_camera_size -> y() );
    value_camera_size_width -> setValidator( new QIntValidator( 1, 9999, this ) );
    value_camera_size_width -> setText( QString::number( k_toon -> currentStatus() -> currentCameraWidth() ) );

    value_camera_size_height = new QLineEdit( "", this );
    value_camera_size_height -> resize( 60, 20 );
    value_camera_size_height -> move( value_camera_size_width -> x() + value_camera_size_width -> width() + 10, value_camera_size_width -> y() );
    value_camera_size_height -> setValidator( new QIntValidator( 1, 9999, this ) );
    value_camera_size_height -> setText( QString::number( k_toon -> currentStatus() -> currentCameraHeight() ) );

    value_background_color = new QLineEdit( c_background.name(), this );
    value_background_color -> resize( 60, 20 );
    value_background_color -> move( text_background_color -> x() + text_background_color -> width() + 10, text_background_color -> y() );
    connect( value_background_color, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeBackgroundColor( const QString & ) ) );

    value_grid_color = new QLineEdit( c_grid.name(), this );
    value_grid_color -> resize( 60, 20 );
    value_grid_color -> move( text_grid_color -> x() + text_grid_color -> width() + 10, text_grid_color -> y() );
    connect( value_grid_color, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeGridColor( const QString & ) ) );

    value_ntsc_color = new QLineEdit( c_ntsc.name(), this );
    value_ntsc_color -> resize( 60, 20 );
    value_ntsc_color -> move( text_ntsc_color -> x() + text_ntsc_color -> width() + 10, text_ntsc_color -> y() );
    connect( value_ntsc_color, SIGNAL( textChanged( const QString & ) ), SLOT( slotChangeNTSCColor( const QString & ) ) );

    //------------- Operations on the buttons -----------------


    background_color = new QPushButton( "", this );
    background_color -> resize( 20, 20 );
    background_color -> move( value_background_color -> x() + value_background_color -> width() + 10, value_background_color -> y() );
    background_color -> setAutoDefault( false );
    background_color -> setPaletteBackgroundColor( c_background );
    connect( background_color, SIGNAL( clicked() ), SLOT( slotBackgroundColor() ) );

    grid_color = new QPushButton( "", this );
    grid_color -> resize( 20, 20 );
    grid_color -> move( value_grid_color -> x() + value_grid_color -> width() + 10, value_grid_color -> y() );
    grid_color -> setAutoDefault( false );
    grid_color -> setPaletteBackgroundColor( c_grid );
    connect( grid_color, SIGNAL( clicked() ), SLOT( slotGridColor() ) );

    ntsc_color = new QPushButton( "", this );
    ntsc_color -> resize( 20, 20 );
    ntsc_color -> move( value_ntsc_color -> x() + value_ntsc_color -> width() + 10, value_ntsc_color -> y() );
    ntsc_color -> setAutoDefault( false );
    ntsc_color -> setPaletteBackgroundColor( c_ntsc );
    connect( ntsc_color, SIGNAL( clicked() ), SLOT( slotNTSCColor() ) );

    accept = new QPushButton( tr( "Accept" ), this );
    accept -> resize( 60, 30 );
    accept -> move( value_camera_size_width -> x(), value_ntsc_color -> y() + value_ntsc_color -> height() + 10 );
    connect( accept, SIGNAL( clicked() ), SLOT( slotAccept() ) );

    cancel = new QPushButton( tr( "Cancel" ), this );
    cancel -> resize( 60, 30 );
    cancel -> move( value_camera_size_height -> x(), value_ntsc_color -> y() + value_ntsc_color -> height() + 10 );
    connect( cancel, SIGNAL( clicked() ), SLOT( slotCancel() ) );
}

//-------------- DESTRUCTOR -----------------

Properties::~Properties()
{
    delete accept;
    delete cancel;
    delete background_color;
    delete grid_color;
    delete ntsc_color;
    delete text_frame_rate;
    delete text_camera_size;
    delete text_background_color;
    delete text_grid_color;
    delete text_ntsc_color;
    delete value_frame_rate;
    delete value_camera_size_width;
    delete value_camera_size_height;
    delete value_background_color;
    delete value_grid_color;
    delete value_ntsc_color;
}

//------------------- SLOTS -------------------------

void Properties::slotBackgroundColor()
{
    c_background = QColorDialog::getColor( k_toon -> currentStatus() -> currentBackgroundColor(), this );
    if ( c_background.isValid() )
    {
        background_color -> setPaletteBackgroundColor( c_background );
        value_background_color -> setText( c_background.name() );
    }
}

void Properties::slotGridColor()
{
    c_grid = QColorDialog::getColor( k_toon -> currentStatus() -> currentGridColor(), this );
    if ( c_grid.isValid() )
    {
        grid_color -> setPaletteBackgroundColor( c_grid );
        value_grid_color -> setText( c_grid.name() );
    }
}

void Properties::slotNTSCColor()
{
    c_ntsc = QColorDialog::getColor( k_toon -> currentStatus() -> currentNTSCColor(), this );
    if ( c_ntsc.isValid() )
    {
        ntsc_color -> setPaletteBackgroundColor( c_ntsc );
        value_ntsc_color -> setText( c_ntsc.name() );
    }
}

void Properties::slotChangeBackgroundColor( const QString &rgb )
{
    QColor intermediate;
    intermediate.setNamedColor( rgb );
    if ( intermediate.isValid() )
    {
	c_background = intermediate;
	background_color -> setPaletteBackgroundColor( intermediate );
    }
}

void Properties::slotChangeGridColor( const QString &rgb )
{
    QColor intermediate;
    intermediate.setNamedColor( rgb );
    if ( intermediate.isValid() )
    {
	c_grid = intermediate;
	grid_color -> setPaletteBackgroundColor( intermediate );
    }
}

void Properties::slotChangeNTSCColor( const QString &rgb )
{
    QColor intermediate;
    intermediate.setNamedColor( rgb );
    if ( intermediate.isValid() )
    {
	c_ntsc = intermediate;
	ntsc_color -> setPaletteBackgroundColor( intermediate );
    }
}

void Properties::slotAccept()
{
    int new_fr = ( value_frame_rate -> text() ).toInt();
    int new_cw = ( value_camera_size_width -> text() ).toInt();
    int new_ch = ( value_camera_size_height -> text() ).toInt();

    KTDoc -> getAnimation() -> setFrameRate( new_fr );
    k_toon -> currentStatus() -> setCurrentFrameRate( new_fr );
    k_toon -> currentStatus() -> setCurrentCameraWidth( new_cw );
    k_toon -> currentStatus() -> setCurrentCameraHeight( new_ch );
    k_toon -> renderCameraPreview() -> updateGL();
    k_toon -> timeline() -> slotUpdateCurrentTime( k_toon -> timeline() -> frameSequenceManager() -> getRuler() -> getOffset() );
    k_toon -> timeline() -> slotUpdateTotalTime( k_toon -> timeline() -> frameSequenceManager() -> getRuler() -> getMaxOffset() );
    k_toon -> currentStatus() -> setCurrentBackgroundColor( c_background );
    k_toon -> currentStatus() -> setCurrentGridColor( c_grid );
    k_toon -> currentStatus() -> setCurrentNTSCColor( c_ntsc );
    
    k_toon -> drawingArea() -> slotSetGridColor();
    k_toon -> drawingArea() -> slotSetNTSCColor();

    close( true );
}

void Properties::slotCancel()
{
    close( true );
}
