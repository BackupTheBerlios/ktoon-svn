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

#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QString>
#include "backupdatabase.h"

#include "base/observer.h"

namespace Base {
class Package;
}

namespace Backups {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Manager : public Base::Observer
{
	public:
		Manager();
		~Manager();
		
		bool makeBackup(const QString &filepath, const QDateTime &date, const QString &name);
		bool removeBackup(const QString &name, const QDateTime &date);
		bool restoreBackup(const QString &name, const QDateTime &date);
		
		QHash<QString, QList<Backups::Database::Entry> > entries();
		
		void handlePackage(Base::Package *const pkg);
		
		QDateTime date(const QString &name);
		
	private:
		struct Private;
		Private *const d;
};

}

#endif


