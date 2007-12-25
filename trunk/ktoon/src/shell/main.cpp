/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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
#include <QLocale>

#include <QTranslator>

#include "ktapplication.h"

#include <kcore/kapplicationproperties.h>
#include <kcore/kdebug.h>
#include <kgui/kcollapsiblewidget.h>
#include <ksound/kaudioplayer.h>

#include "ktsplash.h"

#include "ktmainwindow.h"

#include "crashhandler.h"

#ifdef ENABLE_KTOONSTYLE
#include "kwaitstyle.h"
#endif

#include <QPlastiqueStyle>

#ifndef CONFIG_H
#define CONFIG_H
#define HAVE_LIBGIF
#define VERSION 0.9+svn
#define VERSION_STR "0.9+svn"
#endif

#ifndef HAVE_SOUND
#define HAVE_SOUND
#endif

#ifndef HAVE_FFMPEG
#define HAVE_FFMPEG
#endif

/**
 * Main class of the application. 
 * This is the code where KToon application starts.
 * @author David Cuadrado <krawek@gmail.com>
*/

/** 
 * This method defines the console line options to run KToon
 */
void usage();

/** 
 * This method is the first one invoked when KToon is launched
 */

int main( int argc, char ** argv )
{
	KTApplication application( argc, argv );
	
	qsrand( ::time(0) );
	
	// Initializing the crash handler, very useful to catch bugs
	CrashHandler::init();
	
	// Setting the gui style for the interface
#ifdef ENABLE_KTOONSTYLE
	QApplication::setStyle(new KWaitStyle());
#elif defined(Q_OS_LINUX)
	QApplication::setStyle(new QPlastiqueStyle());
#endif
	// If help es required as argument from command line, show the options console help
	if ( application.isArg("help") || application.isArg("h") )
	{
		usage();
		application.exit(0);
		return 0;
	}
	
	// Time to define global variables for KToon
	KCONFIG->beginGroup("General");
	
	if ( ! KCONFIG->isOk() )
	{
		KCONFIG->setValue("Home", QString::fromLocal8Bit(::getenv("KTOON_HOME")));
		KCONFIG->setValue("Cache", QDir::tempPath() );
	}
	
	kAppProp->setHomeDir(KCONFIG->value("Home").toString());

	// Setting the repository directory (where the projects are saved)
	application.createCache(KCONFIG->value("Cache").toString());

	// If user asked for reconfigure KToon or if this is the first time the application is launched
	if ( kAppProp->homeDir().isEmpty() || application.isArg("r") || application.isArg("reconfigure") )
	{
		// Launching the basic configuration dialog
		if ( ! application.firstRun() )
		{
			// If dialog is canceled or KToon can not be configured, kill the whole application
			kFatal () << "**********************You need configure the application" << endl;
			QMessageBox::critical(0, QObject::tr("Missing..."), 
					      QObject::tr("You need configure the application"));
			application.exit(-1);
			return -1;
		}
		
		// Setting the new global variables for KToon
		kAppProp->setHomeDir(KCONFIG->value("Home").toString());
		application.createCache(KCONFIG->value("Cache").toString());
	}
	
	// Setting the current version for KToon
	kAppProp->setVersion(VERSION_STR);
	
	// Time to apply the theme for the application GUI
	QString themefile = KCONFIG->value("ThemeFile").toString();
	if ( ! themefile.isEmpty() )
	{
		application.applyTheme(themefile);
	}

	// Loading localization files... now you got KToon in your native language	
	{
		QString locale = QString(QLocale::system().name()).left(2);
	
		if ( locale.length() < 2 )
		{
			locale = "en";
		}
		
		QTranslator *qttranslator = new QTranslator(&application);
		
		qttranslator->load( QString( "qt_" ) + locale, HOME_DIR+"/data/translations");
	
		application.installTranslator( qttranslator );
		
		QTranslator *translator = new QTranslator(&application);
		translator->load( QString( "ktoon_" )+locale,  HOME_DIR+"/data/translations");
		
		application.installTranslator( translator );
	}
	
	// Time to show the KToon initial splash 
	KTSplash *splash = new KTSplash;
	splash->show();
	splash->setMessage( QObject::tr( "Initializing..." ) );
	
	splash->setMessage( QObject::tr( "Loading Modules" ) );
	KTMainWindow mainWindow(splash);
	
	splash->setMessage( QObject::tr( "Loaded!" ) );

        // 	splash->finish( &mainWindow );
	
	mainWindow.showMaximized();
	
	delete splash;

	// Looking for plugins for KToon
	kWarning() << "Loading plugins from: " << HOME_DIR << " + /plugins";
	QApplication::addLibraryPath (HOME_DIR + "/plugins");

	// Loading visual components required for the Crash Handler
#ifdef Q_OS_UNIX
	CHANDLER->setConfig(DATA_DIR+"/crashhandler.xml");
	CHANDLER->setImagePath(THEME_DIR+"/icons/");
#endif
	// If user added a second argument, it means, he wants to load a project from the command line
	if ( argc == 2 )
	{
		QString project = QString(argv[1]);
		mainWindow.openProject( project );
	}

	// It is time to play with KToon!	
	return application.exec();
}

/** 
 * This method defines the console line options to run KToon
 */

void usage()
{
#if defined(Q_OS_UNIX)
        // Characters \033[1;33m and \033[1;34m are useful for colored messages
	puts(QString("\033[1;33m"+QApplication::applicationName() + kAppProp->version()).toLocal8Bit());
	puts(QString(QObject::tr("2D Animation Toolkit")+"\033[0;0m" ).toLocal8Bit());

	puts(QString("\033[1;34m"+QObject::tr("Usage: %1 [option]").arg(kApp->argv()[0])+"\033[0;0m").toLocal8Bit());
	
	puts(QString("\033[1;31m"+QObject::tr("Options: ")).toLocal8Bit());
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(QApplication::applicationName()).toLocal8Bit());
	
	puts("\033[0;0m");
#else
	puts(QString(QApplication::applicationName() + kApp->version()).toLocal8Bit());

	puts(QObject::tr("Usage: %1 [option]").arg(kApp->argv()[0]).toLocal8Bit());
	
	puts(QObject::tr("Options: ").toLocal8Bit());
	
	puts("-r, --reconfigure");
	puts(QObject::tr("\t\tReconfigure %1").arg(QApplication::applicationName()).toLocal8Bit()); 
#endif
}

