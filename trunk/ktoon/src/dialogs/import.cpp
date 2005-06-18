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

#include "import.h"
#include "ktoon.h"

/**
 * @todo 
 * - remove k_toon dependence
 */

//--------------- CONSTRUCTOR --------------------

Import::Import( QWidget *parent ) : QDialog( parent, "Import", true )
{
    Q_CHECK_PTR( parent );

    //Initializations
    setCaption( tr( "Import" ) );
    
    k_toon = ( KToon * ) parent;
    setMinimumSize( 240, 280 );
    setMaximumSize( 240, 280 );

    //------------- Operations on the static texts ----------

    main_text = new QLabel( tr( "Select the kind of file to import:" ), this );
    main_text -> resize( 230, 20 );
    main_text -> move( 10, 10 );

    //------------- Operations on the radio buttons -------------

    group = new QButtonGroup( this );
    group -> move( main_text -> x(), main_text -> y() + main_text -> height() + 10 );
    group -> resize( 220, 190 );

    rb_image = new QRadioButton( tr( "Image" ), group );
    rb_image -> move( 5, 5 );
    rb_image -> setChecked( true );

    rb_image_sequence = new QRadioButton( tr( "Image Sequence" ), group );
    rb_image_sequence -> move( rb_image -> x(), rb_image -> y() + rb_image -> height() );
    rb_image_sequence -> resize( 190, rb_image_sequence -> height() );

    rb_library = new QRadioButton( tr( "Library" ), group );
    rb_library -> move( rb_image_sequence -> x(), rb_image_sequence -> y() + rb_image_sequence -> height() );

    rb_palette = new QRadioButton( tr( "Palette" ), group );
    rb_palette -> move( rb_library -> x(), rb_library -> y() + rb_library -> height() );

    rb_brushes = new QRadioButton( tr( "Brushes" ), group );
    rb_brushes -> move( rb_palette -> x(), rb_palette -> y() + rb_palette -> height() );

    rb_sound = new QRadioButton( tr( "Sound" ), group );
    rb_sound -> move( rb_brushes -> x(), rb_brushes -> y() + rb_brushes -> height() );

    //------------- Operations on the buttons -----------------

    accept = new QPushButton( tr( "Accept" ), this );
    accept -> resize( 60, 30 );
    accept -> move( group -> x() + 50, group -> y() + group -> height() + 10 );
    connect( accept, SIGNAL( clicked() ), SLOT( slotAccept() ) );

    cancel = new QPushButton( tr( "Cancel" ), this );
    cancel -> resize( 60, 30 );
    cancel -> move( accept -> x() + accept -> width() + 10, accept -> y() );
    connect( cancel, SIGNAL( clicked() ), SLOT( slotCancel() ) );
}

//-------------- DESTRUCTOR -----------------

Import::~Import()
{
    delete accept;
    delete cancel;
    delete main_text;
    delete rb_image;
    delete rb_image_sequence;
    delete rb_library;
    delete rb_palette;
    delete rb_brushes;
    delete rb_sound;
    delete group;
}

//--------------- PUBLIC MEMBERS --------------------

//------------------- SLOTS -------------------------

void Import::slotAccept()
{
    switch ( group -> selectedId() )
    {
        case 0:
	{
            QString fn = QFileDialog::getOpenFileName( QString::null, "Image ( *.png *.jpg *.xpm )", this,
	                                               tr( "Import Image" ), tr( "Choose an image to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadImage( fn );
	    break;
	}
        case 1:
	{
            QString fn = QFileDialog::getOpenFileName( QString::null, "Image Sequence ( *.png *.jpg *.xpm )", this,
	                                               tr( "Import Image Sequence" ), tr( "Choose an image Sequence to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadImageSequence( fn );
	    break;
	}
        case 2:
	{
            QString fn = QFileDialog::getOpenFileName( KTOON_REPOSITORY + "/lbr/", "Graphic Library ( *.lbr )", this,
	                                               tr( "Import Library" ), tr( "Choose a graphic library to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadLibrary( fn );
	    break;
	}
        case 3:
	{
            QString fn = QFileDialog::getOpenFileName( KTOON_REPOSITORY + "/cpl/", "Palette ( *.cpl )", this,
	                                               tr( "Import Palette" ), tr( "Choose a color palette to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadPalette( fn );
	    break;
	}
        case 4:
	{
            QString fn = QFileDialog::getOpenFileName( KTOON_REPOSITORY + "/bru/", "Brushes File ( *.bru )", this,
	                                               tr( "Import Brushes" ), tr( "Choose a brush file to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadBrushes( fn );
	    break;
	}
	case 5:
	{
            QString fn = QFileDialog::getOpenFileName( QString::null, "Sound File ( *.wav )", this,
	                                               tr( "Import Sound" ), tr( "Choose a sound file to import" ) );
            if ( !fn.isEmpty() )
	        k_toon -> loadSound( fn );
	    break;
	}
	default: break;
    }

    close( true );
}

void Import::slotCancel()
{
    close( true );
}
