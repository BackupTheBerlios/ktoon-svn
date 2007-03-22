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
 
#include "observer.h"
// #include "noticeparser.h"

#include <dosd.h>
#include <QObject>

namespace Comunications {

Observer::Observer()
 : Base::PackageObserver()
{
}


Observer::~Observer()
{
}

void Observer::handlePackage(Base::Package *const pkg)
{
// 	if(pkg->root() == "notice")
// 	{
// 		NoticeParser parser;
// 		if(parser.parse(pkg->xml()))
// 		{
// 			QString message = QObject::tr("From") + ": "+ parser.from() + "\n" + parser.message();
// 			DOsd::self()->display(message);
// 			pkg->accept();
// 		}
// 	}
}
}