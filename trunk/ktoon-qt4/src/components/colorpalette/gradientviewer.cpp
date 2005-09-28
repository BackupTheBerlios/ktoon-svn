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

#include <q3ptrlist.h>
//Added by qt3to4:
#include <Q3Frame>

#include "gradientviewer.h"

//-------------- CONSTRUCTOR ---------------

GradientViewer::GradientViewer( QWidget *parent ) : Q3Frame( parent )
{
    Q_CHECK_PTR( parent );

    setMinimumSize( 46, 46 );
    setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
    parent_widget = parent;
    dark_color = QColor( 135, 135, 135 );
    light_color = paletteBackgroundColor();
    type = LINEAR;

    interpolateColors( QColor( 255, 255, 255 ), QColor( 0, 0, 0 ), 0, 44, 100, 100 );
}

//------------- DESTRUCTOR ------------------

GradientViewer::~GradientViewer()
{

}

//------------ PUBLIC MEMBERS ---------------

//--------------- SLOTS ---------------

void GradientViewer::slotSetType( int in_type )
{
    type = in_type;
    update();
}

void GradientViewer::slotUpdateGradient( const QColor &color1, const QColor &color2, int pos1, int pos2, int alpha1, int alpha2 )
{
    interpolateColors( color1, color2, mapGSPosition( pos1 ), mapGSPosition( pos2 ), alpha1, alpha2 );
    update();
}

//---------- EVENTS AND PROTECTED MEMBERS -------------

void GradientViewer::drawContents( QPainter *painter )
{
    Q_CHECK_PTR( painter );

    QColor pen_color, brush_color;

    pen_color = Qt::white;
    brush_color = Qt::white;
    painter -> setPen( pen_color );
    painter -> setBrush( brush_color );
    painter -> drawRect( 0, 0, 45, 45 );

    if ( type == LINEAR )
    {
        for ( int i = 0; i <= 43; i++ )
        {
            pen_color = linear_array[i];
            painter -> setPen( pen_color );
	    painter -> drawLine( i, 0, i, 43 );
	    pen_color = linear_array_d[i];
	    painter -> setPen( pen_color );
	    painter -> drawPoint( i, 6 );
	    painter -> drawPoint( i, 12 );
	    painter -> drawPoint( i, 18 );
	    painter -> drawPoint( i, 24 );
	    painter -> drawPoint( i, 30 );
	    painter -> drawPoint( i, 36 );
        }
    }
    else if ( type == RADIAL )
    {
        for ( int i = 0; i <= 21; i++ )
        {
            pen_color = radial_array[21 - i];
	    brush_color = radial_array[21 - i];
            painter -> setPen( pen_color );
	    painter -> setBrush( brush_color );
	    painter -> drawEllipse( i + 1, i + 1, 43 - 2 * i, 43 - 2 * i );
	    pen_color = radial_array_d[21 - i];
	    painter -> setPen( pen_color );
	    painter -> drawPoint( ( i + 1 ) + ( 43 - 2 * i ) / 2, i + 1 );
	    painter -> drawPoint( i + 1, ( i + 1 ) + ( 43 - 2 * i ) / 2 );
	    painter -> drawPoint( ( i + 1 ) + ( 43 - 2 * i ) / 2, i + ( 43 - 2 * i ) );
	    painter -> drawPoint( i + ( 43 - 2 * i ), ( i + 1 ) + ( 43 - 2 * i ) / 2 );
        }
    }
}

void GradientViewer::interpolateColors( const QColor &color1, const QColor &color2, int pos_color1, int pos_color2, int alpha1, int alpha2 )
{
    QColor c1, c2, cd1, cd2;
    int grad_distance = pos_color2 - pos_color1;
    float a1, a2;
    a1 = ( float )alpha1 / 100.0;
    a2 = ( float )alpha2 / 100.0;
    c1 = QColor( ( int )( color1.red() * a1 + light_color.red() * ( 1.0 - a1 ) ),
    		 ( int )( color1.green() * a1 + light_color.green() * ( 1.0 - a1 ) ),
		 ( int )( color1.blue() * a1 + light_color.blue() * ( 1.0 - a1 ) ) );
    c2 = QColor( ( int )( color2.red() * a2 + light_color.red() * ( 1.0 - a2 ) ),
    		 ( int )( color2.green() * a2 + light_color.green() * ( 1.0 - a2 ) ),
		 ( int )( color2.blue() * a2 + light_color.blue() * ( 1.0 - a2 ) ) );
    cd1 = QColor( ( int )( color1.red() * a1 + dark_color.red() * ( 1.0 - a1 ) ),
    		 ( int )( color1.green() * a1 + dark_color.green() * ( 1.0 - a1 ) ),
		 ( int )( color1.blue() * a1 + dark_color.blue() * ( 1.0 - a1 ) ) );
    cd2 = QColor( ( int )( color2.red() * a2 + dark_color.red() * ( 1.0 - a2 ) ),
    		 ( int )( color2.green() * a2 + dark_color.green() * ( 1.0 - a2 ) ),
		 ( int )( color2.blue() * a2 + dark_color.blue() * ( 1.0 - a2 ) ) );

    int i;
    float k;
    if ( grad_distance > 1 )
    {
        k = 1.0;
	for ( i = pos_color1; i < pos_color2; i++ )
	{
	    linear_array[i] = QColor( ( int )( c1.red() * k + c2.red() * ( 1.0 - k ) ),
	    			      ( int )( c1.green() * k + c2.green() * ( 1.0 - k ) ),
				      ( int )( c1.blue() * k + c2.blue() * ( 1.0 - k ) ) );
	    linear_array_d[i] = QColor( ( int )( cd1.red() * k + cd2.red() * ( 1.0 - k ) ),
	    			        ( int )( cd1.green() * k + cd2.green() * ( 1.0 - k ) ),
				        ( int )( cd1.blue() * k + cd2.blue() * ( 1.0 - k ) ) );
	    k = k - 1.0 / ( float )grad_distance;
	}
        k = 1.0;
	for ( i = pos_color1 / 2; i < pos_color2 / 2; i++ )
	{
	    radial_array[i] = QColor( ( int )( c1.red() * k + c2.red() * ( 1.0 - k ) ),
	    			      ( int )( c1.green() * k + c2.green() * ( 1.0 - k ) ),
				      ( int )( c1.blue() * k + c2.blue() * ( 1.0 - k ) ) );
	    radial_array_d[i] = QColor( ( int )( cd1.red() * k + cd2.red() * ( 1.0 - k ) ),
	    			        ( int )( cd1.green() * k + cd2.green() * ( 1.0 - k ) ),
				        ( int )( cd1.blue() * k + cd2.blue() * ( 1.0 - k ) ) );
	    k = k - 1.0 / ( float )( grad_distance / 2 );
	}
    }
}

int GradientViewer::mapGSPosition( int gs_pos )
{
    return ( gs_pos * 44 ) / 168;
}
