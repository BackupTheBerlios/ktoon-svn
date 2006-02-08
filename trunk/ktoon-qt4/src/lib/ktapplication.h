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

#ifndef KTAPPLICATION_H
#define KTAPPLICATION_H

#include <QApplication>
#include <QMap>
#include <QColorGroup>

#include "ktthememanager.h"
#include "ktconfig.h"

class QApplication;
class QString;
class KTThemeDocument;
class QPalette;

typedef QMap<QString, QString> ParseArgs;

/**
 * @author David Cuadrado
 * @TODO:
 * - We need have a cache directory (like /tmp in un*x)
*/
class KTApplication : public QApplication
{
	Q_OBJECT
	public:
		enum ColorSchema { Default = 0, DarkBlue };
		KTApplication(int & argc, char ** argv);
		~KTApplication();
		QString home();
		void setHome(const QString &home);
		
		QString themeDir() const;
		void setThemeDir(const QString &themeDir);
		
		QString dataDir() const;
		
		QString repository();
		void createRepository(const QString &repository);
		QString version();
		
		void parseArgs( int &argc, char **argv);
		
		void applyColors(ColorSchema cs);
		void applyPalette( const QPalette &p );
		void applyTheme(const QString &file);
		void applyTheme(const KTThemeDocument &ktd);
		
		bool isArg(const QString &arg);
		QString getParam(const QString &arg);
		
		void changeFont(const QFont &font); // static?
		
		QString configDir();
		
		void config(const QString &group = "General"); // FIXME: return
		
	public slots:
		bool firstRun();
		
	protected:
// 		virtual void detectOpengl();
		
	private:
		QString m_KTOON_HOME;
		QString m_THEME_DIR;
		QString m_KTOON_REPOSITORY;
		QString m_configDir;
		const QString m_VERSION;
		ParseArgs m_parseArgs;
		KTThemeManager m_themeManager;
		bool m_haveDri;
};

#include "ktglobal.h"

#endif
