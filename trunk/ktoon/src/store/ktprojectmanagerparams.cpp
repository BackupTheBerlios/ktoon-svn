/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktprojectmanagerparams.h"

struct KTProjectManagerParams::Private
{
    QString projectName;
    QString author;
    QString description;
    QSize dimension;
    int fps;
};

KTProjectManagerParams::KTProjectManagerParams(): k(new Private)
{
}

KTProjectManagerParams::~KTProjectManagerParams()
{
}

void KTProjectManagerParams::setProjectName(const QString &name)
{
    k->projectName = name;
}

QString KTProjectManagerParams::projectName() const
{
    return k->projectName;
}

void KTProjectManagerParams::setAuthor(const QString &author)
{
    k->author = author;
}

QString KTProjectManagerParams::author() const
{
    return k->author;
}

void KTProjectManagerParams::setDescription(const QString &description)
{
    k->description = description;
}

QString KTProjectManagerParams::description() const
{
    return k->description;
}

void KTProjectManagerParams::setDimension(const QSize &dimension)
{
    k->dimension = dimension;
}

QSize KTProjectManagerParams::dimension() const
{
    return k->dimension;
}

void KTProjectManagerParams::setFPS(const int fps)
{
    k->fps = fps;
}

int KTProjectManagerParams::fps() const
{
    return k->fps;
}
