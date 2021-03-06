/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <ktapplication.h>
#include "ktsplash.h"
#include "ktmainwindow.h"
#include <ktwitter.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qdir.h>

#include <QLocale>
#include <QTranslator>
#include <QDesktopWidget>
#include <QThread>

#include <kcore/kapplicationproperties.h>
#include <kcore/kdebug.h>
#include <kgui/kcollapsiblewidget.h>
#include <ksound/kaudioplayer.h>

#ifdef Q_OS_UNIX
#include "ktcrashhandler.h"
#endif

#ifdef ENABLE_KTOONSTYLE
#include "kwaitstyle.h"
#endif

#include <QPlastiqueStyle>

#ifndef CONFIG_H
#define CONFIG_H
#endif

#ifndef HAVE_SOUND
#define HAVE_SOUND
#endif

#ifndef HAVE_FFMPEG
#define HAVE_FFMPEG
#endif

class SleeperThread : public QThread
{
    public:
        static void msleep(unsigned long msecs)
        {
            QThread::msleep(msecs);
        }
};

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

int main(int argc, char ** argv)
{
    KTApplication application(argc, argv);

    // If help es required as argument from command line, show the options console help
    if (application.isArg("help") || application.isArg("h")) {
        usage();
        application.exit(0);
        return 0;
    }

    // qsrand(::time(0));

#ifdef Q_OS_UNIX
    // Initializing the crash handler, very useful to catch bugs
    KTCrashHandler::init();
#endif

    // Setting the current version for KToon
    kAppProp->setVersion(VERSION);
    kAppProp->setCodeName(CODE_NAME);
    kAppProp->setRevision(REVISION);

    // Time to define global variables for KToon
    KCONFIG->beginGroup("General");

    if (! KCONFIG->isOk()) {
        KCONFIG->setValue("Home", QString::fromLocal8Bit(::getenv("KTOON_HOME")));
        KCONFIG->setValue("Cache", QDir::tempPath());
    }

    kAppProp->setHomeDir(KCONFIG->value("Home").toString());
    kAppProp->setShareDir(QString::fromLocal8Bit(::getenv("KTOON_SHARE")));

    QString locale = QString(QLocale::system().name()).left(2);

    if (locale.length() < 2)
        locale = "en";

    QDir dir(SHARE_DIR + "data/" + locale + "/");
    if (! dir.exists())
        kAppProp->setDataDir(SHARE_DIR + "data/en/");
    else
        kAppProp->setDataDir(SHARE_DIR + "data/" + locale + "/");

    kAppProp->setThemeDir(SHARE_DIR + "themes/default" + "/");

    /*
    kDebug() << "HOME_DIR: " << HOME_DIR;
    kDebug() << "SHARE_DIR: " << SHARE_DIR;
    kDebug() << "DATA_DIR: " << DATA_DIR;
    kDebug() << "THEME_DIR: " << THEME_DIR;
    */

    // Setting the repository directory (where the projects are saved)
    application.createCache(KCONFIG->value("Cache").toString());

    // Downloading ktoon_net Twitter status
    KTwitter *ktwitter = new KTwitter();
    ktwitter->start();

    // SQA: KTooN gui styles must be re-factored 
    // Setting the gui style for the interface
#ifdef ENABLE_KTOONSTYLE
    QApplication::setStyle(new KWaitStyle());
#elif defined(Q_OS_LINUX)
    QApplication::setStyle(new QPlastiqueStyle());
#endif

    // If user asked for reconfigure KToon or if this is the first time the application is launched
    if (kAppProp->homeDir().isEmpty() || application.isArg("r") || application.isArg("reconfigure")) {
        // Launching the basic configuration dialog
        if (! application.firstRun()) {
            // If dialog is canceled or KToon can not be configured, kill the whole application
            #ifdef K_DEBUG
                  kFatal () << "********************* You need configure the application" << endl;
           #endif

           QMessageBox::critical(0, QObject::tr("Missing..."), 
                                 QObject::tr("You need configure the application"));
           application.exit(-1);
           return -1;
        }

        // Setting the new global variables for KToon
        kAppProp->setHomeDir(KCONFIG->value("Home").toString());
        application.createCache(KCONFIG->value("Cache").toString());
    }

    // Time to apply the theme for the application GUI
    QString themefile = KCONFIG->value("ThemeFile").toString();

    if (! themefile.isEmpty()) 
        application.applyTheme(themefile);

    // Loading localization files... now you got KToon in your native language

    QTranslator *translator = new QTranslator;
    translator->load(SHARE_DIR + "data/translations/" + "ktoon_" + locale + ".qm");
    application.installTranslator(translator);

    // Time to show the KToon initial splash 
    KTSplash *splash = new KTSplash;
    splash->show();
    QDesktopWidget desktop;
    splash->move((int) (desktop.screenGeometry().width() - splash->width())/2, 
                 (int) (desktop.screenGeometry().height() - splash->height())/2);

    splash->setMessage(QObject::tr("Initializing..."));
    SleeperThread::msleep(500);

    splash->setMessage(QObject::tr("Loading modules..."));
    SleeperThread::msleep(500);

    KTMainWindow mainWindow(splash, argc);

    splash->setMessage(QObject::tr("Loaded!"));
    SleeperThread::msleep(500);

    mainWindow.showMaximized();

    delete splash;

    // Looking for plugins for KToon
    #ifdef K_DEBUG
           kWarning() << "Loading plugins from: " << SHARE_DIR << " + plugins";
    #endif
    QApplication::addLibraryPath(SHARE_DIR + "plugins");

    // Loading visual components required for the Crash Handler
    #ifdef Q_OS_UNIX
           CHANDLER->setConfig(DATA_DIR + "crashhandler.xml");
           CHANDLER->setImagePath(THEME_DIR + "icons/");
    #endif

    // If there is a second argument, it means to open a project from the command line
    if (argc == 2) {
        QString project = QString(argv[1]);
        mainWindow.openProject(project);
    }

    // It's time to play with KToon!	
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

