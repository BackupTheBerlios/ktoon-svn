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

 #include "gltopcameraview.h"

//-------------- CONSTRUCTOR ---------------

GLTopCameraView::GLTopCameraView( QWidget* parent, QWidget *grandparent, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button, QGLWidget *share, WFlags f )
    : QGLWidget( parent, "", share, f )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );
    Q_CHECK_PTR( in_assigned_menu );
    Q_CHECK_PTR( assig_tb_button );
    Q_CHECK_PTR( share );

    //Initializations
    max_horizontal = 200;
    max_vertical = 350;

    resize( max_horizontal, max_vertical );
    move( 400, 220 );
    setMinimumSize( max_horizontal, max_vertical );
    setMaximumSize( max_horizontal, max_vertical );
    setCaption( tr( "Top Camera View" ) );

    parent_widget = parent;
    grandparent_widget = grandparent;
    share_widget = share;
    assigned_menu = in_assigned_menu;
    assigned_item = id_assigned_item;
    assigned_tb_button = assig_tb_button;
}

//-------------- DESTRUCTOR -----------------

GLTopCameraView::~GLTopCameraView()
{

}

//--------------------- PROTECTED MEMBERS -------------------------

void GLTopCameraView::initializeGL()
{
    glClearColor( 1.0, 0.9, 1.0, 0.0 );
    glShadeModel( GL_SMOOTH );
}

void GLTopCameraView::resizeGL( int w, int h )
{
    glViewport( 0, 0, ( GLint )w, ( GLint )h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, w, h, 0.0, 0.0, 1.0 );
}

void GLTopCameraView::paintGL()
{
    // Clean the color of the screen.
    glClear( GL_COLOR_BUFFER_BIT );
    glPushMatrix();
    drawFrame();
    glScalef( 0.5, 0.5, 0.0 );
    glPopMatrix();
 }

 void GLTopCameraView::drawFrame( )
{
     glLineWidth( 2.0 );
     glColor3f( 1.0, 0.0, 1.0 );

     glBegin( GL_LINE_LOOP );
      glVertex2i( 20, 20 );
      glVertex2i( max_horizontal - 20, 20 );
      glVertex2i( max_horizontal - 20, max_vertical - 20 );
      glVertex2i( 20, max_vertical - 20 );
     glEnd();

     glLineWidth( 1.0 );
     glColor3f( 0.5, 0.5, 0.5 );
     glBegin( GL_LINE_STRIP );
      glVertex2i( 20, 20 );
      glVertex2i( max_horizontal / 2, max_vertical - 20 );
      glVertex2i( max_horizontal - 20, 20 );
     glEnd();
}

void GLTopCameraView::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    mouse_event -> accept();
}

void GLTopCameraView::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    mouse_event -> accept();
}

void GLTopCameraView::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    mouse_event -> accept();
}

void GLTopCameraView::closeEvent( QCloseEvent *close_event )
{
    Q_CHECK_PTR( close_event );
    assigned_menu -> setItemChecked( assigned_item, false );
    assigned_tb_button -> setDown( false );
    close_event -> accept();
}
