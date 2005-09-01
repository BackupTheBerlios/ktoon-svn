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

#include "tools.h"
#include "status.h"
#include "images.h"

#include <qtooltip.h>

//--------------- CONSTRUCTOR --------------------

Tools::Tools( QWidget *parent)//, WFlags style, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button )
	: KTDialogBase(QDockWindow::OutsideDock,parent, "Tools")
{
		
	Q_CHECK_PTR( parent );
    //Initializations
	setCaption( "tools" );
	move( 13, 88 );

    //Icon Initializations
	icon_selection = QPixmap( selection_xpm );
	icon_nodes = QPixmap( nodes_xpm );
	icon_brush = QPixmap( brush_xpm );
	icon_pencil = QPixmap( pencil_xpm );
	icon_line = QPixmap( line_xpm );
	icon_pen = QPixmap( bezier_xpm );
	icon_rectangle = QPixmap( square_xpm );
	icon_ellipse = QPixmap( ellipse_xpm );
	icon_eraser = QPixmap( eraser_xpm );
	icon_slicer = QPixmap( slicer_xpm );
	icon_fill = QPixmap( fill_xpm );
	icon_contour_fill = QPixmap( contour_xpm );
	icon_remove_fill = QPixmap( removefill_xpm );
	icon_dropper = QPixmap( dropper_xpm );
	icon_magnifying_glass = QPixmap( magnifying_xpm );
	icon_hand = QPixmap( hand_xpm );

    //-------- Operations on the Menus associated with every main button --------

	menu_button_selection = new QPopupMenu( this );
	menu_button_selection -> insertItem( icon_selection, tr( "Normal Selection" ), parent, SLOT( slotNormalSelection() ) );
	menu_button_selection -> insertItem( icon_nodes, tr( "Contour Selection" ), parent, SLOT( slotContourSelection() ) );

	menu_button_draw = new QPopupMenu( this );
	menu_button_draw -> insertItem( icon_brush, tr( "Brush" ), parent, SLOT( slotBrush() ) );
	menu_button_draw -> insertItem( icon_pencil, tr( "Pencil" ), parent, SLOT( slotPencil() ) );
	menu_button_draw -> insertItem( icon_pen, tr( "Pen" ), parent, SLOT( slotPen() ) );
	menu_button_draw -> insertItem( icon_line, tr( "Line" ), parent, SLOT( slotLine() ) );
	menu_button_draw -> insertItem( icon_rectangle, tr( "Rectangle" ), parent, SLOT( slotRectangle() ) );
	menu_button_draw -> insertItem( icon_ellipse, tr( "Ellipse" ), parent, SLOT( slotEllipse() ) );

	menu_button_fill = new QPopupMenu( this );
	menu_button_fill -> insertItem( icon_fill, tr( "Fill" ), parent, SLOT( slotFill() ) );
	menu_button_fill -> insertItem( icon_remove_fill, tr( "Remove Fill" ), parent, SLOT( slotRemoveFill() ) );
	menu_button_fill -> insertItem( icon_contour_fill, tr( "Contour Fill" ), parent, SLOT( slotContourFill() ) );
	menu_button_fill -> insertItem( icon_dropper, tr( "Dropper" ), parent, SLOT( slotDropper() ) );

	menu_button_eraser = new QPopupMenu( this );
	menu_button_eraser -> insertItem( icon_eraser, tr( "Eraser" ), parent, SLOT( slotEraser() ) );
	menu_button_eraser -> insertItem( icon_slicer, tr( "Slicer" ), parent, SLOT( slotSlicer() ) );

	menu_button_view = new QPopupMenu( this );
	menu_button_view -> insertItem( icon_magnifying_glass, tr( "Magnifying Glass" ), parent, SLOT( slotMagnifyingGlass() ) );
	menu_button_view -> insertItem( icon_hand, tr( "Hand" ), parent, SLOT( slotHand() ) );

    //-------- Operations on the Main Buttons -------------

	selection_button = new QToolButton( this );
	selection_button -> setIconSet( icon_selection );
	addChild(selection_button);
	selection_button -> setPopup( menu_button_selection );
	selection_button -> setPopupDelay( 1 );
	QToolTip::add( selection_button, tr( "Selection Tools" ) );

	draw_button = new QToolButton( this );
	draw_button -> setIconSet( icon_brush );
	addChild(draw_button);
	draw_button -> setPopup( menu_button_draw );
	draw_button -> setPopupDelay( 1 );
	QToolTip::add( draw_button, tr( "Draw Tools" ) );

	fill_button = new QToolButton( this );
	fill_button -> setIconSet( icon_fill );
	addChild(fill_button);
	fill_button -> setPopup( menu_button_fill );
	fill_button -> setPopupDelay( 1 );
	QToolTip::add( fill_button, tr( "Fill Tools" ) );

	eraser_button = new QToolButton( this );
	eraser_button -> setIconSet( icon_eraser );
	addChild(eraser_button);
	eraser_button -> setPopup( menu_button_eraser );
	eraser_button -> setPopupDelay( 1 );
	QToolTip::add( eraser_button, tr( "Eraser Tools" ) );

	view_button = new QToolButton( this );
	view_button -> setIconSet( icon_magnifying_glass );
	addChild(view_button);
	view_button -> setPopup( menu_button_view );
	view_button -> setPopupDelay( 1 );
	QToolTip::add( view_button, tr( "View Tools" ) );
	adjustSize();
}

//-------------- DESTRUCTOR -----------------

Tools::~Tools()
{
	delete menu_button_selection;
	delete menu_button_draw;
	delete menu_button_fill;
	delete menu_button_eraser;
	delete menu_button_view;
    
	delete selection_button;
	delete draw_button;
	delete fill_button;
	delete eraser_button;
	delete view_button;
}

//--------------------- EVENTS AND OTHER FUNCTIONS --------------------------------

// void Tools::closeEvent( QCloseEvent* close_event )
// {
// 	Q_CHECK_PTR( close_event );
// 	assigned_menu -> setItemChecked( assigned_item, false );
// 	assigned_tb_button -> setDown( false );
// 	KTDialogBase::closeEvent(close_event);
// }

void Tools::changeButtonImage( int id_button )
{
	switch ( id_button )
	{
		case NORMAL_SELECTION: selection_button -> setIconSet( icon_selection );
		KTStatus -> setCurrentCursor( NORMAL_SELECTION );
		break;
		case CONTOUR_SELECTION: selection_button -> setIconSet( icon_nodes );
		KTStatus -> setCurrentCursor( CONTOUR_SELECTION );
		break;
		case BRUSH: draw_button -> setIconSet( icon_brush );
		KTStatus -> setCurrentCursor( BRUSH );
		break;
		case PENCIL: draw_button -> setIconSet( icon_pencil );
		KTStatus -> setCurrentCursor( PENCIL );
		break;
		case PEN: draw_button -> setIconSet( icon_pen );
		KTStatus -> setCurrentCursor( PEN );
		break;
		case LINE: draw_button -> setIconSet( icon_line );
		KTStatus -> setCurrentCursor( LINE );
		break;
		case RECTANGLE: draw_button -> setIconSet( icon_rectangle );
		KTStatus -> setCurrentCursor( RECTANGLE );
		break;
		case ELLIPSE: draw_button -> setIconSet( icon_ellipse );
		KTStatus -> setCurrentCursor( ELLIPSE );
		break;
		case CONTOUR_FILL: fill_button -> setIconSet( icon_contour_fill );
		KTStatus -> setCurrentCursor( CONTOUR_FILL );
		break;
		case FILL: fill_button -> setIconSet( icon_fill );
		KTStatus -> setCurrentCursor( FILL );
		break;
		case REMOVE_FILL: fill_button -> setIconSet( icon_remove_fill );
		KTStatus -> setCurrentCursor( REMOVE_FILL );
		break;
		case DROPPER: fill_button -> setIconSet( icon_dropper );
		KTStatus -> setCurrentCursor( DROPPER );
		break;
		case ERASER: eraser_button -> setIconSet( icon_eraser );
		KTStatus -> setCurrentCursor( ERASER );
		break;
		case SLICER: eraser_button -> setIconSet( icon_slicer );
		KTStatus -> setCurrentCursor( SLICER );
		break;
		case MAGNIFYING_GLASS: view_button -> setIconSet( icon_magnifying_glass );
		KTStatus -> setCurrentCursor( MAGNIFYING_GLASS );
		break;
		case HAND: view_button -> setIconSet( icon_hand );
		KTStatus -> setCurrentCursor( HAND );
		break;
		default: break;
	}
}
