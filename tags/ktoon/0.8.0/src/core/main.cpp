/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include <config.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <QLocale>

#include <QTranslator>

#include "ktapplication.h"
#include "dapplicationproperties.h"

#include "ktsplash.h"
#include "dconfigdocument.h"

#include "ddebug.h"

#include "ktmainwindow.h"

#include "crashhandler.h"

#include "ktoonstyle.h"

void usage();

int main( int argc, char ** argv )
{
	KTApplication application( argc, argv );
	
	CrashHandler::init();
	
#ifdef ENABLE_KTOONSTYLE
	QApplication::setStyle(new KToonStyle());
#elif defined(Q_OS_LINUX)
	QApplication::setStyle(new QPlastiqueStyle());
#endif

	if ( application.isArg("help") || application.isArg("h") )
	{
		usage();
		application.exit(0);
		return 0;
	}
	
	DCONFIG->beginGroup("General");
	
	if ( ! DCONFIG->isOk() )
	{
		DCONFIG->setValue("Home", QString::fromLocal8Bit(::getenv("KTOON_HOME")));
		DCONFIG->setValue("Repository", QDir::tempPath() );
	}
	
	dAppProp->setHomeDir(DCONFIG->value("Home").toString());
	application.createRepository(DCONFIG->value("Repository").toString());

	if ( dAppProp->homeDir().isEmpty() || application.isArg("r") || application.isArg("reconfigure") )
	{
		if ( ! application.firstRun() )
		{
			dFatal () << "**********************You need configure the application" << endl;
			QMessageBox::critical(0, QObject::tr("Missing..."), QObject::tr("You need configure the application"));
			application.exit(-1);
			return -1;
			
		}
		
		
		dAppProp->setHomeDir(DCONFIG->value("Home").toString());
		application.createRepository(DCONFIG->value("Repository").toString());
	}
	
	dAppProp->setVersion(VERSION_STR);
	
	QString themefile = DCONFIG->value("ThemeFile").toString();
	if ( ! themefile.isEmpty() )
	{
		application.applyTheme(themefile);
	}
	
	{
		QString locale = QString(QLocale::system().name()).left(2);
	
		if ( locale.length() < 2 )
		{
			locale = "en";
		}
		
		QTranslator *qttranslator = new QTranslator;
		
		qttranslator->load( QString( "qt_" ) + locale, HOME+"/data/translations");
	
		application.installTranslator( qttranslator );
		
		QTranslator *translator = new QTranslator;
		translator->load( QString( "ktoon_" )+locale,  HOME+"/data/translations");
		
		application.installTranslator( translator );
	}
	
// 	QWidget mainWindow;
// 	mainWindow.show();
	KTSplash *splash = new KTSplash;
	splash->show();
	splash->setMessage( QObject::tr( "Initializing..." ) );
	
	splash->setMessage( QObject::tr( "Loading Modules" ) );
	KTMainWindow mainWindow(splash);
	mainWindow.show();
	

	splash->setMessage( QObject::tr( "Loaded!" ) );

	splash->finish( &mainWindow );
	
	
	mainWindow.showMaximized();
	
	delete splash;
	
	QApplication::addLibraryPath (HOME+"/plugins");
	

#ifdef Q_OS_UNIX
	CHANDLER->setConfig(DATA_DIR+"/crashhandler.xml");
	CHANDLER->setImagePath(THEME_DIR+"/icons/");
#endif
	
	if ( argc == 2 )
	{
		QString project = QString(argv[1]);
		mainWindow.openProject( project );
	}
	
	
	return application.exec();
}

void usage()
{
#if defined(Q_OS_UNIX)
	puts(QString("\033[1;33m"+QApplication::applicationName() + dAppProp->version()).toLatin1());
	puts(QString(QObject::tr("2D Animation tool kit")+"\033[0;0m" ).toLatin1());

	puts(QString("\033[1;34m"+QObject::tr("Usage: %1 [option]").arg(dApp->argv()[0])+"\033[0;0m").toLatin1());
	
	puts(QString("\033[1;31m"+QObject::tr("Options: ")).toLatin1());
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(QApplication::applicationName()).toLatin1());
	
	puts("\033[0;0m");
#else
	puts(QString(QApplication::applicationName() + dApp->version()).toLatin1());

	puts(QObject::tr("Usage: %1 [option]").arg(dApp->argv()[0]).toLatin1());
	
	puts(QObject::tr("Options: ").toLatin1());
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(QApplication::applicationName()).toLatin1()); 
#endif
}

