/***************************************************************************
 *   Copyright (C) 2004 by Fernando José Roldán Correa                     *
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

#include "imageproperties.h"
#include "drawingarea.h"
//Added by qt3to4:
#include <QLabel>

//--------------- CONSTRUCTOR --------------------

ImageProperties::ImageProperties( QWidget *parent ) : QDialog( parent, "Image Properties", true )
{
    Q_CHECK_PTR( parent );

    //Initializations
    setCaption( tr( "Image Properties" ) );
//     setFont( QFont( "helvetica", 10 ) );
    parent_widget = parent;
    resize( 235, 130 );
    setMinimumSize( 235, 130 );
    setMaximumSize( 235, 130 );
    drawing_area = ( DrawingArea * )parent_widget;

    //------------- Operations on the static texts ----------

    text_origin = new QLabel( tr( "Origin Point" ) + QString( ":" ), this );
    text_origin -> resize( 100, 20 );
    text_origin -> move( 10, 10 );

    text_origin_x = new QLabel( "X", this );
    text_origin_x -> resize( 15, 20 );
    text_origin_x -> move( text_origin -> x() + text_origin -> width() + 5, text_origin -> y() );

    text_origin_y = new QLabel( "Y", this );
    text_origin_y -> resize( 15, 20 );
    text_origin_y -> move( text_origin_x -> x() + text_origin_x -> width() + 45, text_origin_x -> y() );

    text_size = new QLabel( tr( "Size" ) + QString( ":" ), this );
    text_size -> resize( 100, 20 );
    text_size -> move( text_origin -> x(), text_origin -> y() + text_origin -> height() + 5 );

    text_size_w = new QLabel( "W", this );
    text_size_w -> resize( 15, 20 );
    text_size_w -> move( text_size -> x() + text_size -> width() + 5, text_size -> y() );

    text_size_h = new QLabel( "H", this );
    text_size_h -> resize( 15, 20 );
    text_size_h -> move( text_size_w -> x() + text_size_w -> width() + 45, text_size_w -> y() );

    text_angle = new QLabel( tr( "Rotation Angle" ) + QString( ":" ), this );
    text_angle -> resize( 100, 20 );
    text_angle -> move( text_size -> x(), text_size -> y() + text_size -> height() + 5 );

    //------------- Operations on the Textfields -------------

    value_origin_x = new QLineEdit( this );
    value_origin_x -> resize( 30, 20 );
    value_origin_x -> move( text_origin_x -> x() + text_origin_x -> width() + 5, text_origin_x -> y() );

    value_origin_y = new QLineEdit( this );
    value_origin_y -> resize( 30, 20 );
    value_origin_y -> move( text_origin_y -> x() + text_origin_y -> width() + 5, text_origin_y -> y() );

    value_size_w = new QLineEdit( this );
    value_size_w -> resize( 30, 20 );
    value_size_w -> move( text_size_w -> x() + text_size_w -> width() + 5, text_size_w -> y() );

    value_size_h = new QLineEdit( this );
    value_size_h -> resize( 30, 20 );
    value_size_h -> move( text_size_h -> x() + text_size_h -> width() + 5, text_size_h -> y() );

    value_angle = new QLineEdit( this );
    value_angle -> resize( 110, 20 );
    value_angle -> move( text_angle -> x() + text_angle -> width() + 5, text_angle -> y() );

    //------------- Operations on the buttons -----------------

    accept = new QPushButton( tr( "Accept" ), this );
    accept -> resize( 60, 30 );
    accept -> move( text_angle -> x() + 45, text_angle -> y() + text_angle -> height() + 10 );
    connect( accept, SIGNAL( clicked() ), SLOT( slotAccept() ) );

    cancel = new QPushButton( tr( "Cancel" ), this );
    cancel -> resize( 60, 30 );
    cancel -> move( accept -> x() + accept -> width() + 5, accept -> y() );
    connect( cancel, SIGNAL( clicked() ), SLOT( slotCancel() ) );
}

//-------------- DESTRUCTOR -----------------

ImageProperties::~ImageProperties()
{
    delete accept;
    delete cancel;
    delete text_origin;
    delete text_origin_x;
    delete text_origin_y;
    delete text_size;
    delete text_size_w;
    delete text_size_h;
    delete text_angle;
    delete value_origin_x;
    delete value_origin_y;
    delete value_size_w;
    delete value_size_h;
    delete value_angle;
}

//-------------- PUBLIC MEMBERS ----------------

void ImageProperties::loadFromGraphic( GLImage *image )
{
   Q_CHECK_PTR( image );

   value_origin_x -> setText( QString::number( image -> originPoint().x() ) );
   value_origin_y -> setText( QString::number( image -> originPoint().y() ) );
   QPoint end = image -> endImage();
   int w = end.x() - image -> originPoint().x();
   int h = end.y() - image -> originPoint().y();
   value_size_w -> setText( QString::number( w ) );
   value_size_h -> setText( QString::number( h ) );
   value_angle -> setText( QString::number( 0 ) );
}

//------------------- SLOTS -------------------------

void ImageProperties::slotAccept()
{
   GLImage *image = ( GLImage * )drawing_area -> currentGraphic();
   image -> setOriginPoint( QPoint( ( value_origin_x -> text() ).toInt(), ( value_origin_y -> text() ).toInt() ) );
   int new_x = ( value_origin_x -> text() ).toInt() + ( value_size_w -> text() ).toInt();
   int new_y = ( value_origin_y -> text() ).toInt() + ( value_size_h -> text() ).toInt();
   image -> setEndImage( QPoint( new_x, new_y ) );
   image -> setRotationAngle( ( value_angle -> text() ).toFloat() );
   drawing_area -> updateGL();
   drawing_area -> modifyDocument( true );
   close( true );
}

void ImageProperties::slotCancel()
{
    close( true );
}
