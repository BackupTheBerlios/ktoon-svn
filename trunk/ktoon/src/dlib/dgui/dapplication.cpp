/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dapplication.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QLocale>

#include <QApplication>
#include <QMap>

#include "ddebug.h"
#include "dactionmanager.h"

DApplication::DApplication(int & argc, char ** argv) : QApplication(argc, argv)
{
	DINIT;
	
	QApplication::setEffectEnabled( Qt::UI_AnimateMenu, true);
	QApplication::setEffectEnabled( Qt::UI_AnimateCombo, true);
	QApplication::setEffectEnabled( Qt::UI_FadeMenu, true);
	QApplication::setEffectEnabled( Qt::UI_FadeTooltip, true);
	
	parseArgs(argc, argv);
	
	m_actionManager = new DActionManager(this);
}


DApplication::~DApplication()
{
	DEND;
	
	DCONFIG->sync();
}

void DApplication::applyTheme(const QString &file)
{
	m_themeManager.applyTheme(file);
}

void DApplication::applyTheme(const DThemeDocument &dd)
{
	m_themeManager.applyTheme(dd);
}

void DApplication::applyColors(ColorSchema cs)
{
	QPalette pal = QApplication::palette();
	switch (cs)
	{
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

void DApplication::applyPalette(const QPalette &pal)
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

void DApplication::changeFont(const QFont &font)
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

DConfig *DApplication::config(const QString &group )
{
	DConfig *config = DConfig::instance();
	
	config->beginGroup( group );
	
	return config;
}

void DApplication::parseArgs(int &argc, char **argv)
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

bool DApplication::isArg(const QString &arg)
{
	return m_parseArgs.keys().contains(arg);
}

QString DApplication::getParam(const QString &arg)
{
	if ( ! m_parseArgs.contains(arg) )
		return "";
	return m_parseArgs[arg];
}

bool DApplication::firstRun() 
{
	return false;
}

bool DApplication::insertGlobalAction(QAction *action, const QString& id)
{
	if (m_actionManager->insert( action, id, "global") )
	{
		action->setShortcutContext(Qt::ApplicationShortcut);
		return true;
	}
	
	return false;
}

void DApplication::removeGlobalAction(QAction *action)
{
	m_actionManager->remove( action, "global");
}

QAction *DApplication::findGlobalAction(const QString &id)
{
	return m_actionManager->find( id, "global");
}

