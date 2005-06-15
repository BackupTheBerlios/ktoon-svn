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

#ifndef KTCONFIGDOCUMENT_H
#define KTCONFIGDOCUMENT_H

#include <qdom.h>
#include <qstringlist.h>

/**
 * This class represents the ktoon configuration xml document
 * @author David Cuadrado
*/
class KTConfigDocument : public QDomDocument
{
	public:
    		KTConfigDocument(const QString &path);
    		~KTConfigDocument();
		void setLang(const QString &lang);
		void setHome(const QString &home);
		void setRepository(const QString &repository);
		void addRecentFiles(const QStringList &names);
		void setThemePath(const QString &theme);
		
		QString path();
		
		void saveConfig(const QString &file = QString::null);
		
	private:
		QString m_path;
		const int MAXRECENTS;
};

#endif


