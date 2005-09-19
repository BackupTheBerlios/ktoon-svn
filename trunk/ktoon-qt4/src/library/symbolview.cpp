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

#include "symbolview.h"
#include "drawingarea.h"
#include "ktoon.h"

#include <qcursor.h>
//Added by qt3to4:
#include <QMouseEvent>

//------------- CONSTRUCTOR ----------------

SymbolView::SymbolView( QWidget *parent, QGLWidget *share ) : QGLWidget( parent, "", share )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( share );
    parent_widget = parent;
    share_widget = share;
    displayed_graphic = NULL;
    dragging = false;
    valid_drag = false;
    k_toon = ( KToon * )( parent_widget -> parentWidget() );
}

//------------- DESTRUCTOR ----------------

SymbolView::~SymbolView()
{

}

//------------- PUBLIC MEMBERS --------------

QGLWidget *SymbolView::shareWidget() const
{
    Q_CHECK_PTR( share_widget );
    return share_widget;
}

void SymbolView::display( GLGraphicComponent *graphic )
{
    displayed_graphic = graphic;
    updateGL();
}

//-------------- SLOTS ---------------

//----------- EVENTS AND PROTECTED MEMBERS ---------------

void SymbolView::initializeGL()
{
    glClearColor( 1.0, 1.0, 1.0, 0.0 );
    glShadeModel( GL_SMOOTH );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
}

void SymbolView::resizeGL( int w, int h )
{
    glViewport( 0, 0, ( GLint ) w, ( GLint ) h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, w, h, 0.0, -10.0, 10.0 );
    glMatrixMode( GL_MODELVIEW );
}

void SymbolView::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glPushMatrix();

    drawFrame();
    if ( displayed_graphic != NULL )
    {
	glPushMatrix();
	displayed_graphic -> calculateTopLeft();
	displayed_graphic -> calculateBottomRight();
	QPoint ptl = displayed_graphic -> topLeft();
	QPoint pbr = displayed_graphic -> bottomRight();
	//140x100
	int w = pbr.x() - ptl.x();
	int h = pbr.y() - ptl.y();
	float scale_factor;
	int y = 5, x = 5;

	if ( w >= h )
	{
	    scale_factor = 130.0 / ( float )w;
	    y = ( int )( 100 - h * ( int )( scale_factor ) ) / 2;
	}
	else
	{
	    scale_factor = 90.0 / ( float )h;
	    x = ( int )( 140 - w * ( int )( scale_factor ) ) / 2;
	}
	glScalef( scale_factor, scale_factor, 1.0 );
	glTranslatef( ( float )ptl.x() * -1.0 + x, ( float )ptl.y() * -1.0 + y, 0.0 );
	displayed_graphic -> draw();
	glPopMatrix();
    }
    else
        drawDisabled();

    glPopMatrix();
}

void SymbolView::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    mouse_event -> accept();
    dragging = true;
}

void SymbolView::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( dragging && displayed_graphic != NULL )
    {
        mouse_event -> accept();
	if ( parent_widget -> childAt( mapToParent( mouse_event -> pos() ) ) == this ||
	     k_toon -> childAt( mouse_event -> globalPos() ) == KTStatus->currentDrawingArea() )
	    setCursor( QCursor( Qt::PointingHandCursor ) );
	else
	    setCursor( QCursor( Qt::ForbiddenCursor ) );

	if ( k_toon -> childAt( mouse_event -> globalPos() ) == KTStatus->currentDrawingArea() )
	    valid_drag = true;
	else
	    valid_drag = false;
    }
}

void SymbolView::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( valid_drag )
    {
        mouse_event -> accept();
	switch ( displayed_graphic -> kindGraphic() )
        {
            case GLGraphicComponent::GC_BRUSH:
	    {
	    	    GLBrush *b = new GLBrush( *( ( GLBrush * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    b -> translateGraphic( ( b -> originPoint().x() ) * -1, ( b -> originPoint().y() ) * -1 );
		    b -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( b );
	    }
		    break;
	    case GLGraphicComponent::GC_PENCIL:
	    {
	    	    GLPencil *pc = new GLPencil( *( ( GLPencil * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    pc -> translateGraphic( ( pc -> originPoint().x() ) * -1, ( pc -> originPoint().y() ) * -1 );
		    pc -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( pc );
	    }
		    break;
	    case GLGraphicComponent::GC_LINE:
	    {
	    	    GLLine *l = new GLLine( *( ( GLLine * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    l -> translateGraphic( ( l -> originPoint().x() ) * -1, ( l -> originPoint().y() ) * -1 );
		    l -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( l );
	    }
		    break;
	    case GLGraphicComponent::GC_PEN:
	    {
	    	    GLPen *p = new GLPen( *( ( GLPen * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    p -> translateGraphic( ( p -> originPoint().x() ) * -1, ( p -> originPoint().y() ) * -1 );
		    p -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( p );
	    }
		    break;
	    case GLGraphicComponent::GC_RECTANGLE:
	    {
	    	    GLRectangle *r = new GLRectangle( *( ( GLRectangle * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    r -> translateGraphic( ( r -> originPoint().x() ) * -1, ( r -> originPoint().y() ) * -1 );
		    r -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( r );
	    }
		    break;
	    case GLGraphicComponent::GC_ELLIPSE:
	    {
		    GLEllipse *e = new GLEllipse( *( ( GLEllipse * )displayed_graphic ) );
		    QPoint global( mouse_event -> globalX(), mouse_event -> globalY() );
		    QPoint da_local( KTStatus->currentDrawingArea() -> mapFromGlobal( global ) );
		    e -> translateGraphic( ( e -> originPoint().x() ) * -1, ( e -> originPoint().y() ) * -1 );
		    e -> translateGraphic( da_local.x(), da_local.y() );
	            KTStatus->currentDrawingArea() -> addGraphicComponent( e );
	    }
		    break;
	    default: break;
        }
    }
    dragging = false;
    valid_drag = false;
    setCursor( QCursor( Qt::ArrowCursor ) );
}

void SymbolView::drawFrame()
{
    glLineWidth( 1.5 );
    glColor3ub( 0, 0, 0 );
    glBegin( GL_LINE_LOOP );
	glVertex2i( 0, 0 );
	glVertex2i( width() - 1, 0 );
	glVertex2i( width() - 1, height() - 1 );
	glVertex2i( 0, height() - 1 );
    glEnd();
}

void SymbolView::drawDisabled()
{
    glLineWidth( 1.0 );
    glColor3ub( 0, 0, 0 );
    glBegin( GL_LINES );
        glVertex2i( 0, 0 );
	glVertex2i( width() - 1, height() - 1 );
	glVertex2i( width() - 1, 0 );
	glVertex2i( 0, height() - 1 );
    glEnd();
}
