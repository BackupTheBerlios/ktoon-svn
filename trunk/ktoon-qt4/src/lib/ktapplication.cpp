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

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QLocale>

#include <QApplication>
#include <QMap>

#include "ktdebug.h"

KTApplication::KTApplication(int & argc, char ** argv)
	: QApplication(argc, argv/*, true*/), m_VERSION("0.8alpha")
{
	KTINIT;
	
	QApplication::setEffectEnabled ( Qt::UI_AnimateMenu, true);
	QApplication::setEffectEnabled ( Qt::UI_AnimateCombo, true);
	QApplication::setEffectEnabled ( Qt::UI_FadeMenu, true);
	QApplication::setEffectEnabled ( Qt::UI_FadeTooltip, true);
	
	parseArgs(argc, argv);
	
	setApplicationName("KToon");
	setOrganizationName ("toonka.com");
	
#ifdef Q_WS_X11
	setStyle("plastik");
	m_configDir = QDir::homePath()+"/.ktoon";
#elif defined(Q_WS_WIN)
	m_configDir = QDir::homePath()+"/KToon";
#elif defined(Q_WS_MAC)
	m_configDir = QDir::homePath()+"/.ktoon";
#endif

	QDir home(m_configDir);
	
	if ( ! home.exists() )
	{
		ktDebug() << tr("Initializing ktoon home");
		if ( ! home.mkdir(m_configDir) )
		{
			ktError() << tr("I cannot create ktoon local home %1").arg(m_configDir);
		}
	}
	
	applyColors(Default);

//  	detectOpengl();
}


KTApplication::~KTApplication()
{
	KTEND;
	
	KTCONFIG->sync();
}

void KTApplication::applyTheme(const QString &file)
{
	m_themeManager.applyTheme(file);
}

void KTApplication::applyTheme(const KTThemeDocument &ktd)
{
	m_themeManager.applyTheme(ktd);
}

void KTApplication::applyColors(ColorSchema cs)
{
	QPalette pal = QApplication::palette();
	switch (cs)
	{
		case Default:
		{
			const QColor bg( 239, 237, 223 );
			const QColor bgAlt( 183, 182, 171 );
			
			pal.setColor( QPalette::Text, Qt::black );
			pal.setColor( QPalette::Base, QColor(183, 183, 183) );
			pal.setColor( QPalette::Foreground, 0x3e3e45);
			pal.setColor( QPalette::Background, bg );
		
			pal.setColor( QPalette::Button, bgAlt );
			pal.setColor( QPalette::ButtonText,0x3e3e45 );
		
			pal.setColor( QPalette::Highlight, 0x8f8368 );
			pal.setColor( QPalette::HighlightedText, bg );
			
			pal.setColor( QPalette::Link, 0x596B8C);
			pal.setColor(QPalette::LinkVisited, 0x7B94C1);
			
			int h,s,v;
			bgAlt.getHsv( &h, &s, &v );
			pal.setColor( QPalette::Midlight, QColor( h, s/3, (int)(v * 1.2)/*,QColor::Hsv*/ ) );
			pal.setColor( QPalette::Light, Qt::white );
			pal.setColor( QPalette::Dark, Qt::black );
		}
		break;
		case DarkBlue:
		{
			const QColor bg( 32,32,82 );
			const QColor bgAlt( 57, 64, 98 );
			
			pal.setColor( QPalette::Text, Qt::white );
			pal.setColor( QPalette::Base, bg );
			pal.setColor( QPalette::Foreground, 0xd7d7ef );
			pal.setColor( QPalette::Background, bgAlt );
		
			pal.setColor( QPalette::Button, bgAlt );
			pal.setColor( QPalette::ButtonText, 0xd7d7ef );
		
			pal.setColor( QPalette::Highlight, Qt::white );
			pal.setColor( QPalette::HighlightedText, bg );
			int h,s,v;
			bgAlt.getHsv( &h, &s, &v );
			pal.setColor( QPalette::Midlight, QColor( h, s/3, (int)(v * 1.2)/*,QColor::Hsv*/ ) );
		}
		break;
	}
	applyPalette(pal);
}

void KTApplication::applyPalette(const QPalette &pal)
{
	setPalette(pal);
	
	QWidgetList list = allWidgets ();
	for( int i = 0; i < list.count(); i++ )
	{
		QObject *o = list[i];
		if ( o )
		{
			static_cast<QWidget*>(o)->setPalette(pal);
		}
	}
}

void KTApplication::changeFont(const QFont &font)
{
	QApplication::setFont(font, "QWidget");
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
		QString opt = QString(argv[i]).simplified();
		if ( opt.startsWith("--") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplified();
			m_parseArgs.insert(opt.remove(0,2), arg);
		}
		else if (opt.startsWith("-") )
		{
			QString arg = "";
			if ( argv[i+1] && ! QString(argv[i+1]).startsWith("-") )
				arg = QString(argv[i+1]).simplified();
			
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

QString KTApplication::home()
{
	return m_KTOON_HOME;
}

void KTApplication::setHome(const QString &home)
{
	m_KTOON_HOME = home;
	m_THEME_DIR = home+"/themes/default/";
}

void KTApplication::setThemeDir(const QString &themeDir)
{
	m_THEME_DIR = themeDir;
}

QString KTApplication::themeDir() const
{
	return m_THEME_DIR;
}

/**
 * @todo Simplify this
 */
QString KTApplication::dataDir() const
{
	QString locale = QString(QLocale::system().name()).left(2);
	
	if ( locale.length() < 2 )
	{
		locale = "en";
	}
	
	return m_KTOON_HOME+"/data/"+locale+"/";
}

QString KTApplication::repository()
{
	return m_KTOON_REPOSITORY;
}

void KTApplication::createRepository(const QString &repository)
{
	m_KTOON_REPOSITORY = repository;
	
	QDir repos(m_KTOON_REPOSITORY);
	if ( ! repos.exists() )
	{
		ktDebug() << tr("Initializing repository %1").arg(m_KTOON_REPOSITORY);
		if ( ! repos.mkdir(m_KTOON_REPOSITORY) )
		{
			ktError() << tr("I cannot create the repository");
		}
	}
}

QString KTApplication::configDir()
{
	return m_configDir;
}

void KTApplication::config(const QString &group)
{
}

QString KTApplication::version()
{
	return m_VERSION;
}

bool KTApplication::firstRun()
{
	ConfigWizard *firstDialog = new ConfigWizard;
	QApplication::setActiveWindow(firstDialog);

	if ( firstDialog->exec() != QDialog::Rejected )
	{
		m_KTOON_HOME = firstDialog->home();
		createRepository(firstDialog->repository());
		
		KTCONFIG->beginGroup("General");
		
		KTCONFIG->setValue( "Home", m_KTOON_HOME);
		KTCONFIG->setValue( "Repository", m_KTOON_REPOSITORY);
		
		KTCONFIG->sync();
		
		delete firstDialog;
		
		return true;
	}
	
	delete firstDialog;
	
	return false;
}

// void KTApplication::detectOpengl()
// {
// 	if ( QGLFormat::hasOpenGL() )
// 	{
// 		QGLWidget gl((QWidget *) 0);
// 		gl.makeCurrent();
// 		
// 		ktDebug() << "-> OpenGL detected :)";
// 		if ( gl.format().directRendering() )
// 		{
// 			ktDebug() << "-> Using direct rendering :)";
// 		}
// 		else
// 		{
// 			ktDebug() << "-> No direct rendering";
// 		}
// 	}
// 	else
// 	{
// 		ktDebug() << "-> OpenGL not detected";
// 	}
// }

