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

#include "ktapplication.h"
#include "configwizard.h" 
#include <iostream>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qobject.h> 

#include "ktdebug.h"

KTApplication::KTApplication(int & argc, char ** argv)
	: QApplication(argc, argv, true), m_VERSION("0.8alpha")
{
	parseArgs(argc, argv);
	
#ifdef Q_WS_X11
	setStyle("plastik");
	m_configDir = QDir::homeDirPath()+"/.ktoon";
#elif defined(Q_WS_WIN)
	m_configDir = QDir::homeDirPath()+"/KToon";
#elif defined(Q_WS_MAC)
	m_configDir = QDir::homeDirPath()+"/.ktoon";
#endif

 	applyColors(Default);
	setFont( QFont( "helvetica", 10 ) );
	
	KTCONFIG->init();
	
	m_KTOON_HOME = KTCONFIG->read("KTHome");
	m_KTOON_REPOSITORY = KTCONFIG->read("Repository");
	QString themefile = KTCONFIG->read("KTTheme");
	if ( ! themefile.isEmpty() )
	{
		applyTheme(themefile);
	}
	
	if ( ! KTCONFIG->isOk() || isArg("r") || isArg("reconfigure") )
	{
		if ( ! firstRun() && ! (isArg("r") || isArg("reconfigure")) )
		{
			QMessageBox::critical(0, QObject::tr("Missing..."), QObject::tr("You need configure the application"));
		}
	}
}


KTApplication::~KTApplication()
{
}

void KTApplication::applyTheme(const QString &file)
{
	m_themeManager.applyTheme(file);
}

void KTApplication::applyTheme(const KTThemeDocument &ktd)
{
	m_themeManager.applyTheme(ktd);
}

void KTApplication::applyColors(ColorScheme cs)
{
	QColorGroup group = QApplication::palette().active();
	switch (cs)
	{
		case Default:
		{
			const QColor bg( 239, 237, 223 );
			const QColor bgAlt( 183, 182, 171 );
			
			group.setColor( QColorGroup::Text, Qt::black );
			group.setColor( QColorGroup::Base, bg );
			group.setColor( QColorGroup::Foreground, 0x3e3e45);
			group.setColor( QColorGroup::Background, bg );
		
			group.setColor( QColorGroup::Button, bgAlt );
			group.setColor( QColorGroup::ButtonText,0x3e3e45 );
		
			group.setColor( QColorGroup::Highlight, QColor(59,104,134) );
			group.setColor( QColorGroup::HighlightedText, bg );
			int h,s,v;
			bgAlt.getHsv( &h, &s, &v );
			group.setColor( QColorGroup::Midlight, QColor( h, s/3, (int)(v * 1.2),QColor::Hsv ) );
		}
		break;
		case DarkBlue:
		{
			const QColor bg( 32,32,82 );
			const QColor bgAlt( 57, 64, 98 );
			
			group.setColor( QColorGroup::Text, Qt::white );
			group.setColor( QColorGroup::Base, bg );
			group.setColor( QColorGroup::Foreground, 0xd7d7ef );
			group.setColor( QColorGroup::Background, bgAlt );
		
			group.setColor( QColorGroup::Button, bgAlt );
			group.setColor( QColorGroup::ButtonText, 0xd7d7ef );
		
			group.setColor( QColorGroup::Highlight, Qt::white );
			group.setColor( QColorGroup::HighlightedText, bg );
			int h,s,v;
			bgAlt.getHsv( &h, &s, &v );
			group.setColor( QColorGroup::Midlight, QColor( h, s/3, (int)(v * 1.2),QColor::Hsv ) );
		}
		break;
	}
	QPalette pal(group, group, group);
	applyPalette(pal);
}

void KTApplication::applyPalette(const QPalette &pal)
{
	setPalette(pal);
	
	if ( mainWidget() )
	{
		QObjectList list = mainWidget()->queryList("QWidget");
		for( uint i = 0; i < list.count(); i++ )
		{
			QObject *o = list[i];
			if ( o )
			{
				static_cast<QWidget*>(o)->setPalette(pal);
			}
		}
		
		mainWidget()->setPalette(pal);
	}
}

void KTApplication::changeFont(const QFont &font)
{
	QApplication::setFont(font, true, "QWidget");
// 	if ( mainWidget() )
// 	{
// 		QObjectList* const list = mainWidget()->queryList("QWidget");
// 		for( QObject *o = list->first(); o; o = list->next() )
// 		{
// 			if ( o )
// 			{
// 				QWidget *widget = static_cast<QWidget*>(o);
// 				widget->setFont(font);
// 			}
// 		}
// 		delete list;
// 	}
}

void KTApplication::parseArgs(int &argc, char **argv)
{
	for(int i = 0; i < argc; i++)
	{
		QString opt = QString(argv[i]).simplifyWhiteSpace();
		if ( opt.startsWith("--") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplifyWhiteSpace();
			m_parseArgs.insert(opt.remove(0,2), arg);
		}
		else if (opt.startsWith("-") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplifyWhiteSpace();
			
			m_parseArgs.insert(opt.remove(0,1), arg );
		}
	}
}

bool KTApplication::isArg(const QString &arg)
{
	return m_parseArgs.keys().contains(arg);
}

QString KTApplication::getParam(const QString &arg)
{
	if ( ! m_parseArgs.contains(arg) )
		return "";
	return m_parseArgs[arg];
}

QString KTApplication::getHome()
{
	return m_KTOON_HOME;
}

void KTApplication::setHome(const QString &home)
{
	m_KTOON_HOME = home;
}

QString KTApplication::getRepository()
{
	return m_KTOON_REPOSITORY;
}

void KTApplication::setRepository(const QString &repos)
{
	m_KTOON_REPOSITORY = repos;
}

QString KTApplication::configDir()
{
	return m_configDir;
}


QString KTApplication::getVersion()
{
	return m_VERSION;
}

bool KTApplication::firstRun()
{
	ConfigWizard firstDialog;
	
#ifdef KTHOME
	firstDialog.setInitialData(KTHOME, QString(KTHOME)+"/projects");
#endif

	if ( firstDialog.exec() != QDialog::Rejected )
	{
		m_KTOON_HOME = firstDialog.getHome();
		m_KTOON_REPOSITORY = firstDialog.getRepos();
		
		initDirectories();
		
		KTCONFIG->configDocument()->setHome( m_KTOON_HOME );
		KTCONFIG->configDocument()->setRepository( m_KTOON_REPOSITORY );
		
// 		KTCONFIG->sync();
		
		return true;
	}
	
	return false;
}

void KTApplication::initDirectories()
{
	QDir home(m_KTOON_HOME);
	
	if ( ! home.exists() )
	{
		ktDebug() << tr("Initializing ktoon home") << endl;
		if ( ! home.mkdir(m_KTOON_HOME) )
		{
			ktError() << tr("I cannot create ktoon home %1").arg(m_KTOON_HOME) << endl;
		}
	}
	
	home.mkdir(m_configDir);
	
	QDir repos (m_KTOON_REPOSITORY);
	if ( ! repos.exists() )
	{
		ktDebug() << tr("Initializing repository %1").arg(m_KTOON_REPOSITORY) << endl;
		if ( ! repos.mkdir(m_KTOON_REPOSITORY) )
		{
			ktError() << tr("I cannot create the repository") << endl;
		}
		
		QStringList files = QStringList() << "bru" << "cpl" << "output" << "lbr" << "components";
		
		for (uint i = 0; i < files.count(); i++)
		{
			QDir tmp(m_KTOON_REPOSITORY+"/"+files[i]+"/");
			
			if ( ! tmp.exists() )
			{
				tmp.mkdir(m_KTOON_REPOSITORY+"/"+files[i]+"/");
			}
		}
	}
}


