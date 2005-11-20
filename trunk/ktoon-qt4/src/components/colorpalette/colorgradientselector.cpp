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

#include "colorgradientselector.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QLinearGradient>

//------------ CONSTRUCTOR ----------------

ColorGradientSelector::ColorGradientSelector( QWidget *parent ) : QWidget( parent )
{
    Q_CHECK_PTR( parent );

    highlight = QColor( 255, 255, 255 );
    shadow = QColor( 0, 0, 0 );
    current_switch = NULL;

    //---------- Default gradient switches --------------

    fake_switch = new GradientSwitch( this );
    fake_switch -> hide();
    fake_switch -> setPosition( -1 );

    for ( int i = 0; i <= 168; i++ )
	switches.append( fake_switch );

    interpolateColors( QColor( 255, 255, 255 ), QColor( 0, 0, 0 ), 0, 168 );

    GradientSwitch *first_switch = new GradientSwitch( this );
    first_switch -> setPosition( 0 );
    first_switch -> move( first_switch -> position(), 10 );
    connect( first_switch, SIGNAL( dragged( int ) ), SLOT( slotDragSwitch( int ) ) );
    connect( first_switch, SIGNAL( clicked() ), SLOT( slotSelectSwitch() ) );
    connect( first_switch, SIGNAL( colorChanged( const QColor & ) ), SLOT( slotUpdateGradients() ) );
    connect( first_switch, SIGNAL( alphaChanged( int ) ), SLOT( slotUpdateGradients() ) );
    connect( first_switch, SIGNAL( deleted() ), SLOT( slotDeleteSwitch() ) );
    switches.replace( first_switch -> position(), first_switch );

    GradientSwitch *last_switch = new GradientSwitch( this );
    last_switch -> setPosition( 168 );
    last_switch -> move( last_switch -> position(), 10 );
    last_switch -> slotSetColor( QColor( 0, 0, 0 ) );
    connect( last_switch, SIGNAL( dragged( int ) ), SLOT( slotDragSwitch( int ) ) );
    connect( last_switch, SIGNAL( clicked() ), SLOT( slotSelectSwitch() ) );
    connect( last_switch, SIGNAL( colorChanged( const QColor& ) ), SLOT( slotUpdateGradients() ) );
    connect( last_switch, SIGNAL( alphaChanged( int ) ), SLOT( slotUpdateGradients() ) );
    connect( last_switch, SIGNAL( deleted() ), SLOT( slotDeleteSwitch() ) );
    switches.replace( last_switch -> position(), last_switch );
}

//------------ DESTRUCTOR -----------------

ColorGradientSelector::~ColorGradientSelector()
{

}



//----------- PUBLIC MEMBERS --------------
QSize ColorGradientSelector::sizeHint () const
{
	return QSize( 175, 20 );
}


GradientSwitch *ColorGradientSelector::currentSwitch()
{
    return current_switch;
}

GradientSwitch *ColorGradientSelector::switchBefore( GradientSwitch *gradient_switch )
{
    Q_CHECK_PTR( gradient_switch );

    GradientSwitch *gs_iterator, *gs_before;
    gs_before = NULL;
    for ( int i = 0; i < switches.count(); i++ )
    {
	    gs_iterator = switches[i];
	if ( gs_iterator != fake_switch && gs_iterator != gradient_switch )
	    gs_before = gs_iterator;

	if ( gs_iterator == gradient_switch )
	    return gs_before;
    }
    return NULL;
}

GradientSwitch *ColorGradientSelector::switchAfter( GradientSwitch *gradient_switch )
{
    Q_CHECK_PTR( gradient_switch );

    GradientSwitch *gs_iterator, *gs_before;
    gs_before = NULL;
    for ( int i = switches.count()-1; i >= 0; i-- )
    {
	    gs_iterator = switches[i];
	if ( gs_iterator != fake_switch && gs_iterator != gradient_switch )
	    gs_before = gs_iterator;

	if ( gs_iterator == gradient_switch )
	    return gs_before;
    }
    return NULL;
}

//----------- SLOTS ------------

void ColorGradientSelector::slotDragSwitch( int direction )
{
    GradientSwitch *dragged_switch = ( GradientSwitch * )sender();

    if ( direction == GradientSwitch::DRAG_LEFT )
    {
	if ( switches.at( dragged_switch -> position() - 1 ) == fake_switch )
	{
	    switches.replace( dragged_switch -> position(), fake_switch );
	    switches.replace( dragged_switch -> position() - 1, dragged_switch );
	    dragged_switch -> setPosition( dragged_switch -> position() - 1 );
	    dragged_switch -> move( dragged_switch -> position(), dragged_switch -> y() );

	    GradientSwitch *before, *after;
	    before = switchBefore( dragged_switch );
	    after = switchAfter( dragged_switch );
	    if ( before != NULL )
	    {
	        interpolateColors( before -> currentColor(), dragged_switch -> currentColor(), before -> position(), dragged_switch -> position() );
		emit gradientChanged( before -> currentColor(), dragged_switch -> currentColor(), before -> position(), dragged_switch -> position(), before -> getAlpha(), dragged_switch -> getAlpha() );
	    }
	    if ( after != NULL )
	    {
	        interpolateColors( dragged_switch -> currentColor(), after -> currentColor(), dragged_switch -> position(), after -> position() );
		emit gradientChanged( dragged_switch -> currentColor(), after -> currentColor(), dragged_switch -> position(), after -> position(), dragged_switch -> getAlpha(), after -> getAlpha() );
	    }

	    update();
	}
    }
    else
    {
	if ( switches.at( dragged_switch -> position() + 1 ) == fake_switch )
	{
	    switches.replace( dragged_switch -> position(), fake_switch );
	    switches.replace( dragged_switch -> position() + 1, dragged_switch );
	    dragged_switch -> setPosition( dragged_switch -> position() + 1 );
	    dragged_switch -> move( dragged_switch -> position(), dragged_switch -> y() );

	    GradientSwitch *before, *after;
	    before = switchBefore( dragged_switch );
	    after = switchAfter( dragged_switch );
	    if ( before != NULL )
	    {
	        interpolateColors( before -> currentColor(), dragged_switch -> currentColor(), before -> position(), dragged_switch -> position() );
		emit gradientChanged( before -> currentColor(), dragged_switch -> currentColor(), before -> position(), dragged_switch -> position(), before -> getAlpha(), dragged_switch -> getAlpha() );
	    }
	    if ( after != NULL )
	    {
	        interpolateColors( dragged_switch -> currentColor(), after -> currentColor(), dragged_switch -> position(), after -> position() );
		emit gradientChanged( dragged_switch -> currentColor(), after -> currentColor(), dragged_switch -> position(), after -> position(), dragged_switch -> getAlpha(), after -> getAlpha() );
	    }

	    update();
	}
    }
}

void ColorGradientSelector::slotSelectSwitch()
{
    if ( current_switch != NULL )
        current_switch -> setSelected( false );
    current_switch = ( GradientSwitch * )sender();
    current_switch -> setSelected( true );
    emit switchSelected( current_switch -> currentColor() );
    emit switchSelected( current_switch -> getAlpha() );
}

void ColorGradientSelector::slotUpdateGradients()
{
    GradientSwitch *switch_that_changed = ( GradientSwitch * )sender();

    GradientSwitch *before, *after;
    before = switchBefore( switch_that_changed );
    after = switchAfter( switch_that_changed );
    if ( before != NULL )
    {
        interpolateColors( before -> currentColor(), switch_that_changed -> currentColor(), before -> position(), switch_that_changed -> position() );
	emit gradientChanged( before -> currentColor(), switch_that_changed -> currentColor(), before -> position(), switch_that_changed -> position(), before -> getAlpha(), switch_that_changed -> getAlpha() );
    }
    if ( after != NULL )
    {
        interpolateColors( switch_that_changed -> currentColor(), after -> currentColor(), switch_that_changed -> position(), after -> position() );
	emit gradientChanged( switch_that_changed -> currentColor(), after -> currentColor(), switch_that_changed -> position(), after -> position(), switch_that_changed -> getAlpha(), after -> getAlpha() );
    }

    update();
}

void ColorGradientSelector::slotDeleteSwitch()
{
    GradientSwitch *deleted_switch = ( GradientSwitch * )sender();

    GradientSwitch *before, *after;
    before = switchBefore( deleted_switch );
    after = switchAfter( deleted_switch );
    if ( before != NULL && after != NULL )
    {
        interpolateColors( before -> currentColor(), after -> currentColor(), before -> position(), after -> position() );
	emit gradientChanged( before -> currentColor(), after -> currentColor(), before -> position(), after -> position(), before -> getAlpha(), after -> getAlpha() );
    }
    switches.replace( deleted_switch -> position(), fake_switch );
    delete deleted_switch;
    if ( deleted_switch == current_switch )
        current_switch = NULL;

    update();
}

//--------- EVENTS AND PROTECTED MEMBERS -----------

void ColorGradientSelector::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    if ( mouse_event -> y() >= 10 && mouse_event -> x() >= 3 && mouse_event -> x() <= 171 && mouse_event -> button() == Qt::LeftButton && mouse_event -> state() == Qt::NoButton )
    {
        if ( fake_switch != switches.at( mouse_event -> x() ) )
	    return;

        GradientSwitch *new_switch = new GradientSwitch( this );
	new_switch -> setPosition( mouse_event -> x() );
	new_switch -> move( new_switch -> position(), 10 );
	new_switch -> slotSetColor( QColor( 255, 255, 255 ) );
	new_switch -> show();
	if ( current_switch != NULL )
	    current_switch -> setSelected( false );
	current_switch = new_switch;
	current_switch -> setSelected( true );
	emit switchSelected( current_switch -> currentColor() );
	emit switchSelected( current_switch -> getAlpha() );
        connect( new_switch, SIGNAL( dragged( int ) ), SLOT( slotDragSwitch( int ) ) );
        connect( new_switch, SIGNAL( clicked() ), SLOT( slotSelectSwitch() ) );
        connect( new_switch, SIGNAL( colorChanged( const QColor& ) ), SLOT( slotUpdateGradients() ) );
        connect( new_switch, SIGNAL( alphaChanged( int ) ), SLOT( slotUpdateGradients() ) );
        connect( new_switch, SIGNAL( deleted() ), SLOT( slotDeleteSwitch() ) );
    	switches.replace( new_switch -> position(), new_switch );

	GradientSwitch *before, *after;
	before = switchBefore( new_switch );
	after = switchAfter( new_switch );
        if ( before != NULL )
	{
	    interpolateColors( before -> currentColor(), new_switch -> currentColor(), before -> position(), new_switch -> position() );
	    emit gradientChanged( before -> currentColor(), new_switch -> currentColor(), before -> position(), new_switch -> position(), before -> getAlpha(), new_switch -> getAlpha() );
	}
	if ( after != NULL )
	{
            interpolateColors( new_switch -> currentColor(), after -> currentColor(), new_switch -> position(), after -> position() );
	    emit gradientChanged( new_switch -> currentColor(), after -> currentColor(), new_switch -> position(), after -> position(), new_switch -> getAlpha(), after -> getAlpha() );
	}

	update();
	mouse_event -> accept();
    }
}

void ColorGradientSelector::paintEvent( QPaintEvent *paint_event )
{
    Q_CHECK_PTR( paint_event );
    QLinearGradient gradient(QPoint(0,0), QPoint(width(), height()));
    
    if ( paint_event -> erased() )
    {
        painter.begin( this );

        painter.setPen( shadow );
        painter.drawLine( 2, 0, 171, 0 );
        painter.drawLine( 2, 0, 2, 9 );

        painter.setPen( highlight );
        painter.drawLine( 171, 1, 171, 9 );
        painter.drawLine( 3, 9, 171, 9 );

        //---------- Paint the frame according to switch colors -----------

//         QColor pen_color;
//         for ( int i = 0; i <= 167; i++ )
//         {
// 		pen_color = color_array[i];
// 		gradient.setColorAt(i, color_array[i]);
		
// 	}
	painter.setBrush(gradient);
	painter.drawRect(0, 0, width(), height()/2);

	
//             pen_color = color_array[i];
//             painter.setPen( pen_color );
// 	    painter.drawLine( i + 3, 1, i + 3, 8 );
//         }
// 	    painter.

        painter.end();
    }
}

void ColorGradientSelector::interpolateColors( const QColor &color1, const QColor &color2, int pos_color1, int pos_color2 )
{
    Q_ASSERT( color1.isValid() && color2.isValid() );
    QColor intermediate;
    int grad_distance = pos_color2 - pos_color1;

    int i;
    float k;
    if ( grad_distance > 1 )
    {
        k = 1.0;
	for ( i = pos_color1; i < pos_color2; i++ )
	{
	    color_array[i] = QColor( ( int )( color1.red() * k + color2.red() * ( 1.0 - k ) ),
	    			     ( int )( color1.green() * k + color2.green() * ( 1.0 - k ) ),
				     ( int )( color1.blue() * k + color2.blue() * ( 1.0 - k ) ) );
	    k = k - 1.0 / ( float )grad_distance;
	}
    }
}


