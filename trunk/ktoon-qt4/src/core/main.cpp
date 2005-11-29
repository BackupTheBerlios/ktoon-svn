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

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qdir.h>

#include <QTranslator>

#include "ktapplication.h"

#include "ktsplash.h"
#include "ktconfigdocument.h"
#include "ktxmlreader.h"
#include "ktdebug.h"

#include "ktmainwindow.h"

void usage();

int main( int argc, char ** argv )
{	
	KTApplication application( argc, argv );
	
	QApplication::setStyle("plastique");
	
	if ( application.isArg("help") || application.isArg("h") )
	{
		usage();
		application.exit(0);
		return 0;
	}
	
	KTCONFIG->init();

	if ( ! KTCONFIG->isOk() || application.isArg("r") || application.isArg("reconfigure") )
	{
		qDebug("RECONFIGURING");
		if ( ! application.firstRun() /*&& ! (application.isArg("r") || application.isArg("reconfigure"))*/ )
		{
			ktFatal () << "**********************You need configure the application" << endl;
			QMessageBox::critical(0, QObject::tr("Missing..."), QObject::tr("You need configure the application"));
			application.exit(-1);
			return -1;
			
		}
	}
	
	application.setHome(KTCONFIG->read("KTHome"));
	application.setRepository(KTCONFIG->read("Repository"));
	QString themefile = KTCONFIG->read("KTTheme");
	if ( ! themefile.isEmpty() )
	{
		application.applyTheme(themefile);
	}
	
	QTranslator *qttranslator = new QTranslator( 0 );
	
	qttranslator->load( QString( "qt_" ) + QTextCodec::locale(), KTOON_HOME+"/data/translations");

	application.installTranslator( qttranslator );
	
	QTranslator *translator = new QTranslator( 0 );
	translator->load( QString( "ktoon_" ) + QTextCodec::locale(),  KTOON_HOME+"/data/translations");
	
	application.installTranslator( translator );
	
	Splash *splash = new Splash;
	splash->show();
	splash->setMessage( QObject::tr( "Initializing..." ) );
	
	KTMainWindow *mainWindow = new KTMainWindow;
	splash->setMessage( QObject::tr( "Loading Modules" ) );


	splash->setMessage( QObject::tr( "Loaded!" ) );
	
	splash->finish( mainWindow );
	delete splash;

	mainWindow->showMaximized();
	
	QApplication::setActiveWindow(mainWindow);
	QApplication::addLibraryPath (KTOON_HOME+"/plugins");
	
	return application.exec();
}

void usage()
{
#if defined(Q_OS_UNIX)
	puts("\033[1;33m"+ktapp->objectName() + ktapp->version());
	puts(QObject::tr("2D Animation tool kit")+"\033[0;0m" );

	puts("\033[1;34m"+QObject::tr("Usage: %1 [option]").arg(ktapp->argv()[0])+"\033[0;0m");
	
	puts("\033[1;31m"+QObject::tr("Options: "));
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(ktapp->name()));
	
	puts("\033[0;0m");
#else
	puts(ktapp->objectName() + ktapp->version());

	puts(QObject::tr("Usage: %1 [option]").arg(ktapp->argv()[0]));
	
	puts(QObject::tr("Options: "));
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(ktapp->name())); 
#endif
}
