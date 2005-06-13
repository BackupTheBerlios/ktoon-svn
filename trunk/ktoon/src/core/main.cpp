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

#include <ktapplication.h>
#include <qgl.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qdir.h>

#include "ktapplication.h"
#include "ktoon.h"
#include "splash.h"
#include "ktconfigdocument.h"
#include "ktxmlreader.h"

void usage();

int main( int argc, char ** argv )
{
	KTApplication application( argc, argv );
	
	if ( application.isArg("help") || application.isArg("h") )
	{
		usage();
		application.exit(0);
		return 0;
	}
		
	KTConfigDocument ktconfig(QDir::homeDirPath()+QString("/.ktoonrc"));
	
	if ( ! QFile::exists( ktconfig.path() ) || application.isArg("r") || application.isArg("reconfigure") )
	{
		if ( application.firstRun() )
		{
			ktconfig.setHome(application.getHome());
			ktconfig.setRepository(application.getRepository());
			
			ktconfig.saveConfig();
		}
		else if ( ! (application.isArg("r") || application.isArg("reconfigure")) )
		{
			QMessageBox::critical(0, QObject::tr("Missing..."), QObject::tr("You need configure the application"));
			return -1;
		}
	}
// 	else
	{
		// Analizamos el documento XML de configuracion y ponemos las variables en application
		KTXmlReader reader;
		QFile f(ktconfig.path());
		QXmlInputSource xmlsource(&f);
		if ( reader.parse(&xmlsource))
		{
			XMLResults results = reader.getResults();
			
			application.setHome(results["KTHome"]);
			application.setRepository(results["Repository"]);
		}
		else
		{
			QMessageBox::critical(0, QObject::tr("Failed..."), QObject::tr("Sorry, your configuration file is corrupted, please remove and try again."));
			return -1;
		}
	}
	
	application.initRepository();
	
	QTranslator *qttranslator = new QTranslator( 0 );
	
	qttranslator->load( QString( "qt_" ) + QTextCodec::locale(), KTOON_HOME+"/data/translations");

	application.installTranslator( qttranslator );
	
	QTranslator *translator = new QTranslator( 0 );
	translator->load( QString( "ktoon_" ) + QTextCodec::locale(),  KTOON_HOME+"/data/translations");
	
	application.installTranslator( translator );
	
	QGLFormat format;
	format.setAlpha( TRUE );
	format.setDepth( TRUE );
	QGLFormat::setDefaultFormat( format );
	
	Splash *splash_screen = new Splash();
	splash_screen -> show();
	splash_screen -> message( QObject::tr( "Loading Modules" ) );
	
	KToon *main_window = new KToon();
	
	main_window -> setCaption( "KToon" );
	application.setMainWidget( main_window );
	main_window -> showMaximized();
	
	splash_screen -> finish( main_window );
	delete splash_screen;
	
	application.connect( &application, SIGNAL( lastWindowClosed() ), &application, SLOT( quit()) );
	
	return application.exec();
}

void usage()
{
	std::cout << ktapp->name() << " " << ktapp->getVersion() << std::endl;
	std::cout << std::endl;
	
	std::cout << QObject::tr("Usage: %1 [option]").arg(ktapp->argv()[0]) << std::endl;
	
	std::cout << std::endl;
	std::cout << QObject::tr("Options: ") << std::endl;
	
	std::cout << "-r, --reconfigure" << std::endl;
	std::cout << QObject::tr("\t\tReconfigure %1").arg(ktapp->name()) << std::endl;
}
