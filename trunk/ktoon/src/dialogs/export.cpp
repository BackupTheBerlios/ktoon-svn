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

#include <qfiledialog.h>
#include <qimage.h>
#include <qstringlist.h>
#include <qpicture.h>

#include "export.h"
#include "status.h"
#include "ktapplication.h"
#include "ktexporter.h"
// #include "ktoon.h"
//#include "mingpp.h"

#include <new>

//--------------- CONSTRUCTOR --------------------

Export::Export( QWidget *parent ) : QDialog( parent, "Export", true )
{
    //Initializations
    setCaption( tr( "Export" ) );
    
//     parent_widget = parent;
//     k_toon = ( KToon * )parent_widget;
    setMinimumSize( 210, 220 );
    setMaximumSize( 210, 220 );

    //------------- Operations on the static texts ----------

    main_text = new QLabel( tr( "Select the option to export:" ), this );
    main_text -> resize( 200, 20 );
    main_text -> move( 10, 10 );

    //------------- Operations on the radio buttons -------------

    group = new QButtonGroup( this );
    group -> move( main_text -> x(), main_text -> y() + main_text -> height() + 10 );
    group -> resize( 190, 130 );

    rb_animation = new QRadioButton( tr( "Animation" ), group );
    rb_animation -> move( 5, 5 );
    rb_animation -> setChecked( true );

    rb_single_frame = new QRadioButton( tr( "Single Frame" ), group );
    rb_single_frame -> move( rb_animation -> x(), rb_animation -> y() + rb_animation -> height() );
    rb_single_frame -> resize( 180, rb_single_frame -> height() );

    rb_swf = new QRadioButton( tr( "SWF (Macromedia Flash)" ), group );
    rb_swf -> move( rb_single_frame -> x(), rb_single_frame -> y() + rb_single_frame -> height() );
    rb_swf -> resize( 180, rb_swf -> height() );

    rb_svg = new QRadioButton( tr( "SVG" ), group );
    rb_svg -> move( rb_swf -> x(), rb_swf -> y() + rb_swf -> height() );

    //------------- Operations on the buttons -----------------

    accept = new QPushButton( tr( "Accept" ), this );
    accept -> resize( 60, 30 );
    accept -> move( group -> x() + 50, group -> y() + group -> height() + 10 );
    connect( accept, SIGNAL( clicked() ), SLOT( accept() ) );

    cancel = new QPushButton( tr( "Cancel" ), this );
    cancel -> resize( 60, 30 );
    cancel -> move( accept -> x() + accept -> width() + 10, accept -> y() );
    connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
}

//-------------- DESTRUCTOR -----------------

Export::~Export()
{
    delete accept;
    delete cancel;
    delete main_text;
    delete rb_single_frame;
    delete rb_swf;
    delete rb_animation;
    delete rb_svg;
    delete group;
}

//--------------- PUBLIC MEMBERS --------------------

// SWFDisplayItem *Export::doAnimation( SWFMovie *movie, const QStringList &images )
// {
//     Q_ASSERT( movie );
//     
//     int i, image_count;
//     QStringList copy = QStringList( images );
//     QStringList::Iterator it;
//     SWFDisplayItem *frame = NULL;
//     image_count = ( int )copy.count();
//     SWFShape *shape[image_count];
// 
//     for( it = copy.begin(), i = 0; i < image_count; ++i, ++it )
//     {
//         SWFBitmap *bitmap = new SWFBitmap( ( char * )( *it ).latin1() );
//         float width  = bitmap -> getWidth();
//         float height = bitmap -> getHeight();
// 
// 	  SWFShape* p_shape = new(std::nothrow) SWFShape();
// 	  if(!p_shape)
// 	      {
// 		delete bitmap;
// 		throw std::bad_alloc();
// 		}
// 	  shape[i] = p_shape;
// 	  
//         SWFFill *fill = shape[i] -> addBitmapFill( bitmap );
//         shape[i] -> setRightFill( fill );
//         shape[i] -> drawLine( width, 0 );
//         shape[i] -> drawLine( 0, height );
//         shape[i] -> drawLine( -1 * width, 0 );
//         shape[i] -> drawLine( 0, -1 * height );
// 
// 	frame = movie -> add( shape[i] );
//         movie -> nextFrame();
//     }
// 
//     return frame;
// }

// SWFMovie *Export::doMovie( int width, int height )
// {
//     SWFMovie *movie = new SWFMovie();
//     movie -> setDimension( width, height );
//     movie -> setBackground( 0xff, 0xff, 0xff );
// 
//     return movie;
// }

//------------------- SLOTS -------------------------

void Export::slotAccept()
{
#ifdef USE_MING // FIXME: 
    switch ( group -> selectedId() )
    {
        case 0:
	{
            QString fn = QFileDialog::getSaveFileName( KTOON_REPOSITORY + "/output/", "PNG sequence ( *.png )", this );
            if ( !fn.isEmpty() )
	    {
		    QString message = "";
// 		    if ( k_toon -> scenes() -> exportAnimation( fn, "PNG" ) > 0 ) // FIXME
//         	    message = tr( "Animation Exported Successfully" );
//         	else
//         	    message = tr( "Could not Export Animation" );
		
		emit sendMessage (message);
	    }
	    break;
	}
        case 1:
	{
            QString fn = QFileDialog::getSaveFileName( KTOON_REPOSITORY + "/output/", "Single PNG ( *.png )", this );
            if ( !fn.isEmpty() )
	    {
		QPixmap exp;
		fn = fn + ".png";
		//FIXME:kuadrosx
// 		exp.convertFromImage( k_toon -> renderCameraPreview() -> grabFrameBuffer() );
		if ( !exp.save( fn, "PNG" ) )
			emit sendMessage( tr( "Could not save the file: %1" ).arg( fn ) );
	    }
	    break;
	}
        case 2:
	{
            QString fn = QFileDialog::getSaveFileName( KTOON_REPOSITORY + "/output/", "Macromedia Flash ( *.swf )", this );
            if ( !fn.isEmpty() )
	    {
	        int number_of_images = k_toon -> scenes() -> exportAnimation( fn, "JPEG" );
        	if ( number_of_images > 0 )
		{
		    int frame_rate = KTStatus -> currentFrameRate();

		    SWFMovie *movie = doMovie( 360, 280 );
  		    movie -> setRate( ( float )frame_rate );
  		    QStringList images;

		    for ( int i = 1; i < number_of_images; i++ )
  		    {
     			QString iterator = QString::number( i );
     			iterator = iterator.rightJustify( 3, '0' );
     			images << QString( fn + iterator + ".jpg" );
  		    }

  		    SWFDisplayItem *frame = doAnimation( movie, images );
		    frame -> addColor( 255, 0, 0 );

  		    if ( movie -> save( ( char * )( fn + ".swf" ).latin1() ) < 0 )
			    emit sendMessage( tr( "Could not Export Animation" ) );
		    else
			    emit sendMessage( tr( "Animation Exported Successfully" ) );

		    for ( int i = 1; i < number_of_images; i++ )
  		    {
     			QString iterator = QString::number( i );
     			iterator = iterator.rightJustify( 3, '0' );
     			QFile::remove( fn + iterator + ".jpg" );
  		    }
		}
        	else
        	    emit sendMessage( tr( "Could not Export Animation" ) );
	    }
	    break;
	}
	case 3:
	{
            QString fn = QFileDialog::getSaveFileName( KTOON_REPOSITORY + "/output/", "Scalable Vector Graphics ( *.svg )", this );
            if ( !fn.isEmpty() )
	    {
		QPixmap exp;
		fn = fn + ".svg";
		//FIXME:kuadrosx
		exp.convertFromImage( k_toon -> renderCameraPreview() -> grabFrameBuffer() );

		QPainter painter;
		QPicture picture;

		painter.begin( &picture );
		painter.drawPixmap( 0, 0, exp );
		painter.end();

		if ( picture.save( fn, "svg" ) )
		    emit sendMessage( tr( "SVG Exported Successfully" ));
		else
			emit sendMessage( tr( "Could not Export SVG" ));
	    }
	    break;
	}
	default: break;
    }
#endif
	
	QDialog::accept ();
}

void Export::slotCancel()
{
    close( true );
}
