/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
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
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "settings.h"

namespace Base {

Settings *Settings::s_settings = 0;

struct Settings::Private 
{
    QString databaseDirPath;
    QString backupDirPath;
};

Settings::Settings() : k(new Private())
{
}

Settings::~Settings()
{
    delete k;
}

Settings *Settings::self()
{
    if (! s_settings)
        s_settings = new Settings();
    
    return s_settings;
}

void Settings::setDatabaseDirPath(const QString &dbdir)
{
    k->databaseDirPath = dbdir;
}

QString Settings::databaseDirPath() const
{
    return k->databaseDirPath;
}

void Settings::setBackupDirPath(const QString &dir)
{
    k->backupDirPath = dir;
}

QString Settings::backupDirPath() const
{
    return k->backupDirPath;
}

}
