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

#include <qapplication.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "preferences.h"

//--------------- CONSTRUCTOR --------------------

Preferences::Preferences( QWidget *parent ) : QTabDialog( parent, "Application Preferences", true )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    setCaption( tr( "Application Preferences" ) );
    setFont( QFont( "helvetica", 10 ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 220, 230 );
    parent_widget = parent;
    setCancelButton();
    connect( this, SIGNAL( applyButtonPressed() ), SLOT( slotOK() ) );

    //-- 1: LANGUAGE CONFIGURATION --

    language = new QFrame( this );

    language_group = new QButtonGroup( language );
    language_group -> move( 10, 10 );
    language_group -> resize( 180, 130 );

    rb_system = new QRadioButton( tr( "System Default" ), language_group );
    rb_system -> move( 5, 5 );
    rb_system -> resize( 150, rb_system -> height() );

    rb_english = new QRadioButton( tr( "English" ), language_group );
    rb_english -> move( rb_system -> x(), rb_system -> y() + rb_system -> height() );

    rb_spanish = new QRadioButton( tr( "Spanish" ), language_group );
    rb_spanish -> move( rb_english -> x(), rb_english -> y() + rb_english -> height() );

    rb_french = new QRadioButton( tr( "French" ), language_group );
    rb_french -> move( rb_spanish -> x(), rb_spanish -> y() + rb_spanish -> height() );

    addTab( language, tr( "Language" ) );

    QFile settings( "settings" );
    QString language;
    if ( !settings.open( IO_ReadOnly ) )
        rb_english -> setChecked( true );
    else
    {
        QTextStream stream( &settings );
        stream >> language;
    }
    settings.close();

    if ( language == "LANG=en" )
	rb_english -> setChecked( true );
    else if ( language == "LANG=es" )
	rb_spanish -> setChecked( true );
    else if ( language == "LANG=fr" )
	rb_french -> setChecked( true );
    else
        rb_system -> setChecked( true );
}

//-------------- DESTRUCTOR -----------------

Preferences::~Preferences()
{
    delete rb_english;
    delete rb_spanish;
    delete rb_french;
    delete rb_system;
    delete language_group;
    delete language;
}

//------------------- SLOTS -------------------------

void Preferences::slotOK()
{
    // --- LANGUAGE SETTINGS ---

    QFile settings( "settings" );
    QString language;

    switch ( language_group -> selectedId() )
    {
        case 0:
	{
    	    if ( settings.open( IO_WriteOnly ) )
	    {
        	language = "LANG=" + QString( QTextCodec::locale() ).left( 2 ) + "\n";
        	QTextStream stream( &settings );
        	stream << language;
    	    }
	    else
	        QMessageBox::warning( this, tr( "Warning" ), tr( "Could not write to the settings file" ) );
	    break;
	}
        case 1:
	{
    	    if ( settings.open( IO_WriteOnly ) )
	    {
        	language = "LANG=en\n";
        	QTextStream stream( &settings );
        	stream << language;
    	    }
	    else
	        QMessageBox::warning( this, tr( "Warning" ), tr( "Could not write to the settings file" ) );
	    break;
	}
        case 2:
	{
    	    if ( settings.open( IO_WriteOnly ) )
	    {
        	language = "LANG=es\n";
        	QTextStream stream( &settings );
        	stream << language;
    	    }
	    else
	        QMessageBox::warning( this, tr( "Warning" ), tr( "Could not write to the settings file" ) );
	    break;
	}
        case 3:
	{
    	    if ( settings.open( IO_WriteOnly ) )
	    {
        	language = "LANG=fr\n";
        	QTextStream stream( &settings );
        	stream << language;
    	    }
	    else
	        QMessageBox::warning( this, tr( "Warning" ), tr( "Could not write to the settings file" ) );
	    break;
	}
	default: break;
    }

    settings.close();
    QMessageBox::information( this, tr( "Information" ), tr( "You must restart the application in order to\nsome changes take effect" ) );
}
