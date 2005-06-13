/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#include <stdlib.h>
#include <qimage.h>
#include <qcursor.h>
#include <qpopupmenu.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qclipboard.h>

#include "drawingarea.h"

#include "keyframe.h"
#include "brushproperties.h"
#include "pencilproperties.h"
#include "penproperties.h"
#include "lineproperties.h"
#include "rectangleproperties.h"
#include "ellipseproperties.h"
#include "imageproperties.h"
#include "images.h"
#include "ktapplication.h"
#include "status.h"


//--------------- CONSTRUCTOR --------------------


DrawingArea::DrawingArea( QWidget *parent, const char *name, WFlags f ) : GLControl( parent, name, 0, f | WDestructiveClose), rotation_x( 0.0 ), rotation_y( 0.0 ), rotation_z( 0.0 ), translation_x( 0.0 ), translation_y( 0.0 ), translation_z( -10.0 ), scale_x( 1.0 ), scale_y( 1.0 ), scale_z( 1.0 )
{	
	qDebug("[Initializing DrawingArea]");
	
	number_lines = 32;
	max_vertical = 440;
	max_horizontal = 600;
	vertical_margin = 0;
	horizontal_margin = 0;
	previous_cursor = Tools::NORMAL_SELECTION;
	
	zoom_lines = false;
	ntsc_zone = false;
	bezier = false;
	selected_graphic = false;
	modified = false;
	light_table = false;
	using_hand = false;
	front_grid = false;
	selected_all = false;
	
	grid = NULL;
	current_graphic = NULL;
	right_click_menu = NULL;
	
	shift_pressed = false;
	
	w = 640;
	h = 480;
	
	//Initializations
	QWidget::resize( 640, 480 );
	move( 146, 8 );
	setMinimumSize( 160, 120 );
	setMaximumSize( 1023, 650 );
	setCaption( name ); 
}

//-------------- DESTRUCTOR -----------------

DrawingArea::~DrawingArea()
{
	qDebug("[Destroying DrawingArea]");
	if( grid != NULL )
		delete grid;
	if ( right_click_menu != NULL )
		delete right_click_menu;
}

//--------------------- PUBLIC AND PROTECTED MEMBERS --------------------------------

void DrawingArea::initializeGL()
{
	glShadeModel( GL_SMOOTH );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_LINE_SMOOTH );
	//glEnable( GL_POLYGON_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	
	glEnable( GL_BLEND );
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0 );
	glDepthRange( 0.0, 1.0 );
	glDepthMask( GL_TRUE );
	
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	
	glEnable( GL_TEXTURE_2D );
	id_texture = loadTexture( KTOON_HOME + "/images/arrow_selection.xpm" );
	id_spin = loadTexture( KTOON_HOME + "/images/spin.xpm" );
	glDisable( GL_TEXTURE_2D );
	
	for ( int i = 0; i < 9; i++ )
	{
		id_arrow_texture[i] = createSelectionArrows( id_texture );
		id_arrow_names[i] = GLGraphicComponent::selection_name - 1;
	}
	
	id_spin_texture = createSelectionArrows( id_spin );
	
	grid = new GLGrid( this, maxHorizontal(), maxVertical(), numberLines(), zoomLines(), ntscZone() );
}

void DrawingArea::resizeGL( int w, int h )
{
	Q_ASSERT( w > 0 && h > 0 );
	glViewport( 0, 0, ( GLint ) w, ( GLint ) h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, w, h, 0.0, -10.0, 10.0 );
	setHeight( ( GLuint ) h );
	setWidth( ( GLuint ) w );
	glMatrixMode( GL_MODELVIEW );
}

void DrawingArea::paintGL()
{
	GLGraphicComponent *graphic;
	
	QColor b_color = KTStatus -> currentBackgroundColor();
	glClearColor( ( float )b_color.red() / 255.0, ( float )b_color.green() / 255.0, ( float )b_color.blue() / 255.0, 0.0 );
	
	// Clear the elements on the screen.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//calculate the grid position
	gridPosition();
	
	glPushMatrix();
	
	glScalef( scaleX(), scaleY(), 1.0 );
	glTranslatef( horizontalMargin(), verticalMargin(), 0.0 );
	
	glTranslatef( maxHorizontal() / 2.0, maxVertical() / 2.0, 0.0 );
	glRotatef( rotationZ(), 0.0, 0.0, 1.0 );
	glTranslatef( -1.0 * maxHorizontal() / 2.0, -1.0 * maxVertical() / 2.0, 0.0 );
	
	current_cursor = KTStatus -> currentCursor();
	
	//----------- Draw the onion skin and Light Table---------------
	if( KTStatus -> currentPreviousOnionSkin() != 0 || KTStatus -> currentNextOnionSkin() != 0 || light_table )
		drawOnionSkinsAndLightTable();
	
	//----------- Draw the grid ---------------
	if ( !front_grid )
	{
		glPushMatrix();
		glTranslatef( 0.0, 0.0, -10.0 );
		grid -> draw();
		glPopMatrix();
	}
	
	// TODO: sacar la matrix a partir de las rotaciones y escalaciones...
	glGetDoublev( GL_MODELVIEW_MATRIX, matrix );
	
	// no hace falta habilitar el Z-buffer, si los objetos a dibujar estan ordenados por el valor de Z
	glDisable( GL_DEPTH_TEST );
	
	//----------- Draw the graphic components ---------------
	if ( KTStatus -> currentKeyFrame() != NULL )
		graphic_list = KTStatus -> currentKeyFrame() -> getDrawing() -> graphicComponents();
	else
		graphic_list.clear();
	
	for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		graphic -> draw();
	
	if ( selected_graphic && current_graphic != NULL && KTStatus -> currentKeyFrame() != NULL )
		markSelected();
	
	//----------- Draw the grid ---------------
	if ( front_grid )
	{
		glPushMatrix();
		glTranslatef( 0.0, 0.0, 10.0 );
		grid -> draw();
		glPopMatrix();
	}
	
	glPopMatrix();
	
	if ( !graphic_list.isEmpty() /*&& k_toon -> timeline() != NULL && k_toon -> exposureSheet() != NULL*/ )
	{
		emit wasDrawn( true );
		if ( KTStatus -> currentILayer() -> selectedFrame() != NULL )
			KTStatus -> currentILayer() -> selectedFrame() -> setHasDrawing( true );
	}
	else if ( graphic_list.isEmpty() /*&& k_toon -> timeline() != NULL && k_toon -> exposureSheet() != NULL*/ )
	{
		emit wasDrawn( false );
		if ( KTStatus -> currentILayer() -> selectedFrame() != NULL )
			KTStatus -> currentILayer() -> selectedFrame() -> setHasDrawing( false );
	}
	glEnable( GL_DEPTH_TEST );
	
	if ( current_graphic == NULL )
		markSelected( newPosition(), oldPosition() );

}

//Mark the selected graphic
void DrawingArea::markSelected()
{
	current_graphic -> calculateTopLeft();
	current_graphic -> calculateBottomRight();
	QPoint topLeft = current_graphic -> topLeft();
	QPoint bottomRight = current_graphic -> bottomRight();
	topLeft.setX( topLeft.x() - 25 );
	topLeft.setY( topLeft.y() - 15 );
	bottomRight.setX( bottomRight.x() + 15 );
	bottomRight.setY( bottomRight.y() + 25 );
	
	int x = topLeft.x();
	int y = topLeft.y();
	glPushMatrix();
	glTranslatef( current_graphic -> getTranslate().x(), current_graphic -> getTranslate().y(), 0.0 );
	
	glTranslatef( ( topLeft.x() + bottomRight.x() ) / 2,
			( topLeft.y() + bottomRight.y() ) / 2, 0.0 );
	
	glRotatef( current_graphic -> rotationAngle(), 0.0, 0.0, 1.0 );
	
	glTranslatef( ( -1 ) * ( topLeft.x() + bottomRight.x() ) / 2,
			( -1 ) * ( topLeft.y() + bottomRight.y() ) / 2, 0.0 );
	
	for ( int i = 1; i < 9; i++ )
	{
		glPushMatrix();
		glTranslatef( x, y, current_graphic -> getZ() );
		glRotatef( 45 * ( i - 2 ), 0.0, 0.0, 1.0 );
		//glPushName( id_arrow_texture[i] );
			glScalef( 1/scaleX(), 1/scaleY(), 1.0 );
			glCallList( id_arrow_texture[i] );
			glScalef( scaleX(), scaleY(), 1.0 );
		//glPopName();
		glPopMatrix();
		if ( i == 1 || i == 2 )
		{
		x += ( bottomRight.x() - topLeft.x() ) / 2;
		if ( i == 1 )
			y -= 10;
		}
		if ( i == 3 || i == 4 )
		{
		y += ( bottomRight.y() - topLeft.y() ) / 2;
		if ( i == 3 )
			x += 10;
		}
		if ( i == 5 || i == 6 )
		{
		x -= ( bottomRight.x() - topLeft.x() ) / 2;
		if ( i == 5 )
			y += 10;
		}
		if ( i == 7 )
		{
		y -= ( bottomRight.y() - topLeft.y() ) / 2;
		x -= 10;
		}
	}
	x = topLeft.x() + ( bottomRight.x() - topLeft.x() ) / 2;
	y = topLeft.y() + ( bottomRight.y() - topLeft.y() ) / 2;
	glPushMatrix();
	glTranslatef( x - 5, y - 15, current_graphic -> getZ() );
	glPushName( id_spin_texture );
	glScalef( 1/scaleX(), 1/scaleY(), 1.0 );
	glCallList( id_spin_texture );
	glScalef( scaleX(), scaleY(), 1.0 );
	glPopName();
	glPopMatrix();
	glPopMatrix();
}

void DrawingArea::markSelected( QPoint mouse_press, QPoint mouse_release )
{
	glPushMatrix();
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 1, 0x0F0F );
	glColor4f( 0.0, 0.0, 1.0, 1.0 );
	glLineWidth( 1.0 );
	glBegin( GL_LINE_LOOP );
	glVertex2f( mouse_press.x(), mouse_press.y() );
	glVertex2f( mouse_release.x(), mouse_press.y() );
	glVertex2f( mouse_release.x(), mouse_release.y() );
	glVertex2f( mouse_press.x(), mouse_release.y() );
	glEnd();
	glDisable( GL_LINE_STIPPLE );
	glPopMatrix();
}

//Process the selection buffer
void DrawingArea::processHits( GLint hits, GLuint buffer[] )
{
	int i, j;
	GLuint names, *ptr, aux = 0;
	unsigned long int z1, z2, z = ULONG_MAX;
	GLGraphicComponent *graphic;
	ptr = ( GLuint * ) buffer;
	for ( i = 0; i < hits; i++ )
	{           // for each hit
		names = *ptr;
		ptr++;
		z1 = *ptr;
		ptr++;
		z2 = *ptr;
		if ( names > 0 )
		{
			if ( z >= z1 )
			{
				z = z1;
				aux = *( ptr + 1 );
			}
			if ( z >= z2 )
			{
				z = z2;
				aux = *( ptr + 1 );
			}
		}
	
		ptr++;
		for ( j = 0; j < ( int )names; j++)
			ptr++;
	}
	GLGraphicComponent *prev_graphic = current_graphic;
	
	// poner estas variables en NULL o falso, SOLO si no se ha seleccionado un objeto o una flecha
	
	current_graphic = 0;
	selected_graphic = false;
	
	if( hits > 0 )
	{
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		{
			if ( graphic -> selectionName() == aux )
			{
			current_graphic = graphic_list.at( graphic_list.at() );
			selected_graphic = true;
			break;
			}
		}
		// TODO: si se ha seleccionado un objeto que no esta dentro de la lista
		// de graphic_list, mirar si es una flecha! y tomar el ultimo objeto seleccionado
		for( int i=0; i<9; i++ )
		{
			if( aux == id_arrow_names[i] )
			{
	//			qDebug( "fecha %d seleccionada", i );
				current_graphic = prev_graphic;
				selected_graphic = true;
			}
		}
		current_graphic_list.append( current_graphic );
	}
}

void DrawingArea::drawSelected( QMouseEvent *mouse_event )
{
        Q_CHECK_PTR( mouse_event );

	GLint viewport[4];
	GLdouble delta = 5.0;

	selected_graphic = true;

	glGetIntegerv( GL_VIEWPORT, viewport );

	glSelectBuffer( sizeof( selectionBuffer ), selectionBuffer );
	glRenderMode( GL_SELECT );
	glInitNames();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix( mouse_event -> x(), viewport[3] - mouse_event -> y(), delta, delta, viewport );
	glOrtho( 0.0, width(), height(), 0.0, -10.0, 10.0 );

	glMatrixMode( GL_MODELVIEW );
	paintGL();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	int nSeleccion = glRenderMode( GL_RENDER );

	processHits ( nSeleccion, selectionBuffer );
}

void DrawingArea::drawSelected( QPoint mouse_press, QPoint mouse_release )
{
	GLGraphicComponent *graphic;
	current_graphic_list.clear();
	invertMatrix();
        for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
	{
	        bool included = graphic -> graphicIncluded( mapPointToMatrix( mouse_press ), mapPointToMatrix( mouse_release ) );
		if ( included )
		{
		    current_graphic = NULL;
		    current_graphic_list.append( graphic );
		}
	}
}

//Draw the previous and next onion skin
void DrawingArea::drawOnionSkinsAndLightTable()
{
	GLGraphicComponent *onion;
	GLGraphicComponent *graphic;
	
	int previous = KTStatus -> currentPreviousOnionSkin();
	int next = KTStatus -> currentNextOnionSkin();
	
	QColor b_color = KTStatus -> currentBackgroundColor();
	
	previous_onion_skin_1.clear();
	previous_onion_skin_2.clear();
	previous_onion_skin_3.clear();
	next_onion_skin_1.clear();
	next_onion_skin_2.clear();
	next_onion_skin_3.clear();
	
	if ( previous == 3 || next == 3 )
	{
		if ( previous == 3 )
		slotPrevious3OnionSkin();
		if ( next == 3 )
		slotNext3OnionSkin();
		glPushMatrix();
			glTranslatef( 0.0, 0.0, -8.0 );
			if ( previous == 3 )
			for ( onion = previous_onion_skin_3.first(); onion; onion = previous_onion_skin_3.next() )
				onion -> draw();
			if ( next == 3 )
			for ( onion = next_onion_skin_3.first(); onion; onion = next_onion_skin_3.next() )
				onion -> draw();
			glColor4f( ( float )b_color.red() / 255.0, ( float )b_color.green() / 255.0, ( float )b_color.blue() / 255.0, 0.4 );
			glTranslatef( 0.0, 0.0, 1.0 );
			glDepthMask( GL_FALSE );
			glRectf( -10000.0, -10000.0, 10000.0, 10000.0 );
			glDepthMask( GL_TRUE );
		glPopMatrix();
	}
	
	if ( previous >= 2 || next >= 2 )
	{
		if ( previous >= 2 )
		slotPrevious2OnionSkin();
		if ( next >= 2 )
		slotNext2OnionSkin();
		glPushMatrix();
			glTranslatef( 0.0, 0.0, -7.0 );
			if ( previous >= 2 )
			for ( onion = previous_onion_skin_2.first(); onion; onion = previous_onion_skin_2.next() )
				onion -> draw();
			if ( next >= 2 )
			for ( onion = next_onion_skin_2.first(); onion; onion = next_onion_skin_2.next() )
				onion -> draw();
			glColor4f( ( float )b_color.red() / 255.0, ( float )b_color.green() / 255.0, ( float )b_color.blue() / 255.0, 0.5 );
			glTranslatef( 0.0, 0.0, 1.0 );
			glDepthMask( GL_FALSE );
			glRectf( -10000.0, -10000.0, 10000.0, 10000.0 );
			glDepthMask( GL_TRUE );
		glPopMatrix();
	}
	
	if ( previous >= 1 || next >= 1 || light_table )
	{
		if ( previous >= 1 )
		slotPreviousOnionSkin();
		if ( next >= 1 )
		slotNextOnionSkin();
		glPushMatrix();
			glTranslatef( 0.0, 0.0, -6.0 );
			if ( previous >= 1 )
			for ( onion = previous_onion_skin_1.first(); onion; onion = previous_onion_skin_1.next() )
				onion -> draw();
			if ( next >= 1 )
			for ( onion = next_onion_skin_1.first(); onion; onion = next_onion_skin_1.next() )
				onion -> draw();
			if( light_table )
				for ( graphic = light_table_list.first(); graphic; graphic = light_table_list.next() )
					graphic -> draw();
			glTranslatef( 0.0, 0.0, 1.0 );
			glColor4f( ( float )b_color.red() / 255.0, ( float )b_color.green() / 255.0, ( float )b_color.blue() / 255.0, 0.6 );
			glDepthMask( GL_FALSE );
			glRectf( -10000.0, -10000.0, 10000.0, 10000.0 );
			glDepthMask( GL_TRUE );
		glPopMatrix();
	}
}

// Cuando presiona el maus selecciona un herramienta
void DrawingArea::mousePressEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	selected_all = false;
	
	if ( KTStatus -> currentKeyFrame() == NULL /*|| k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked()*/ )
	{
		mouse_event -> ignore();
		return;
	}
	emit selected();
	GLControl::mousePressEvent( mouse_event );
	selected_graphic = false;
	current_graphic_list.clear();
	
	if ( mouse_event -> button() == Qt::RightButton )
	{
		drawSelected( mouse_event );
		if ( current_graphic != NULL )
		{
			if ( right_click_menu != NULL )
				delete right_click_menu;
			right_click_menu = new QPopupMenu( this );
			right_click_menu -> setFont( QFont( "helvetica", 10 ) );
			right_click_menu -> insertItem( QPixmap( copy_xpm ), tr( "&Copy" ), this, SLOT( slotCopy() ) );

			right_click_menu -> insertItem( QPixmap( cut_xpm ), tr( "C&ut" ), this, SLOT( slotCut() ) );
			right_click_menu -> insertItem( tr( "&Delete" ), this, SLOT( slotDelete() ) );
			right_click_menu -> insertSeparator();
			//Order Submenu
			QPopupMenu * order = new QPopupMenu( this );
			order -> setFont( QFont( "helvetica", 10 ) );
			order -> insertItem( QPixmap( bring_to_front_xpm ), tr( "&Bring to Front" ), this, SLOT( slotBringToFront() ) );
			order -> insertItem( QPixmap( send_to_back_xpm ), tr( "&Send to Back" ), this, SLOT( slotSendToBack() ) );
			order -> insertItem( QPixmap( one_forward_xpm ), tr( "One Step &Forward" ), this, SLOT( slotOneStepForward() ) );
			order -> insertItem( QPixmap( one_backward_xpm ), tr( "One Step B&ackward" ), this, SLOT( slotOneStepBackward() ) );
			right_click_menu -> insertItem( tr( "&Order" ), order );
			//Align Submenu
			QPopupMenu * align = new QPopupMenu( this );
// 			align -> setFont( QFont( "helvetica", 10 ) );
			align -> insertItem( QPixmap( align_l_xpm ), tr( "&Left" ), this, SLOT( slotAlignLeft() ) );
			align -> insertItem( QPixmap( align_cv_xpm ), tr( "&Center Vertically" ), this, SLOT( slotCenterVertically() ) );
			align -> insertItem( QPixmap( align_r_xpm ), tr( "&Right" ), this, SLOT( slotAlignRight() ) );
			align -> insertSeparator();
			align -> insertItem( QPixmap( align_t_xpm ), tr( "&Top" ), this, SLOT( slotAlignTop() ) );
			align -> insertItem( QPixmap( align_ch_xpm ), tr( "Center &Horizontally" ), this, SLOT( slotCenterHorizontally() ) );
			align -> insertItem( QPixmap( align_b_xpm ), tr( "&Bottom" ), this, SLOT( slotAlignBottom() ) );
			right_click_menu -> insertItem(tr( "A&lign" ), align );
			// Transform Submenu
			QPopupMenu * transform = new QPopupMenu( this );
			transform -> setFont( QFont( "helvetica", 10 ) );
			transform -> insertItem( tr( "Flip &Horizontally" ), this, SLOT( slotFlipHorizontally() ) );
			transform -> insertItem( tr( "Flip &Vertically" ), this, SLOT( slotFlipVertically() ) );
			transform -> insertSeparator();
			transform -> insertItem( tr( "&Rotate 90 CW" ), this, SLOT( slotRotateCW90() ) );
			transform -> insertItem( tr( "R&otate 90 CCW" ), this, SLOT( slotRotateCCW90() ) );
			transform -> insertItem( tr( "Rotate &180" ), this, SLOT( slotRotate180() ) );
			transform -> insertSeparator();
			transform -> insertItem( QPixmap( perspective_xpm ), tr( "&Perspective" ), this, SLOT( slotPerspectiveSelection() ) );
			right_click_menu -> insertItem( tr( "&Transform" ), transform );
			right_click_menu -> insertSeparator();
			right_click_menu -> insertItem( tr( "Add to Li&brary" ), this, SLOT( slotAddToLibrary() ) );
			right_click_menu -> insertSeparator();
			right_click_menu -> insertItem( QPixmap( properties_xpm ), tr( "P&roperties..." ), this, SLOT( slotProperties() ) );
			right_click_menu -> exec( QCursor::pos() );
		}
		else
		{
			right_click_menu = new QPopupMenu( this );
			right_click_menu -> insertItem( QPixmap( undo_xpm ), tr( "&Undo" ), this, SLOT( slotUndo() ) );
			right_click_menu -> insertItem( QPixmap( redo_xpm ), tr( "&Redo" ), this, SLOT( slotRedo() ) );
			right_click_menu -> insertSeparator();
			right_click_menu -> insertItem( QPixmap( paste_xpm ), tr( "&Paste" ), this, SLOT( slotPaste() ) );
			right_click_menu -> insertItem( tr( "P&aste in Place" ), this, SLOT( slotPasteInPlace() ) );
			right_click_menu -> exec( QCursor::pos() );
		}
	}
	else
	{
	current_outline_color = KTStatus -> currentOutlineColor();
	current_fill_color = KTStatus -> currentFillColor();
	current_brush = KTStatus -> currentBrush();
	
	switch ( current_cursor )
		{
		case Tools::NORMAL_SELECTION:
			drawSelected( mouse_event );
			invertMatrix();
			bezier = false;
			break;
		case Tools::CONTOUR_SELECTION:
			{
			drawSelected( mouse_event );
			if ( current_graphic != NULL )
			current_graphic -> contourSelectionPressOption();
			}
			break;
		case Tools::BRUSH:
			{
			qDebug("Brush");
			invertMatrix();
			current_graphic = new GLBrush( this, mapPointToMatrix( oldPosition() ), *current_outline_color,*current_brush, mapPointToMatrix( newPosition() ) );
			( ( GLBrush * )( current_graphic ) ) -> smoothnessBrush( 2 );
			
			( ( GLBrush * )( current_graphic ) ) -> setPressureBrush( ( int )( ( pressureTablet() * 100 ) / 255 ) );
			addGraphicComponent( current_graphic, false );
			bezier = false;
			break;
			}
		case Tools::PENCIL:
			{
			invertMatrix();
			current_graphic = new GLPencil( this, mapPointToMatrix( oldPosition() ), *current_outline_color,
							*current_brush, mapPointToMatrix( newPosition() ) );
			addGraphicComponent( current_graphic, false );
			bezier = false;
			break;
			}
		case Tools::PEN:
			invertMatrix();
			if ( !bezier )
			{
				current_graphic = new GLPen( this, mapPointToMatrix( oldPosition() ), *current_outline_color,
							*current_brush, mapPointToMatrix( newPosition() ) );
				QPoint * new_point = new QPoint( mouse_event -> x(), mouse_event -> y() );
				try {
				( ( GLPen * )( current_graphic ) ) -> setBezierPoint( *new_point );
				}
				catch(...)
				{
				delete new_point;
				throw;
				}
				addGraphicComponent( current_graphic, false );
				bezier = true;
			}
			else
			{
				mouse_event -> accept();
				QPoint new_point1( mouse_event -> x(), mouse_event -> y() );
				( ( GLPen * )( current_graphic ) ) -> setControlPen( QPoint( mapPointToMatrix( new_point1 ) ),
										QPoint( mapPointToMatrix( new_point1 )) );
				
				QPoint* new_bezier_point = new QPoint( mapPointToMatrix( new_point1 ) );
				try {
				( ( GLPen * )( current_graphic ) ) -> setBezierPoint( *new_bezier_point );
				}
				catch(...)
				{
				delete new_bezier_point;
				throw;
				}
			}
			break;
		case Tools::LINE:
			{
			invertMatrix();
			current_graphic = new GLLine( this, mapPointToMatrix( oldPosition() ), *current_outline_color,
						*current_brush, mapPointToMatrix( newPosition() ) );
			addGraphicComponent( current_graphic, false );
			bezier = false;
			break;
			}
		case Tools::RECTANGLE:
			{
			invertMatrix();
			current_graphic = new GLRectangle( this, mapPointToMatrix( oldPosition() ), *current_outline_color,
							*current_brush, mapPointToMatrix( newPosition() ), *current_fill_color );
			addGraphicComponent( current_graphic, false );
			bezier = false;
			break;
			}
		case Tools::ELLIPSE:
			{
			invertMatrix();
			current_graphic = new GLEllipse( this, mapPointToMatrix( oldPosition() ), *current_outline_color,
							*current_brush, mapPointToMatrix( newPosition() ), *current_fill_color );
			addGraphicComponent( current_graphic, false );
			bezier = false;
			break;
			}
		case Tools::CONTOUR_FILL:
			drawSelected( mouse_event );
			if ( current_graphic != NULL )
				current_graphic -> setOutlineColor( *current_outline_color );
			break;
		case Tools::FILL:
			drawSelected( mouse_event );
			if ( current_graphic != NULL )
			current_graphic -> fillPressOption( *current_fill_color );
			break;
		case Tools::REMOVE_FILL:
			drawSelected( mouse_event );
			if ( current_graphic != NULL )
			current_graphic -> removeFillPressOption();
			break;
		case Tools::DROPPER:
			drawSelected( mouse_event );
			emit colorGrabbed( grabColor( mouse_event -> pos() ) );
			break;
		case Tools::ERASER:
			break;
		case Tools::SLICER:
			break;
		case Tools::MAGNIFYING_GLASS:
			break;
		case Tools::HAND:
			mouse_event -> accept();
			setOldPosition ( mouse_event -> pos() );
			break;
		default:
			bezier = false;
			break;
		}
	}
	updateGL();
}

void DrawingArea::mouseReleaseEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	
	if ( KTStatus -> currentKeyFrame() == NULL/* || k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked()*/ )
	{
	mouse_event -> ignore();
	return;
	}
	current_graphic_list.clear();
	
	switch ( current_cursor )
		{
		case Tools::NORMAL_SELECTION:
			if ( newPosition() != oldPosition() )
				drawSelected( oldPosition(), newPosition() );
			else
				drawSelected( mouse_event );
			//invertMatrix();
			bezier = false;
			break;
		case Tools::BRUSH:
			//( ( GLBrush * )( current_graphic ) ) -> smoothnessBrush( current_brush -> smoothnessBrush() );
			break;
		case Tools::MAGNIFYING_GLASS:
			if ( newPosition() == oldPosition() )
			{
			if ( shift_pressed )
				slotZoomOut();
			else
				slotZoomIn();
			}
			else
			{
			float x = newPosition().x() < oldPosition().x() ? newPosition().x() : oldPosition().x();
			float y = newPosition().y() < oldPosition().y() ? newPosition().y() : oldPosition().y();
			glTranslatef(-x, -y, 0);
			//setScaleX( newPosition().x() / 10 - oldPosition().x() / 10 );
			//setScaleY( newPosition().y() / 10 - oldPosition().y() / 10 );
			}
			break;
		default:
		break;
		}
	
	if ( current_graphic != NULL && current_graphic -> kindGraphic() != GLGraphicComponent::GC_PEN )
		selected_graphic = true;
	
	GLControl::mouseReleaseEvent( mouse_event );
	
	updateGL();
}


void DrawingArea::mouseMoveEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	
	if ( KTStatus -> currentKeyFrame() == NULL )
	{
	mouse_event -> ignore();
	return;
	}
	current_brush = KTStatus -> currentBrush();
	current_graphic_list.clear();
	
	if( current_graphic )
	{
	switch ( current_cursor )
		{
		case Tools::NORMAL_SELECTION:
			{
			invertMatrix();
			mouse_event -> accept();
			QPoint old_pos;
	
			old_pos.setX( newPosition().x() );
			old_pos.setY( newPosition().y() );
	
			setNewPosition ( mouse_event -> pos() );
			//qDebug("(%d - %d)(%d - %d)", newPosition().x(), newPosition().y(), oldPosition().x(),oldPosition().y() );
			current_graphic -> translateGraphic( mapPointToMatrix( newPosition() ), mapPointToMatrix( old_pos ) );
			modifyDocument( true );
			}
			break;
		case Tools::CONTOUR_SELECTION:
			{
			drawSelected( mouse_event );
			mouse_event -> accept();
			QPoint new_point( mouse_event -> x(),  mouse_event -> y() );
			if ( current_graphic != NULL )
			current_graphic -> contourSelectionMoveOption( QPoint( mapPointToMatrix( new_point ) ) );
			}
			break;
		case Tools::BRUSH:
			{
			invertMatrix();
			mouse_event -> accept();
			QPoint new_point( mouse_event -> x(),  mouse_event -> y() );
			current_graphic -> brushMoveOption( *( new QPoint( mapPointToMatrix( new_point ) ) ) );
			}
			( ( GLBrush * )( current_graphic ) ) -> setPressureBrush( ( int )( ( pressureTablet() * 100 ) / 255 ) );
			break;
		case Tools::PENCIL:
			{
			invertMatrix();
			mouse_event -> accept();
			QPoint new_point( mouse_event -> x(),  mouse_event -> y() );
			current_graphic -> pencilMoveOption( QPoint( mapPointToMatrix( new_point ) ) );
			}
			break;
		case Tools::PEN:
			{
			invertMatrix();
			mouse_event -> accept();
			QPoint new_point( mouse_event -> x(),  mouse_event -> y() );
			current_graphic -> penMoveOption( QPoint( mapPointToMatrix( new_point ) ) );
			( ( GLPen * )( current_graphic ) ) -> setWidthPoint( *current_brush );
			}
			break;
		case Tools::LINE:
			{
			invertMatrix();
			mouse_event -> accept();
			setNewPosition ( mouse_event -> pos() );
			current_graphic -> lineMoveOption( mapPointToMatrix( newPosition() ) );
			}
			break;
		case Tools::RECTANGLE:
			{
			invertMatrix();
			mouse_event -> accept();
			setNewPosition ( mouse_event -> pos() );
			current_graphic -> rectangleMoveOption( mapPointToMatrix( newPosition() ) );
			}
			break;
		case Tools::ELLIPSE:
			{
			invertMatrix();
			mouse_event -> accept();
			setNewPosition ( mouse_event -> pos() );
			current_graphic -> ellipseMoveOption( mapPointToMatrix( newPosition() ) );
			}
			break;
		case Tools::REMOVE_FILL:
			break;
		case Tools::DROPPER:
			break;
		case Tools::ERASER:
			break;
		case Tools::SLICER:
			break;
		case Tools::MAGNIFYING_GLASS:
			mouse_event -> accept();
			setNewPosition ( mouse_event -> pos() );
			break;
		case Tools::HAND:
			mouse_event -> accept();
			setOldPosition ( newPosition() );
			setNewPosition ( mouse_event -> pos() );
			setTranslationX( newPosition().x() - oldPosition().x() );
			setTranslationY( newPosition().y() - oldPosition().y() );
			glTranslatef( translationX(), translationY(), 0.0 );
			break;
		default:
			break;
		}
	}
	else
	{
	switch ( current_cursor )
		{
		case Tools::NORMAL_SELECTION:
			mouse_event -> accept();
			//setOldPosition ( newPosition() );
			setNewPosition ( mouse_event -> pos() );
			break;
		case Tools::CONTOUR_SELECTION:
			break;
		case Tools::BRUSH:
			break;
		case Tools::PENCIL:
			break;
		case Tools::PEN:
			break;
		case Tools::LINE:
			break;
		case Tools::RECTANGLE:
			break;
		case Tools::ELLIPSE:
			break;
		case Tools::CONTOUR_FILL:
			break;
		case Tools::FILL:
			break;
		case Tools::REMOVE_FILL:
			break;
		case Tools::DROPPER:
			break;
		case Tools::ERASER:
			break;
		case Tools::SLICER:
			break;
		case Tools::MAGNIFYING_GLASS:
			mouse_event -> accept();
			setNewPosition ( mouse_event -> pos() );
			break;
		case Tools::HAND:
			mouse_event -> accept();
			setOldPosition ( newPosition() );
			setNewPosition ( mouse_event -> pos() );
			setTranslationX( newPosition().x() - oldPosition().x() );
			setTranslationY( newPosition().y() - oldPosition().y() );
			glTranslatef( translationX(), translationY(), 0.0 );
			break;
		default: break;
		}
	}
	GLControl::mouseMoveEvent( mouse_event );
	updateGL();
}

void DrawingArea::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	
	if ( KTStatus -> currentKeyFrame() == NULL /*|| k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked()*/ )
	{
	mouse_event -> ignore();
	return;
	}
	current_graphic_list.clear();
	
	if( current_graphic )
	{
		switch ( current_cursor )
		{
			case Tools::PEN:
			{
				mouse_event -> accept();
				QPoint new_point1( mouse_event -> x(),  mouse_event -> y() );
				QPoint new_point2( mouse_event -> x(),  mouse_event -> y() );
				( ( GLPen * )( current_graphic ) ) -> setControlPen( *( new QPoint( mapPointToMatrix( new_point1 ) ) ), *( new QPoint( mapPointToMatrix( new_point2 ) ) ) );
				graphic_list.remove( current_graphic );
				addGraphicComponent( current_graphic, false );
				current_graphic = NULL;
				selected_graphic = true;
				bezier = false;
				break;
			}
			default:
				break;
		}
	}
	updateGL();
}

void DrawingArea::keyPressEvent( QKeyEvent *key_event )
{
	Q_CHECK_PTR( key_event );
	
	if ( selected_all )
	{
		current_graphic_list.clear();
		if ( KTStatus -> currentKeyFrame() != NULL )
			current_graphic_list = KTStatus -> currentKeyFrame() -> getDrawing() -> graphicComponents();
		current_graphic = NULL;
	}
	else if ( current_graphic != NULL )
	{
		current_graphic_list.clear();
		current_graphic_list.append( current_graphic );
	}
	bezier = false;
	
	if ( KTStatus -> currentKeyFrame() == NULL /*|| k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked()*/ )
	{
	key_event -> ignore();
	return;
	}
	if ( key_event -> key() == Qt::Key_Space )
	{
		using_hand = true;
		previous_cursor = KTStatus -> currentCursor();
// 		k_toon -> slotHand(); //FIXME: krawek
	}
	if ( key_event -> key() == Qt::Key_Shift )
	{
		shift_pressed = true;
	}
	
	if ( current_graphic_list.count() != 0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> keyPressEvent( key_event );
	}
	updateGL();
}

void DrawingArea::keyReleaseEvent( QKeyEvent *key_event )
{
	Q_CHECK_PTR( key_event );
	
	if ( KTStatus -> currentKeyFrame() == NULL/* || k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked()*/ )
	{
	key_event -> ignore();
	return;
	}
	
	if ( using_hand )
	{
		emit useTool(previous_cursor);
	}
	
	using_hand = false;
	shift_pressed = false;
	if ( current_graphic )
		current_graphic -> keyReleaseEvent( key_event );
	updateGL();
}

void DrawingArea::closeEvent( QCloseEvent *close_event )
{
	Q_CHECK_PTR( close_event );
	
	//If the document has not been saved, display a closing confirmation
	if ( modified )
	{
		switch( QMessageBox::warning( this, tr( "Save Changes" ), tr( "Save changes to %1?" ).arg( caption().remove( " (*)" ) ),
			tr( "Yes" ), tr( "No" ), tr( "Cancel" ) ) )
		{
		case 0:
			close_event -> accept();
			emit closed();
			break;
		case 1:
			close_event -> accept();
			modified = false;
			emit closed();
			break;
		default:
			close_event -> ignore();
			break;
		}
	}
	else
	{
		close_event -> accept();
		emit closed();
	}
}

void DrawingArea::setMaxVertical( const GLuint & max_vert )
{
	Q_ASSERT( max_vert > 0 );
	
	max_vertical = max_vert;
	grid -> setMaxHeight( max_vert );
}

void DrawingArea::setMaxHorizontal( const GLuint & max_hor )
{
	Q_ASSERT( max_hor > 0 );
	
	max_horizontal = max_hor;
	grid -> setMaxWidth( max_hor );
}

void DrawingArea::setNumberLines( const GLuint & num_lines )
{
	Q_ASSERT( num_lines > 0 );
	
	number_lines = num_lines;
	grid -> setNumberLines( num_lines );
}

void DrawingArea::setVerticalMargin( const GLuint & marg_vert )
{
	Q_ASSERT( marg_vert > 0 );
	
	vertical_margin = marg_vert;
}

void DrawingArea::setHorizontalMargin( const GLuint & marg_hor )
{
	Q_ASSERT( marg_hor > 0 );
	
	horizontal_margin = marg_hor;
}

void DrawingArea::setHeight( const GLuint & height )
{
	Q_ASSERT( height > 0 );
	
	h = height;
}

void DrawingArea::setWidth( const GLuint & width )
{
	Q_ASSERT( width > 0 );
	
	w = width;
}

void DrawingArea::setZoomLines( const bool & zoom )
{
	zoom_lines = zoom;
	grid -> setZoomLines( zoom );
}

void DrawingArea::setNtscZone( const bool & ntsc )
{
	ntsc_zone = ntsc;
	grid->setNtscZone( ntsc );
}

void DrawingArea::setLightTable( const bool & ltable )
{
	light_table = ltable;
}

void DrawingArea::pasteBrush( QString brush )
{
	QString number1, number2, count;
	QPoint point;
	KTColor o_color;
	Brush br;
	int i, number_points;
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	number2 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	number2 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	br.setThicknessMinBrush( number1.toInt() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	br.setThicknessMaxBrush( number1.toInt() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	br.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLBrush( this, oldPosition(), o_color, br, newPosition() );
	( ( GLBrush * )( current_graphic ) ) -> smoothnessBrush( current_brush -> smoothnessBrush() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number1.length() + 1 );
	
	number2 = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, number2.length() + 1 );
	
	current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	
	//List of points. first is the number points, after point (x, y)
	count = brush.left( brush.find( ' ', 0 ) );
	brush.remove( 0, count.length() + 1 );
	number_points = count.toInt();
	
	for ( i = 0; i < number_points; i++ )
	{
		number1 = brush.left( brush.find( ' ', 0 ) );
		brush.remove( 0, number1.length() + 1 );
	
		number2 = brush.left( brush.find( ' ', 0 ) );
		brush.remove( 0, number2.length() + 1 );
	
		( ( GLBrush * ) ( current_graphic ) ) -> setEndBrush( QPoint( number1.toInt(), number2.toInt() ) );
	}
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::pastePencil( QString pencil )
{
	QString number1, number2, count;
	QPoint point;
	KTColor o_color;
	Brush brush;
	int i, number_points;
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	number2 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	number2 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	brush.setThicknessMinBrush( number1.toInt() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	brush.setThicknessMaxBrush( number1.toInt() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	brush.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLPencil( this, oldPosition(), o_color, brush, newPosition() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number1.length() + 1 );
	
	number2 = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, number2.length() + 1 );
	
	current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	
	//List of points. first is the number points, after point (x, y)
	count = pencil.left( pencil.find( ' ', 0 ) );
	pencil.remove( 0, count.length() + 1 );
	number_points = count.toInt();
	
	for ( i = 0; i < number_points; i++ )
	{
		number1 = pencil.left( pencil.find( ' ', 0 ) );
		pencil.remove( 0, number1.length() + 1 );
	
		number2 = pencil.left( pencil.find( ' ', 0 ) );
		pencil.remove( 0, number2.length() + 1 );
	
		( ( GLPencil * ) ( current_graphic ) ) -> setEndPencil( QPoint( number1.toInt(), number2.toInt() ) );
	}
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::pastePen( QString pen )
{
	QString number1, number2, count;
	QPoint point;
	KTColor o_color;
	Brush brush;
	int i, number_points;
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	number2 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	number2 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	brush.setThicknessMinBrush( number1.toInt() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	brush.setThicknessMaxBrush( number1.toInt() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	brush.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLPen( this, oldPosition(), o_color, brush, newPosition() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number1.length() + 1 );
	
	number2 = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, number2.length() + 1 );
	
	current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	
	//List of points. first is the number points, after point (x, y)
	count = pen.left( pen.find( ' ', 0 ) );
	pen.remove( 0, count.length() + 1 );
	number_points = count.toInt();
	
	//The control points
	( ( GLPen * ) ( current_graphic ) ) -> clearPoints();
	for ( i = 0; i < number_points; i++ )
	{
		number1 = pen.left( pen.find( ' ', 0 ) );
		pen.remove( 0, number1.length() + 1 );
	
		number2 = pen.left( pen.find( ' ', 0 ) );
		pen.remove( 0, number2.length() + 1 );
	
		( ( GLPen * ) ( current_graphic ) ) -> setOneControlPen( QPoint( number1.toInt() , number2.toInt() ) );
	}
	
	//The bezier points
	for ( i = 0; i < number_points / 2; i++ )
	{
		number1 = pen.left( pen.find( ' ', 0 ) );
		pen.remove( 0, number1.length() + 1 );
	
		number2 = pen.left( pen.find( ' ', 0 ) );
		pen.remove( 0, number2.length() + 1 );
	
		QPoint * new_point = new QPoint( number1.toInt(), number2.toInt() );
		try {
		( ( GLPen * )( current_graphic ) ) -> setBezierPoint( *new_point );
		}
		catch(...)
		{
		delete new_point;
		throw;
		}
	}
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::pasteLine( QString line )
{
	QString number1, number2;
	KTColor o_color;
	Brush brush;
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	number2 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	number2 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	brush.setThicknessMinBrush( number1.toInt() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	brush.setThicknessMaxBrush( number1.toInt() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	brush.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLLine( this, oldPosition(), o_color, brush, newPosition() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number1.length() + 1 );
	
	number2 = line.left( line.find( ' ', 0 ) );
	line.remove( 0, number2.length() + 1 );
	
	current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::pasteRectangle( QString rectangle )
{
	QString number1, number2;
	KTColor o_color, f_color;
	Brush brush;
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	number2 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	number2 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	f_color.setColorRed( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	f_color.setColorGreen( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	f_color.setColorBlue( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	f_color.setColorAlpha( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	brush.setThicknessMinBrush( number1.toInt() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	brush.setThicknessMaxBrush( number1.toInt() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	brush.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLRectangle( this, oldPosition(), o_color, brush, newPosition(), f_color );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	( ( GLRectangle * )( current_graphic ) ) -> setPerfectRectangle( number1.toInt() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number1.length() + 1 );
	
	number2 = rectangle.left( rectangle.find( ' ', 0 ) );
	rectangle.remove( 0, number2.length() + 1 );
	
	current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::pasteEllipse( QString ellipse )
{
	QString number1, number2;
	KTColor o_color, f_color;
	Brush brush;
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	number2 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number2.length() + 1 );
	
	setOldPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	number2 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number2.length() + 1 );
	
	setNewPosition( QPoint( number1.toInt(), number2.toInt() ) );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	o_color.setColorRed( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	o_color.setColorGreen( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	o_color.setColorBlue( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	o_color.setColorAlpha( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	f_color.setColorRed( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	f_color.setColorGreen( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	f_color.setColorBlue( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	f_color.setColorAlpha( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	brush.setThicknessMinBrush( number1.toInt() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	brush.setThicknessMaxBrush( number1.toInt() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	brush.setSmoothnessBrush( number1.toInt() );
	
	current_graphic = new GLEllipse( this, oldPosition(), o_color, brush, newPosition(), f_color );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStipplePattern( number1.toUShort() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	current_graphic -> setStippleFactor( number1.toUShort() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	( ( GLEllipse * )( current_graphic ) ) -> setPerfectEllipse( number1.toInt() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	current_graphic -> setRotationAngle( number1.toFloat() );
	
	number1 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number1.length() + 1 );
	
	number2 = ellipse.left( ellipse.find( ' ', 0 ) );
	ellipse.remove( 0, number2.length() + 1 );
	
	// KRAWEK
	//current_graphic -> translateGraphic( number1.toInt(), number2.toInt() );
	current_graphic -> translateGraphic( number2.toInt(), number1.toInt() );
	
	addGraphicComponent( current_graphic, false );
}

void DrawingArea::addGraphicComponent( GLGraphicComponent *graphic_component, bool update )
{
#ifdef DEBUG_ADDGRAPHIC
    qDebug("Adding graphic component!!");
    Q_CHECK_PTR( graphic_component );

    if ( graphic_component->kindGraphic() == GLGraphicComponent::GC_BRUSH )
        qDebug("Brush");
    
#endif

	history_graphic_list.clear();
	
	modifyDocument( true );
	GLfloat z = -10.0;
	if ( graphic_list.count() != 0 )
		{
		GLGraphicComponent *graphic;
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
			if ( graphic -> getZ() > z )
			z = graphic -> getZ();
		}
	else
		z = 0.0;
	
	graphic_component -> setZ( z + 0.0001 );
	graphic_list.append( graphic_component );
	GLDrawing *drawing = KTStatus -> currentKeyFrame() -> getDrawing();
	drawing -> setGraphicComponents( graphic_list );
	
	//drawing_list.append(drawing);
	
	current_graphic = graphic_component;
	
	if ( update )
		updateGL();
}
	
void DrawingArea::modifyDocument( bool modify )
{
	if ( modify )
	{
		if ( !modified )
		{
			modified = true;
			setCaption( caption() + " (*)" );
		}
	}
	else
	{
		modified = false;
		setCaption( caption().remove( " (*)" ) );
	}
}

void DrawingArea::loadImage( const QString & file_name )
{
	glEnable( GL_TEXTURE_2D );
	GLImage *image = new GLImage( this, file_name, QPoint( 20, 20 ), QPoint() );
	glDisable( GL_TEXTURE_2D );
	addGraphicComponent( image, false );
}

void DrawingArea::loadImageSequence( const QString & file_name )
{

}

//Makes a group from the selected objects of the drawing area
void DrawingArea::slotGroup()
{
	modifyDocument( true );
}

//Ungroups the previous grouped elements
void DrawingArea::slotUngroup()
{
	modifyDocument( true );
}

//Puts the selected object over all the objects in the drawing area
void DrawingArea::slotBringToFront()
{
	if( current_graphic )
	{
		modifyDocument( true );
		GLGraphicComponent *graphic;
		GLfloat z = current_graphic -> getZ();
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		{
			if ( graphic -> getZ() > z)
				z = graphic -> getZ();
			if ( graphic == current_graphic )
				current_graphic = graphic_list.take();
		}
		if ( z != current_graphic -> getZ() )
			current_graphic -> setZ( z + 0.0001 );
		graphic_list.append( current_graphic );
		KTStatus -> currentKeyFrame() -> getDrawing() -> setGraphicComponents( graphic_list );
	}
	
	updateGL();
}

//Sends the selected object to the drawing area's background
void DrawingArea::slotSendToBack()
{
	if( current_graphic )
	{
		modifyDocument( true );
		GLGraphicComponent *graphic;
		GLfloat z = current_graphic -> getZ();
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		{
			if ( graphic -> getZ() < z)
				z = graphic -> getZ();
			if ( graphic == current_graphic )
				current_graphic = graphic_list.take();
		}
		if ( z != current_graphic -> getZ() )
			current_graphic -> setZ( z - 0.0001 );
		graphic_list.prepend( current_graphic );
		KTStatus -> currentKeyFrame() -> getDrawing() -> setGraphicComponents( graphic_list );
	}
	updateGL();
}

//Moves the selected object one step forward into the drawing area
void DrawingArea::slotOneStepForward()
{
	if( current_graphic )
	{
		if ( current_graphic == graphic_list.getLast() )
			return;
		modifyDocument( true );
		GLGraphicComponent *graphic;
		int index = 0;
		GLfloat z = current_graphic -> getZ();
		GLfloat w = 9.0;
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
			if ( ( graphic -> getZ() > z ) && ( graphic -> getZ() < w ) )
				w = graphic -> getZ();
		for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		{
			if ( graphic -> getZ() == z )
			graphic -> setZ( w );
			else
			if ( graphic -> getZ() == w )
				graphic -> setZ( z );
			if ( graphic == current_graphic )
			{
				index = graphic_list.at();
				current_graphic = graphic_list.take();
			}
		}
		current_graphic -> setZ( z + 0.0001 );
		graphic_list.insert( index + 1,  current_graphic );
		KTStatus -> currentKeyFrame() -> getDrawing() -> setGraphicComponents( graphic_list );
	}
	updateGL();
}

//Moves the selected object one step backward into the drawing area
void DrawingArea::slotOneStepBackward()
{
	if( current_graphic )
	{
	if ( current_graphic == graphic_list.getFirst() )
		return;
	modifyDocument( true );
	GLGraphicComponent *graphic;
	int index = 0;
	GLfloat z = current_graphic -> getZ();
	GLfloat w = -9.0;
	for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		if ( ( graphic -> getZ() < z ) && ( graphic -> getZ() > w ) )
		w = graphic -> getZ();
	
	for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		{
		if ( graphic -> getZ() == z )
		graphic -> setZ( w );
		else
		if ( graphic -> getZ() == w )
			graphic -> setZ( z );
		if ( graphic == current_graphic )
		{
		index = graphic_list.at();
		current_graphic = graphic_list.take();
		}
		}
	current_graphic -> setZ( z - 0.0001 );
	graphic_list.insert( index - 1,  current_graphic );
	KTStatus -> currentKeyFrame() -> getDrawing() -> setGraphicComponents( graphic_list );
	}
	updateGL();
}

//Aligns the selection to the drawing area's right border
void DrawingArea::slotAlignRight()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			graphic -> calculateBottomRight();
			QPoint point = QPoint( graphic -> bottomRight() );
			int x = graphic -> getTranslate().x();
			graphic -> translateGraphic( -x, 0 );
			x = maxHorizontal();
			graphic -> translateGraphic( ( x - ( point.x() ) ), 0 );
		}
	}
}

//Centers the selection vertically into the drawing area
void DrawingArea::slotCenterVertically()
{
	if ( current_graphic_list.count() !=0 )
	{
	GLGraphicComponent *graphic;
	modifyDocument( true );
	for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
		graphic -> calculateTopLeft();
		graphic -> calculateBottomRight();
		QPoint point_tl = QPoint( graphic -> topLeft() );
		QPoint point_br = QPoint( graphic -> bottomRight() );
		int y = graphic -> getTranslate().y();
		graphic -> translateGraphic( 0, -y );
		y = maxVertical();
		graphic -> translateGraphic( 0, -( point_tl.y() ) );
		graphic -> translateGraphic( 0, ( int ) ( ( y - ( point_br.y() - point_tl.y() ) ) / 2 ) );
		}
	}
}

//Aligns the selection to the drawing area's left border
void DrawingArea::slotAlignLeft()
{
	if ( graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			graphic -> calculateTopLeft();
			QPoint point = QPoint( graphic -> topLeft() );
			int x = graphic -> getTranslate().x();
			graphic -> translateGraphic( -x, 0 );
			graphic -> translateGraphic( -( point.x() ), 0 );
		}
	}
}

//Aligns the selection to the drawing area's top border
void DrawingArea::slotAlignTop()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			graphic -> calculateTopLeft();
			QPoint point = QPoint( current_graphic -> topLeft() );
			int y = graphic -> getTranslate().y();
			graphic -> translateGraphic( 0, -y );
			graphic -> translateGraphic( 0, - ( point.y() ) );
		}
	}
}

//Centers the selection horizontally into the drawing area
void DrawingArea::slotCenterHorizontally()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			graphic -> calculateTopLeft();
			graphic -> calculateBottomRight();
			QPoint point_tl = QPoint( graphic -> topLeft() );
			QPoint point_br = QPoint( graphic -> bottomRight() );
			int x = graphic -> getTranslate().x();
			graphic -> translateGraphic( -x, 0 );
			x = maxHorizontal();
			graphic -> translateGraphic( -( point_tl.x() ), 0 );
			graphic -> translateGraphic( ( int ) ( ( x - ( point_br.x() - point_tl.x() ) ) / 2 ), 0 );
		}
	}
}

//Aligns the selection to the drawing area's bottom border
void DrawingArea::slotAlignBottom()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			graphic -> calculateBottomRight();
			QPoint point = QPoint( graphic -> bottomRight() );
			int y = graphic -> getTranslate().y();
			graphic -> translateGraphic( 0, -y );
			y = maxVertical();
			graphic -> translateGraphic( 0, ( y - ( point.y() ) ) );
		}
	}
}

//Reflects the selection in the drawing area - horizontal axis
void DrawingArea::slotFlipHorizontally()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> toggleHorizontalFlip();
	}
}

//Reflects the selection in the drawing area - vertical axis
void DrawingArea::slotFlipVertically()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> toggleVerticalFlip();
	}
}

//Rotates the selected elements 90º to the right
void DrawingArea::slotRotateCW90()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> setRotationAngle( 90.0 );
	}
}

//Rotates the selected elements 90º to the left
void DrawingArea::slotRotateCCW90()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> setRotationAngle( -90.0 );
	}
}

//Rotates the selected elements 180º
void DrawingArea::slotRotate180()
{
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> setRotationAngle( 180.0 );
	}
}

//Special Tool for transformations
void DrawingArea::slotPerspectiveSelection()
{

}

void DrawingArea::slotProperties()
{
	if ( current_graphic != NULL )
	{
		switch ( current_graphic -> kindGraphic() )
		{
			case GLGraphicComponent::GC_BRUSH:
			{
				BrushProperties *brush_properties = new BrushProperties( this );
				brush_properties -> loadFromGraphic( ( GLBrush * )current_graphic );
				brush_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_PENCIL:
			{
				PencilProperties *pencil_properties = new PencilProperties( this );
				pencil_properties -> loadFromGraphic( ( GLPencil * )current_graphic );
				pencil_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_PEN:
			{
				PenProperties *pen_properties = new PenProperties( this );
				pen_properties -> loadFromGraphic( ( GLPen * )current_graphic );
				pen_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_LINE:
			{
				LineProperties *line_properties = new LineProperties( this );
				line_properties -> loadFromGraphic( ( GLLine * )current_graphic );
				line_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_RECTANGLE:
			{
				RectangleProperties *rectangle_properties = new RectangleProperties( this );
				rectangle_properties -> loadFromGraphic( ( GLRectangle * )current_graphic );
				rectangle_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_ELLIPSE:
			{
				EllipseProperties *ellipse_properties = new EllipseProperties( this );
				ellipse_properties -> loadFromGraphic( ( GLEllipse * )current_graphic );
				ellipse_properties -> show();
				break;
			}
			case GLGraphicComponent::GC_IMAGE:
			{
				ImageProperties *image_properties = new ImageProperties( this );
				image_properties -> loadFromGraphic( ( GLImage * )current_graphic );
				image_properties -> show();
				break;
			}
			default: break;
		}
	}
}

void DrawingArea::slotAddToLibrary()
{
	bool ok;
	QString answer;
	answer = QInputDialog::getText( tr( "Add To Library" ), tr( "Symbol Name: " ), QLineEdit::Normal, QString::null, &ok, this );
	// FIXME: krawek
// 	if ( ok )
// 	k_toon -> library() -> addSymbol( current_graphic, answer );
}

bool DrawingArea::isModified()
{
	return modified;
}

GLuint DrawingArea::maxVertical() const
{
	return max_vertical;
}

GLuint DrawingArea::maxHorizontal() const
{
	return max_horizontal;
}

GLuint DrawingArea::numberLines() const
{
	return number_lines;
}

GLuint DrawingArea::verticalMargin() const
{
	return vertical_margin;
}

GLuint DrawingArea::horizontalMargin() const
{
	return horizontal_margin;
}

GLuint DrawingArea::height() const
{
	return h;
}

GLuint DrawingArea::width() const
{
	return w;
}

bool DrawingArea::zoomLines() const
{
	return zoom_lines;
}

bool DrawingArea::ntscZone() const
{
	return ntsc_zone;
}

bool DrawingArea::lightTable() const
{
	return light_table;
}

GLGraphicComponent *DrawingArea::currentGraphic() const
{
	return current_graphic;
}

QPtrList<GLGraphicComponent> DrawingArea::graphicList() const
{
	return graphic_list;
}

void DrawingArea::gridPosition( )
{
		// If the width of the window is greater than the grid, increase the margin in the diference
	
		if ( width() > ( 1.334 ) * height() )
		{
			setMaxVertical( height() - 40 );
			setMaxHorizontal( ( GLuint ) ( ( 1.334 ) * height() ) - 40 );
			setHorizontalMargin( ( ( width() - maxHorizontal() ) / 2 ) );
			setVerticalMargin( 20 );
		}
		// If width is smaller than height, decrease the margin of the grid in the diference
		if ( width() < height() )
		{
			setMaxHorizontal( width() );
			setMaxVertical( ( GLuint ) ( ( 0.75 ) * width() ) );
			setVerticalMargin( ( height() - maxVertical() ) / 2 );
			setHorizontalMargin( 20 );
		}
	
		if ( width() > maxHorizontal() )
		{
			setHorizontalMargin( ( ( width() - maxHorizontal() ) / 2 ) );
			if ( horizontalMargin() < 20 )
			setHorizontalMargin( 20 );
		}
		else  // 4/3 is the proportion of width and height
		{
			// define the size of the grid horizontally
			setMaxHorizontal( width() - 40 );
			setMaxVertical( ( GLuint ) ( ( 0.75 ) * width() ) - 40 );
			setVerticalMargin( ( height() - maxVertical() ) / 2 );
		}
	
		// If the height of the window is greater than the grid's, increase the margin in the difference
		if ( height() >= maxVertical() )
		{
			setVerticalMargin( ( ( height() - maxVertical() ) / 2 ) );
			if ( verticalMargin() < 20 )
			setVerticalMargin( 20 );
		}
		else
		{
		// define the size of the grid vertically
			setMaxVertical( height() );
			setMaxHorizontal( ( GLuint ) ( ( 1.334 ) * height() ) );
			setHorizontalMargin( ( ( maxVertical() - maxHorizontal() ) / 2 ) );
		}
}

void DrawingArea::slotChangeOutlineColor()
{
	current_outline_color = KTStatus -> currentOutlineColor();
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
			graphic -> setOutlineColor( *current_outline_color );
	}
	updateGL();
	emit updated();
}

void DrawingArea::slotChangeFillColor()
{
	current_fill_color = KTStatus -> currentFillColor();
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
	GLGraphicComponent *graphic;
	modifyDocument( true );
	for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
		switch( graphic -> kindGraphic() )
			{
			case GLGraphicComponent::GC_RECTANGLE:
			( ( GLRectangle * )( graphic ) ) -> setFillColor( *current_fill_color );
			break;
			case GLGraphicComponent::GC_ELLIPSE:
			( ( GLEllipse * )( graphic ) ) -> setFillColor( *current_fill_color );
			break;
			default:
			break;
			}
		}
	}
	updateGL();
	emit updated();
}

void DrawingArea::slotChangeMinThicknessBrush()
{
	current_brush = KTStatus -> currentBrush();
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
	GLGraphicComponent *graphic;
	modifyDocument( true );
	for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		graphic -> setWidthPoint( *current_brush );
	}
	updateGL();
	emit updated();
}

void DrawingArea::slotChangeMaxThicknessBrush()
{
	current_brush = KTStatus -> currentBrush();
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
	GLGraphicComponent *graphic;
	modifyDocument( true );
	for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			switch( graphic -> kindGraphic() )
				{
				case GLGraphicComponent::GC_BRUSH:
					graphic -> setWidthPoint( *current_brush );
					break;
				default:
					break;
				}
		}
	}
	updateGL();
	emit updated();
}

void DrawingArea::slotChangeSmoothnessBrush()
{
	current_brush = KTStatus -> currentBrush();
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
	GLGraphicComponent *graphic;
	modifyDocument( true );
	for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		{
			switch( current_graphic -> kindGraphic() )
			{
				case GLGraphicComponent::GC_BRUSH:
					( ( GLBrush * )( current_graphic ) ) -> smoothnessBrush( current_brush -> smoothnessBrush() );
					break;
				default:
					break;
			}
		}
	}
	updateGL();
	emit updated();
}

void DrawingArea::slotZoomIn()
{
	setScaleX( scaleX() + 0.1 );
	setScaleY( scaleY() + 0.1 );
	updateGL();
}

void DrawingArea::slotZoomOut()
{
	setScaleX( scaleX() - 0.1 );
	setScaleY( scaleY() - 0.1 );
	updateGL();
}

void DrawingArea::slotNoGrid()
{
	setNumberLines( 2 );
	updateGL();
}

void DrawingArea::slotSeeGrid12()
{
	setNumberLines( 24 );
	updateGL();
}

void DrawingArea::slotSeeGrid16()
{
	setNumberLines( 32 );
	updateGL();
}

void DrawingArea::slotSeeSubgrid()
{
	if( zoomLines() )
		setZoomLines( false );
	else
		setZoomLines( true );
	
	updateGL();
}

void DrawingArea::slotFrontBackGrid()
{
	if( front_grid )
		front_grid = false;
	else
		front_grid = true;
	updateGL();
}

void DrawingArea::slotSeeNTSC()
{
	if( ntscZone() )
		setNtscZone( false );
	else
		setNtscZone( true );
	updateGL();
}

void DrawingArea::slotLightTable()
{
	if( lightTable() )
		setLightTable( false );
	else
		setLightTable( true );
	updateGL();
}

void DrawingArea::slotRotateClockwise()
{
	setRotationZ( rotationZ() + 30 );
	updateGL();
}

void DrawingArea::slotRotateCounterClockwise()
{
	setRotationZ( rotationZ() - 30 );
	updateGL();
}

void DrawingArea::slotRestoreRotation()
{
	setRotationZ( 0 );
	updateGL();
}

void DrawingArea::slotFit()
{
	float x, y;
	setRotationZ( 0 );
	setScaleX( 1.0 );
	setScaleY( 1.0 );
	x = translationX();
	y = translationY();
	setTranslationX( -x );
	setTranslationY( -y );
	setHorizontalMargin( 20 );
	setVerticalMargin( 20 );
	setMaxHorizontal( width() - horizontalMargin() * 2 );
	setMaxVertical( height() - verticalMargin() * 2 );
	updateGL();
}

void DrawingArea::slotUndo()
{
	if ( graphic_list.count() > 0 )
	{
		modifyDocument( true );
		
		GLDrawing *main_drawing = KTStatus -> currentKeyFrame() -> getDrawing();
	
		current_graphic_list.clear();
		
		history_graphic_list.append(graphic_list.last());
	
		for ( int component = 0; component < graphic_list.count() - 1; component++ )
		{
		current_graphic_list.append(graphic_list.at(component));
		}
	
		main_drawing->setGraphicComponents(current_graphic_list);
		updateGL();
	}
}

void DrawingArea::slotRedo()
{
	if ( history_graphic_list.count() > 0)
	{
		modifyDocument( true );
		
		graphic_list.append(history_graphic_list.take(history_graphic_list.count()-1));
		
		current_graphic_list = graphic_list;
	
		KTStatus -> currentKeyFrame() -> getDrawing()->setGraphicComponents(current_graphic_list);
		updateGL();
	}
}

void DrawingArea::slotCut()
{
	modifyDocument( true );
	slotCopy();
	slotDelete();
}

void DrawingArea::slotCopy()
{
	clipboard.clear();
	if ( current_graphic_list.count() != 0 )
	{
		GLGraphicComponent *graphic;
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		clipboard.append( graphic -> clipboardGraphic() );
		//--
		//QApplication::clipboard()->setText(graphic -> clipboardGraphic());
	}
}

void DrawingArea::slotPaste()
{
	qDebug("Paste");
	std::cout << QApplication::clipboard()->text() << std::endl;
	for ( QStringList::Iterator it = clipboard.begin(); it != clipboard.end(); ++it )
	{
		QString text = *it;
		if ( !text.isEmpty() && KTStatus -> currentKeyFrame() != NULL/* && !( k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked() )*/ )
		{
			modifyDocument( true );
			QString number = text.left( text.find( ' ', 0 ) );
			text.remove( 0, number.length() + 1 );
			int i = number.toInt();
			if ( i > 0 )
			{
				switch( i )
				{
					case GLGraphicComponent::GC_BRUSH:
						pasteBrush( text );
						break;
					case GLGraphicComponent::GC_PENCIL:
						pastePencil( text );
						break;
					case GLGraphicComponent::GC_PEN:
						pastePen( text );
						break;
					case GLGraphicComponent::GC_LINE:
						pasteLine( text );
						break;
					case GLGraphicComponent::GC_RECTANGLE:
						pasteRectangle( text );
						break;
					case GLGraphicComponent::GC_ELLIPSE:
						pasteEllipse( text );
						break;
					default:
						break;
				}
				slotCenterHorizontally();
				slotCenterVertically();
			}
		}
	}
	updateGL();
}

void DrawingArea::slotPasteInPlace()
{
	for ( QStringList::Iterator it = clipboard.begin(); it != clipboard.end(); ++it )
	{
		QString text = *it;
		if ( !text.isEmpty() /*&& KTStatus -> currentKeyFrame() != NULL && !( k_toon -> exposureSheet() -> currentLayerObj() -> selectedFrame() -> isLocked() )*/ )
		{
			modifyDocument( true );
			QString number = text.left( text.find( ' ', 0 ) );
			text.remove( 0, number.length() + 1 );
			int i = number.toInt();
			switch( i )
			{
				case GLGraphicComponent::GC_BRUSH:
				pasteBrush( text );
				break;
				case GLGraphicComponent::GC_PENCIL:
				pastePencil( text );
				break;
				case GLGraphicComponent::GC_PEN:
				pastePen( text );
				break;
				case GLGraphicComponent::GC_LINE:
				pasteLine( text );
				break;
				case GLGraphicComponent::GC_RECTANGLE:
				pasteRectangle( text );
				break;
				case GLGraphicComponent::GC_ELLIPSE:
				pasteEllipse( text );
				break;
				default:
				break;
			}
			if ( current_graphic != NULL )
			{
				float z = -10.0;
				if ( graphic_list.count() != 0 )
				{
				GLGraphicComponent *graphic;
				for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
					if ( graphic -> getZ() > z )
					z = graphic -> getZ();
				}
				else
				z = 0.0;
				current_graphic ->setZ( z + 0.0001);
			}
		}
	}
	updateGL();
}

void DrawingArea::slotDelete()
{
	current_graphic_list.append( current_graphic );
	if ( current_graphic_list.count() !=0 )
	{
		GLGraphicComponent *graphic;
		modifyDocument( true );
		for ( graphic = current_graphic_list.first(); graphic; graphic = current_graphic_list.next() )
		graphic_list.remove( graphic );
	}
	GLDrawing *drawing = KTStatus -> currentKeyFrame() -> getDrawing();
	drawing -> setGraphicComponents( graphic_list );
	current_graphic = NULL;
	selected_graphic = false;
	bezier = false;

	updateGL();
}

void DrawingArea::slotSelectAll()
{
	selected_all = true;
	if ( current_graphic )
	{
		graphic_list.remove( current_graphic );
		addGraphicComponent( current_graphic, false );
		current_graphic = NULL;
		selected_graphic = false;
	}
	current_graphic_list.clear();
	GLGraphicComponent *graphic;
	for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
		current_graphic_list.append( graphic );
}

void DrawingArea::slotSelectFrame()
{
	qDebug("Select frame");
	if ( current_graphic && current_graphic -> kindGraphic() == GLGraphicComponent::GC_PEN && bezier )
	{
		bezier = false;
	}
	
	current_graphic = NULL;
	
	if ( KTStatus -> currentKeyFrame() != NULL )
	{
		light_table_list.clear();
		QPtrList<KeyFrame> keyframes_to_display;
		QPtrList<KeyFrame> keyframe_list = KTStatus -> currentLayer() -> keyFrames();
	
		// FIXME: krawek
// 		QPtrList<Layer> layer_list = k_toon -> exposureSheet() -> visibleLayers();
		Layer *layer_iterator;
		KeyFrame *ckf = KTStatus -> currentKeyFrame();
		Layer *cly = KTStatus -> currentLayer();
// 		for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
// 		{
// 		if ( layer_iterator != cly )
// 		{
// 			int ckf_offset = ckf -> offsetKeyFrame();
// 			int ckf_length = ckf -> lengthKeyFrame();
// 			int limit = ckf_offset + ckf_length - 1;
// 			KeyFrame *to_append = layer_iterator -> keyFrameAt( ckf_offset, limit );
// 			if ( to_append != NULL )
// 			keyframes_to_display.append( to_append );
// 		}
// 		}
	
		KeyFrame *k_it;
		for ( k_it = keyframes_to_display.first(); k_it; k_it = keyframes_to_display.next() )
		{
		QPtrList<GLGraphicComponent> gl = k_it -> getDrawing() -> graphicComponents();
		GLGraphicComponent *graphic;
		for ( graphic = gl.first(); graphic; graphic = gl.next() )
			light_table_list.append( graphic );
		}
	}
	else
	light_table_list.clear();
	
	updateGL();
}

//Disable previous Onion Skins
void DrawingArea::slotNoPreviousOnionSkin()
{
	previous_onion_skin_1.clear();
	previous_onion_skin_2.clear();
	previous_onion_skin_3.clear();
}

//See only the previous Onion Skin
void DrawingArea::slotPreviousOnionSkin()
{
	int i = 0;
	if ( KTStatus -> currentKeyFrame() != NULL )
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
	if ( i > 0 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		previous_onion_skin_1 = ( onion_skin.at( i - 1 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		previous_onion_skin_1.clear();
	
	previous_onion_skin_2.clear();
	previous_onion_skin_3.clear();
}

//See the previous two Onion Skins
void DrawingArea::slotPrevious2OnionSkin()
{
	int i = 0;
	if ( KTStatus -> currentKeyFrame() != NULL )
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
	if ( i > 1 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		previous_onion_skin_2 = ( onion_skin.at( i - 2 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		previous_onion_skin_2.clear();
	previous_onion_skin_3.clear();
}

//See the previous three Onion Skins
void DrawingArea::slotPrevious3OnionSkin()
{
	int i = 0;
	if ( KTStatus -> currentKeyFrame() != NULL )
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
	if ( i > 2 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		previous_onion_skin_3 = ( onion_skin.at( i - 3 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		previous_onion_skin_3.clear();
}

//Disable next Onion Skins
void DrawingArea::slotNoNextOnionSkin()
{
	next_onion_skin_1.clear();
	next_onion_skin_2.clear();
	next_onion_skin_3.clear();
}

//See only the next Onion Skin
void DrawingArea::slotNextOnionSkin()
{
	int i = 0, j = 0;
	
	if ( KTStatus -> currentKeyFrame() != NULL )
	{
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
		j = KTStatus -> currentLayer() -> keyFrames().count();
	}
	if ( i < j - 1 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		next_onion_skin_1 = ( onion_skin.at( i + 1 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		next_onion_skin_1.clear();
	
	next_onion_skin_2.clear();
	next_onion_skin_3.clear();
}

//See the next two Onion Skins
void DrawingArea::slotNext2OnionSkin()
{
	int i = 0, j = 0;
	if ( KTStatus -> currentKeyFrame() != NULL )
	{
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
		j = KTStatus -> currentLayer() -> keyFrames().count();
	}
	if ( i < j - 2 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		next_onion_skin_2 = ( onion_skin.at( i + 2 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		next_onion_skin_2.clear();
	next_onion_skin_3.clear();
}

//See the next three Onion Skins
void DrawingArea::slotNext3OnionSkin()
{
	int i = 0, j = 0;
	if ( KTStatus -> currentKeyFrame() != NULL )
	{
		i = KTStatus -> currentLayer() -> keyFrames().find( KTStatus -> currentKeyFrame() );
		j = KTStatus -> currentLayer() -> keyFrames().count();
	}
	if ( i < j - 3 )
	{
		QPtrList<KeyFrame> onion_skin = KTStatus -> currentLayer() -> keyFrames() ;
		next_onion_skin_3 = ( onion_skin.at( i + 3 ) ) -> getDrawing() -> graphicComponents();
	}
	else
		next_onion_skin_3.clear();
}

KTColor *DrawingArea::grabColor( const QPoint &p )
{
	KTColor* color = 0;
	QPoint point = p;
	point.setY( height() - point.y() );
	if ( current_graphic )
	{
		switch( current_graphic -> kindGraphic() )
		{
			case GLGraphicComponent::GC_BRUSH:
			case GLGraphicComponent::GC_PENCIL:
			case GLGraphicComponent::GC_PEN:
			case GLGraphicComponent::GC_LINE:
				color = new KTColor( current_graphic -> outlineColor().colorRed(),
						current_graphic -> outlineColor().colorGreen(),
						current_graphic -> outlineColor().colorBlue(),
						current_graphic -> outlineColor().colorAlpha() );
				break;
			case GLGraphicComponent::GC_RECTANGLE:
				color = new KTColor( ( ( GLRectangle * )( current_graphic ) ) -> fillColor().colorRed(),
						( ( GLRectangle * )( current_graphic ) ) -> fillColor().colorGreen(),
						( ( GLRectangle * )( current_graphic ) ) -> fillColor().colorBlue(),
						( ( GLRectangle * )( current_graphic ) ) -> fillColor().colorAlpha() );
				break;
			case GLGraphicComponent::GC_ELLIPSE:
				color = new KTColor( ( ( GLEllipse * )( current_graphic ) ) -> fillColor().colorRed(),
						( ( GLEllipse * )( current_graphic ) ) -> fillColor().colorGreen(),
						( ( GLEllipse * )( current_graphic ) ) -> fillColor().colorBlue(),
						( ( GLEllipse * )( current_graphic ) ) -> fillColor().colorAlpha() );
				break;
			default:
				color = new KTColor;
					break;
		}
	}
	else
	{
		GLfloat glcolor[4]; 
		glReadPixels( point.x(), point.y(), 1, 1 , GL_RGBA, GL_FLOAT, &glcolor );
		color = new KTColor( glcolor[0], glcolor[1], glcolor[2], glcolor[3] );
	}
	return color;
}

void DrawingArea::wheelEvent( QWheelEvent *wheel )
 {
	wheel -> accept();
	if ( wheel -> delta() > 0 && scaleX() < 4.0 && scaleY() < 4.0 )
	{
		setScaleX( scaleX() + 0.1 );
		setScaleY( scaleY() + 0.1 );
	
	}
	else if ( scaleX() > 0.2 && scaleY() > 0.2 )
	{
		setScaleX( scaleX() - 0.1 );
		setScaleY( scaleY() - 0.1 );
	}
	updateGL();
}

void DrawingArea::setRotationX( const GLfloat & x )
{
	rotation_x = x;
}

void DrawingArea::setRotationY( const GLfloat & y )
{
	rotation_y = y;
}

void DrawingArea::setRotationZ( const GLfloat & z)
{
	rotation_z = z;
}

void DrawingArea::setTranslationX( const GLfloat & x )
{
 translation_x = x;
}

void DrawingArea::setTranslationY( const GLfloat & y )
{
 translation_y = y;
}

void DrawingArea::setTranslationZ( const GLfloat & z )
{
 translation_z = z;
}

void DrawingArea::setScaleX( const GLfloat & x )
{
 scale_x = x;
}

void DrawingArea::setScaleY( const GLfloat & y )
{
 scale_y = y;
}

void DrawingArea::setScaleZ( const GLfloat & z )
{
 scale_z = z;
}

GLfloat DrawingArea::rotationX() const
{
 return rotation_x;
}

GLfloat DrawingArea::rotationY() const
{
 return rotation_y;
}

GLfloat DrawingArea::rotationZ() const
{
 return rotation_z;
}

GLfloat DrawingArea::translationX() const
{
 return translation_x;
}

GLfloat DrawingArea::translationY() const
{
 return translation_y;
}

GLfloat DrawingArea::translationZ() const
{
 return translation_z;
}

GLfloat DrawingArea::scaleX() const
{
 return scale_x;
}

GLfloat DrawingArea::scaleY() const
{
 return scale_y;
}

GLfloat DrawingArea::scaleZ() const
{
 return scale_z;
}

GLuint DrawingArea::loadTexture( const char *filename )
{
	GLuint id_texture;

	QImage image, image2;
	if ( image.load( filename ) == false )
	     qDebug( "Can't load the image" + QString( filename ) );

	image2 = QGLWidget::convertToGLFormat( image );
	texture = ( QRgb * )( image2.bits() );

	glGenTextures( 1, &id_texture );
	glBindTexture( GL_TEXTURE_2D, id_texture );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image2.width(), image2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture );

        return id_texture;
}

GLuint DrawingArea::createSelectionArrows( GLuint id_texture )
{
     	GLuint id_list = glGenLists( 1 );
        if( id_list <= 0 )
	{
		qDebug( tr("%1:%2 I can't create glList %3").arg(__FILE__).arg(__LINE__).arg(id_list) );

		QApplication::exit(1);
                exit( 1 ); // TODO: salir de una forma menos machetera!
        }
	glNewList( id_list, GL_COMPILE );
		glPushName( GLGraphicComponent::selection_name++ );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, id_texture );
		glColor4f( 0.0, 0.0, 0.0, 0.0 );
		glBegin( GL_QUADS );
			glTexCoord2f( 0, 0 );
			glVertex3f( 0, 0, 8 );
			glTexCoord2f( 1, 0 );
			glVertex3f( 16, 0, 8 );
			glTexCoord2f( 1, 1 );
			glVertex3f( 16, 16, 8 );
			glTexCoord2f( 0, 1 );
			glVertex3f( 0, 16, 8 );
		glEnd();
		glDisable( GL_TEXTURE_2D );
		glPopName();
	glEndList();
        return id_list;
}

void DrawingArea::invertMatrix()
{
	int i,j;
	convertToMatrix();
	GLdouble matrix[4][8];
	
	for ( i = 0; i < 4; i++ )
	for ( j = 0; j < 4; j++ )
		matrix[i][j] = mmatrix[i][j];
	
	for ( i = 0; i < 4; i++ )
	for ( j = 4; j < 8; j++ )
	{
		if ( i == ( j - 4) )
		matrix[i][j] = 1.0;
		else
		matrix[i][j] = 0.0;
	}
	
	for ( i = 0; i < 4; i++ )
	{
	GLdouble pivote = matrix[i][i];
	for ( int k = 0; k < 8; k++ )
	if ( pivote != 1.0 )
		matrix[i][k] = matrix[i][k]/pivote;
	
	for ( j = 0; j < 4; j++ )
	{
		if ( i != j )
		{
			GLdouble pivote = matrix[j][i];
			for ( int k = 0; k < 8; k++ )
			matrix[j][k] = matrix[j][k] - matrix[i][k]*pivote;
		}
	}
	}
	
	for ( i = 0; i < 4; i++ )
	for ( j = 4; j < 8; j++ )
		mmatrix[i][j-4] = matrix[i][j];

}

void DrawingArea::convertToMatrix()
{
	int k = 0;
	for ( int i = 0; i < 4; i ++ )
		for ( int j = 0; j < 4; j++ )
		{
			mmatrix[j][i] = matrix[k];
			k++;
		}
}

QPoint DrawingArea::mapPointToMatrix( const QPoint & point )
{
	QPoint p;
	GLdouble w = mmatrix[3][0] * point.x() + mmatrix[3][1] * point.y() + mmatrix[3][3];
	GLdouble x = ( mmatrix[0][0] * point.x() + mmatrix[0][1] * point.y() + mmatrix[0][3] ) / w;
	GLdouble y = ( mmatrix[1][0] * point.x() + mmatrix[1][1] * point.y() + mmatrix[1][3] ) / w;
	p.setX( ( int ) x );
	p.setY( ( int ) y );
	return p;
}

void DrawingArea::slotSetGridColor()
{
	QColor g_color = KTStatus -> currentGridColor();
	grid -> setGridColor( g_color );
	updateGL();
}

void DrawingArea::slotSetNTSCColor()
{
	QColor n_color = KTStatus -> currentNTSCColor();
	grid -> setNTSCColor( n_color );
	updateGL();
}
