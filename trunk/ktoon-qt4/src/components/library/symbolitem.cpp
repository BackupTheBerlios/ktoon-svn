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

#include "symboltable.h"
#include "symbolitem.h"
#include "drawingarea.h"
#include "../images/images.h"

#include <qmatrix.h>
//Added by qt3to4:
#include <QPixmap>

//--------------- CONSTRUCTORS --------------

SymbolItem::SymbolItem( SymbolTable *parent, const QString &text ) : Q3ListViewItem( parent, text )
{
    Q_CHECK_PTR( parent );
    kind = UNDEFINED;
    graphic = NULL;

    i_folder = QPixmap( folder_icon_xpm );
    i_graphic = QPixmap( redhat_graphics_xpm );
}

SymbolItem::SymbolItem( SymbolItem *parent, const QString &text ) : Q3ListViewItem( parent, text )
{
    Q_CHECK_PTR( parent );
    kind = UNDEFINED;
    graphic = NULL;

    i_folder = QPixmap( folder_icon_xpm );
    i_graphic = QPixmap( redhat_graphics_xpm );
}

//--------------- DESTRUCTOR ---------------

SymbolItem::~SymbolItem()
{

}

//--------------- PUBLIC MEMBERS --------------

int SymbolItem::getKind() const
{
    return kind;
}

void SymbolItem::setKind( int kind_ )
{
    kind = kind_;

    QMatrix m;
    if ( kind == FOLDER )
    {
        m.scale( 0.6, 0.6 );
        QPixmap small = i_folder.xForm( m );
        setPixmap( 0, small );
    }
    else if ( kind == GRAPHIC )
    {
        m.scale( 0.55, 0.55 );
        QPixmap small = i_graphic.xForm( m );
        setPixmap( 0, small );
    }
}

GLGraphicComponent *SymbolItem::getGraphic() const
{
    Q_CHECK_PTR( graphic );
    return graphic;
}

void SymbolItem::setGraphic( GLGraphicComponent *graphic_ )
{
    Q_CHECK_PTR( graphic_ );
    graphic = graphic_;
}

bool SymbolItem::folderHasSymbols() const
{
    if ( kind != FOLDER )
        return false;

    Q3ListViewItemIterator it( ( SymbolItem * )this );

    ++it;
    while ( it.current() )
    {
        SymbolItem *item = ( SymbolItem * )it.current();
	if ( item -> depth() <= depth() )
	    return false;
        else if ( item -> getKind() == SymbolItem::GRAPHIC )
	    return true;
        ++it;
    }

    return false;
}

//---------------- SLOTS -----------------

//-------------- EVENTS AND PROTECTED MEMBERS -------------

