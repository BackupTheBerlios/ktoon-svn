/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#include <qapplication.h>
#include <qstring.h>
#include <qmap.h>

typedef QMap<QString, QString> ParseArgs;

/**
 * 
 * @author David Cuadrado
*/
class KTApplication : public QApplication
{
	Q_OBJECT
	public:
		KTApplication(int & argc, char ** argv);
		~KTApplication();
		QString getHome();
		void setHome(const QString &home);
		QString getRepository();
		void setRepository(const QString &repos);
		QString getVersion();

		bool firstRun();
		
		void initRepository();
		
		void parseArgs( int &argc, char **argv);
		
		bool isArg(const QString &arg);
		QString getParam(const QString &arg);
		
	private:
		QString m_KTOON_HOME;
		QString m_KTOON_REPOSITORY;
		const QString m_VERSION;
		ParseArgs m_parseArgs;
};

#define ktapp static_cast<KTApplication*>(qApp)
#define KTOON_HOME (static_cast<KTApplication*>(qApp))->getHome()

#define KTOON_REPOSITORY (static_cast<KTApplication*>(qApp))->getRepository()

#endif
