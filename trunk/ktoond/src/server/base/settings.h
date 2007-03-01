/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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
#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QString>

namespace Base {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Settings
{
	protected:
		Settings();
		
	public:
		~Settings();
		
		static Settings *self();
		
		void setDatabaseDirPath(const QString &dir);
		QString databaseDirPath() const;
		
		void setBackupDirPath(const QString &dir);
		QString backupDirPath() const;
		
	private:
		static Settings *s_settings;
		
		struct Private;
		Private *const d;
};

}

#endif