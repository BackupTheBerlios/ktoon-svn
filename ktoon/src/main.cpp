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
#include <qgl.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "ktoon.h"
#include "modal/splash.h"

int main( int argc, char ** argv )
{
    QApplication application( argc, argv );

    QFile settings( "settings" );
    QString language;
    if ( !settings.open( IO_ReadOnly ) )
    {
        qDebug( "Could not open the settings file. Loaded the default language instead." );
        language = "LANG=" + QString( QTextCodec::locale() ).left( 2 );
	if ( language == "LANG=C" )
	    language = "LANG=en";
    }
    else
    {
        QTextStream stream( &settings );
	stream >> language;
    }
    settings.close();

    QTranslator translator( 0 );

    if ( language == "LANG=" )
    {
        language = "LANG=" + QString( QTextCodec::locale() ).left( 2 );
	if ( language == "LANG=C" )
	    language = "LANG=en";
    }

    if ( language == "LANG=es" )
    {
        translator.load( "ktoon_es", "src/trans/" );
        application.installTranslator( &translator );
    }
    else if ( language == "LANG=fr" )
    {
        translator.load( "ktoon_fr", "src/trans/" );
        application.installTranslator( &translator );
    }

    QGLFormat format;
    format.setAlpha( TRUE );
    format.setDepth( TRUE );
    QGLFormat::setDefaultFormat( format );

    Splash *splash_screen = new Splash();
    splash_screen -> show();
    splash_screen -> message( QObject::tr( "Loading Modules" ) );
    Ktoon *main_window = new Ktoon();
    main_window -> setCaption( "KToon" );
    application.setMainWidget( main_window );
    main_window -> show();
    splash_screen -> finish( main_window );
    delete splash_screen;

    application.connect( &application, SIGNAL( lastWindowClosed() ), &application, SLOT( quit() ) );
    return application.exec();
}
