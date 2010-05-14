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

#include <kcore/kdebug.h>

#include "project.h"
#include "ktsaveproject.h"
#include "ktproject.h"

#include <QFile>

/*
<!-- Answer to valid open request -->
<project version="0">
        <data>
                <![CDATA[KTN encoded with Base64]]>
        </data>
</project>
*/

namespace Packages
{

struct Project::Private
{
    QDomElement root;
    QDomElement data;
    bool isValid;
};

Project::Project(const QString & projectPath): Package(), k(new Private)
{
    k->root = createElement("project");
    k->root.setAttribute("version", "0");
    appendChild(k->root);
    setProject(projectPath);
}

Project::~Project()
{
}

void Project::setProject(const QString & projectPath)
{
    QFile file(projectPath);
    KTSaveProject loader;
    KTProject *project = new KTProject;
    k->isValid = loader.load(projectPath, project);

    if (k->isValid) {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll().toBase64();
        
        removeChild(k->data);
        k->data = createElement("data");
        k->data.appendChild(createCDATASection(data));
        k->root.appendChild(k->data);
    }
}

bool Project::isValid()
{
    return k->isValid;
}

}
