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

#include "right.h"

#include <kcore/kdebug.h>

namespace Users {

struct Right::Private
{
    QString module;
    bool read;
    bool write;
};

Right::Right() : k(new Private())
{
    k->read = false;
    k->write = false;
}

Right::Right(const QString &module, bool read, bool write) : k(new Private)
{
    k->module = module;
    k->read = read;
    k->write = write;
}

Right::~Right()
{
    delete k;
}

void Right::setRead(bool enable)
{
    k->read = enable;
}

void Right::setWrite(bool enable)
{
    k->write = enable;
}

QString Right::module() const
{
    return k->module;
}

bool Right::write() const
{
    return k->write;
}

bool Right::read() const
{
    return k->read;
}

}

