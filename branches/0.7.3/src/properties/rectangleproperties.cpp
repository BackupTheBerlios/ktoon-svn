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

#include "rectangleproperties.h"
#include "../gl/drawingarea.h"

//--------------- CONSTRUCTOR --------------------

RectangleProperties::RectangleProperties( QWidget *parent ) : QDialog( parent, "Rectangle Properties", true )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    setCaption( tr( "Rectangle Properties" ) );
    setFont( QFont( "helvetica", 10 ) );
    parent_widget = parent;
    resize( 235, 205 );
    setMinimumSize( 235, 205 );
    setMaximumSize( 235, 205 );
    drawing_area = ( DrawingArea * ) parent_widget;

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

    text_pattern = new QLabel( tr( "Border Style" ) + QString( ":" ), this );
    text_pattern -> resize( 100, 20 );
    text_pattern -> move( text_size -> x(), text_size -> y() + text_size -> height() + 5 );

    text_factor = new QLabel( tr( "Stipple Factor" ) + QString( ":" ), this );
    text_factor -> resize( 100, 20 );
    text_factor -> move( text_pattern -> x(), text_pattern -> y() + text_pattern -> height() + 5 );

    text_angle = new QLabel( tr( "Rotation Angle" ) + QString( ":" ), this );
    text_angle -> resize( 100, 20 );
    text_angle -> move( text_factor -> x(), text_factor -> y() + text_factor -> height() + 5 );

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

    value_factor = new QLineEdit( this );
    value_factor -> resize( 110, 20 );
    value_factor -> move( text_factor -> x() + text_factor -> width() + 5, text_factor -> y() );

    value_angle = new QLineEdit( this );
    value_angle -> resize( 110, 20 );
    value_angle -> move( text_angle -> x() + text_angle -> width() + 5, text_angle -> y() );

    //------------- Operations on other components -------------

    check_perfect = new QCheckBox( tr( "Perfect" ), this );
    check_perfect -> resize( 100, 20 );
    check_perfect -> move( text_angle -> x(), text_angle -> y() + text_angle -> height() + 5 );

    check_fill = new QCheckBox( tr( "Fill" ), this );
    check_fill -> resize( 100, 20 );
    check_fill -> move( check_perfect -> x() + check_perfect -> width() + 5, check_perfect -> y() );

    QStringList p_str;
    p_str << "___________" << "_ _ _ _ _ _ _" << "_  _  _  _  _" << ". . . . . . . . ." << ". _ . _ . _ . _"
     	  << "___ ___ ___" << "___ _ ___ _" << "___  _  ___";
    value_pattern = new QComboBox( this );
    value_pattern -> insertStringList( p_str );
    value_pattern -> resize( 110, 20 );
    value_pattern -> move( text_pattern -> x() + text_pattern -> width() + 5, text_pattern -> y() );

    //------------- Operations on the buttons -----------------

    accept = new QPushButton( tr( "Accept" ), this );
    accept -> resize( 60, 30 );
    accept -> move( check_perfect -> x() + 45, check_perfect -> y() + check_perfect -> height() + 10 );
    connect( accept, SIGNAL( clicked() ), SLOT( slotAccept() ) );

    cancel = new QPushButton( tr( "Cancel" ), this );
    cancel -> resize( 60, 30 );
    cancel -> move( accept -> x() + accept -> width() + 5, accept -> y() );
    connect( cancel, SIGNAL( clicked() ), SLOT( slotCancel() ) );
}

//-------------- DESTRUCTOR -----------------

RectangleProperties::~RectangleProperties()
{
    delete accept;
    delete cancel;
    delete text_origin;
    delete text_origin_x;
    delete text_origin_y;
    delete text_pattern;
    delete text_factor;
    delete text_size;
    delete text_size_w;
    delete text_size_h;
    delete text_angle;
    delete value_origin_x;
    delete value_origin_y;
    delete value_factor;
    delete value_size_w;
    delete value_size_h;
    delete value_angle;
    delete check_fill;
    delete check_perfect;
    delete value_pattern;
}

//-------------- PUBLIC MEMBERS ----------------

void RectangleProperties::loadFromGraphic( GLRectangle *rectangle )
{
   Q_CHECK_PTR( rectangle );
   value_origin_x -> setText( QString::number( rectangle -> originPoint().x() ) );
   value_origin_y -> setText( QString::number( rectangle -> originPoint().y() ) );
   QPoint end = rectangle -> endRectangle();
   int w = end.x() - rectangle -> originPoint().x();
   int h = end.y() - rectangle -> originPoint().y();
   value_size_w -> setText( QString::number( w ) );
   value_size_h -> setText( QString::number( h ) );
   switch ( rectangle -> stipplePattern() )
     {
        case 0xFFFF:
   		value_pattern -> setCurrentItem( 0 );
		break;
        case 0x5555:
   		value_pattern -> setCurrentItem( 1 );
		break;
        case 0x0707:
   		value_pattern -> setCurrentItem( 2 );
		break;
        case 0x1111:
   		value_pattern -> setCurrentItem( 3 );
		break;
        case 0x6F6F:
   		value_pattern -> setCurrentItem( 4 );
		break;
        case 0x9F9F:
   		value_pattern -> setCurrentItem( 5 );
		break;
        case 0x5F5F:
   		value_pattern -> setCurrentItem( 6 );
		break;
        case 0x2727:
   		value_pattern -> setCurrentItem( 7 );
		break;
	default:
	        break;
     }
   value_factor -> setText( QString::number( rectangle -> stippleFactor() ) );
   value_angle -> setText( QString::number( 0 ) );
   check_fill -> setChecked( rectangle -> fill() );
   check_perfect -> setChecked( rectangle -> perfectRectangle() );
}

//------------------- SLOTS -------------------------

void RectangleProperties::slotAccept()
{
   GLRectangle *rectangle = ( GLRectangle * )drawing_area -> currentGraphic();
   rectangle -> setOriginPoint( QPoint( ( value_origin_x -> text() ).toInt(), ( value_origin_y -> text() ).toInt() ) );
   int new_x = ( value_origin_x -> text() ).toInt() + ( value_size_w -> text() ).toInt();
   int new_y = ( value_origin_y -> text() ).toInt() + ( value_size_h -> text() ).toInt();
   rectangle -> setEndRectangle( QPoint( new_x, new_y ) );
   rectangle -> setFill( check_fill -> isChecked() );
   rectangle -> setPerfectRectangle( check_perfect -> isChecked() );
   switch ( value_pattern -> currentItem() )
       {
   	  case 0:
	  	rectangle -> setStipplePattern( 0xFFFF );
		break;
	  case 1:
	  	rectangle -> setStipplePattern( 0x5555 );
		break;
	  case 2:
	  	rectangle -> setStipplePattern( 0x0707 );
		break;
	  case 3:
	  	rectangle -> setStipplePattern( 0x1111 );
		break;
	  case 4:
	  	rectangle -> setStipplePattern( 0x6F6F );
		break;
	  case 5:
	  	rectangle -> setStipplePattern( 0x9F9F );
		break;
	  case 6:
	  	rectangle -> setStipplePattern( 0x5F5F );
		break;
	  case 7:
	  	rectangle -> setStipplePattern( 0x2727 );
		break;
	  default:
		break;

       }
   rectangle -> setStippleFactor( ( value_factor -> text() ).toUShort() );
   rectangle -> setRotationAngle( ( value_angle -> text() ).toFloat() );
   drawing_area -> updateGL();
   drawing_area -> modifyDocument( true );
   close( true );
}

void RectangleProperties::slotCancel()
{
    close( true );
}
