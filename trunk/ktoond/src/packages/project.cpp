/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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


#include "project.h"

#include <QFile>
/*
<!-- Respuesta a una peticion open valida -->
<project version="0">
        <data>
                <![CDATA[KTN encoded with Base64]]>
        </data>
</project>
*/

namespace Packages
{
	
Project::Project(const QString & projectPath)
 : Package()
{
	QDomElement root = createElement ( "project" );
	root.setAttribute ( "version",  "0" );
	appendChild(root);
	setProject(projectPath);
}


Project::~Project()
{
}

void Project::setProject(const QString & projectPath)
{
	QFile file(projectPath);
	QByteArray data = file.readAll().toBase64();
	
	removeChild(m_data);
	m_data = createElement("data");
	m_data.appendChild(createCDATASection ( data ));
	appendChild(m_data);
}

}
