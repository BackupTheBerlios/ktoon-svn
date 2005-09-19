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

#include "glrendercamerapreview.h"
#include "status.h"
#include <qimage.h>

//-------------- CONSTRUCTOR -----------------

GLRenderCameraPreview::GLRenderCameraPreview( QWidget* parent, /*QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button, */QGLWidget *share, WFlags f )
    : QGLWidget( parent, "", share, f )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( share );

    //Initializations
    max_horizontal = 360;
    max_vertical = 280;

    resize( max_horizontal, max_vertical );

    setMinimumSize( max_horizontal, max_vertical );
//     setMaximumSize( max_horizontal, max_vertical );
    

    setCaption( tr( "Render Camera Preview" ) );

    current_graphic = 0;
    dragging = false;
}

//-------------- DESTRUCTOR -----------------

GLRenderCameraPreview::~GLRenderCameraPreview()
{
}

//--------------------- PROTECTED MEMBERS -------------------------

void GLRenderCameraPreview::initializeGL()
{
    glClearColor( 1.0, 1.0, 1.0, 0.0 );
    glShadeModel( GL_SMOOTH );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_LINE_SMOOTH );
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
}

void GLRenderCameraPreview::resizeGL( int w, int h )
{
    glViewport( 0, 0, ( GLint ) w, ( GLint ) h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, w, h, 0.0, -10.0, 10.0 );
    glMatrixMode( GL_MODELVIEW );
}

void GLRenderCameraPreview::paintGL()
{
	qDebug("GLRenderCameraPreview: paintGL");
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();

    drawFrame();

    //Calculate it later. It depends on the drawing area's size respect to this widget's size.
    float scale_factor = 0.532;
    float translate_factor = 20.0;
    glTranslatef( translate_factor, translate_factor, 0.0 );
    glScalef( scale_factor, scale_factor, 1.0 );

    QPtrList<KeyFrame> keyframes_to_display = KTStatus -> renderKeyframes();
    int n_keyframes = keyframes_to_display.count();
    float n_translate =  10.0 / ( float )( n_keyframes );
    float position = 9.0;
    KeyFrame *k_it;
    for ( k_it = keyframes_to_display.first(); k_it; k_it = keyframes_to_display.next() )
    {
        position -= n_translate;
 	glTranslatef( 0.0, 0.0, position );
        QPtrList<GLGraphicComponent> gl = k_it -> getDrawing() -> graphicComponents();
	GLGraphicComponent *graphic;
        for ( graphic = gl.first(); graphic; graphic = gl.next() )
	{
            graphic -> draw();
	}
	glTranslatef( 0.0, 0.0, -position );
    }

    glPopMatrix();
}

void GLRenderCameraPreview::drawFrame()
{
    glLineWidth( 2.0 );
    glColor3f( 0.0, 0.0, 0.0 );
    int w = KTStatus -> currentCameraWidth();
    int h = KTStatus -> currentCameraHeight();

    glBegin( GL_LINE_LOOP );
       glVertex2i( 20, 20 );
       glVertex2i( w + 20, 20 );
       glVertex2i( w + 20, h + 20 );
       glVertex2i( 20, h + 20 );
    glEnd();
}

void GLRenderCameraPreview::mousePressEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    drawSelected( mouse_event );
    dragging = true;
    updateGL();
}

void GLRenderCameraPreview::mouseMoveEvent( QMouseEvent *mouse_event )
{
    if ( dragging )
    {
        mouse_event -> accept();
	if ( current_graphic != NULL )
	{
	    //Do something with the mouse event and translate this graphic
	    updateGL();
	}
    }
    else
        mouse_event -> ignore();
}

void GLRenderCameraPreview::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    dragging = false;
}

// void GLRenderCameraPreview::closeEvent( QCloseEvent *close_event )
// {
//     assigned_menu -> setItemChecked( assigned_item, false );
//     assigned_tb_button -> setDown( false );
//     close_event -> accept();
// }

void GLRenderCameraPreview::processHits( GLint hits, GLuint buffer[] )
{
	qDebug("processHits");
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

    current_graphic = NULL;

    QPtrList<KeyFrame> displayed_keyframes = KTStatus -> renderKeyframes();
    QPtrList<GLGraphicComponent> graphic_list;
    KeyFrame *kf_it;
    for ( kf_it = displayed_keyframes.first(); kf_it; kf_it = displayed_keyframes.next() )
    {
        graphic_list = kf_it -> getDrawing() -> graphicComponents();
        for ( graphic = graphic_list.first(); graphic; graphic = graphic_list.next() )
        {
	    if ( graphic -> selectionName() == aux )
	    {
               current_graphic = graphic_list.at( graphic_list.at() );
		//This a test, delete it later
	        //current_graphic -> setOutlineColor( Color( 1.0, 0.0, 0.0, 1.0 ) );
	        return;
	    }
        }
    }
}

void GLRenderCameraPreview::drawSelected( QMouseEvent *mouse_event )
{
	GLint viewport[4];
	GLdouble delta = 10.0;

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

QValueList<QImage> GLRenderCameraPreview::imageList()
{
	QValueList<QImage> images;

	
	return images;
}

void GLRenderCameraPreview::exportTo(int)
{
	QValueList<QImage> imgs = imageList();
	
	std::cout << "Images: " << imgs.count() << std::endl;
	for(uint i = 0; i < imgs.count(); i++)
	{
		imgs[i].save(QString("file%1.png").arg(i), "PNG");
	}
}

void GLRenderCameraPreview::saveImage(const QString &filename/*, KTExporter::Format f*/)
{
	std::cout << "Saving image: " << filename << std::endl;
	QPixmap exp;

	exp.convertFromImage( grabFrameBuffer() );
	
	if ( !exp.save( filename, "PNG" ) ) // TODO: remover las imagenes
	{
		qDebug( tr( "Could not save the file: %1" ).arg( filename ) );
	}
}
		

void GLRenderCameraPreview::resizeEvent(QResizeEvent *e)
{
// 	KTStatus->setCurrentCameraWidth(e->size().width());
// 	KTStatus->setCurrentCameraHeight(e->size().height());
	QGLWidget::resizeEvent(e);
}

