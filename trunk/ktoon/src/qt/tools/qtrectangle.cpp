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

#include "qtrectangle.h"

#include <qpainter.h>

//--------------- CONSTRUCTOR --------------

QtRectangle::QtRectangle( QCanvas *canvas ) : QCanvasRectangle( canvas ), selected( false )
{

}

//--------------- DESTRUCTOR ---------------

QtRectangle::~QtRectangle()
{
    hide();
}

//--------------- PUBLIC MEMBERS --------------

int QtRectangle::rtti() const
{
    return RTTI;
}

bool QtRectangle::isSelected() const
{
    return selected;
}

void QtRectangle::setSelected( bool select )
{
    selected = select;
    update();
}

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void QtRectangle::drawShape( QPainter &painter )
{
    QCanvasRectangle::drawShape( painter );
    if ( selected )
        drawSelectionArrows( painter );
}

void QtRectangle::drawSelectionArrows( QPainter &painter )
{
    painter.setPen( QPen( QColor( 0, 0, 255 ), 2 ) );
}
