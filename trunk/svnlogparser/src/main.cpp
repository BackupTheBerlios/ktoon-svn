/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>

#include "slpparser.h"

#include "slpgenerator.h"

// I start this project at: 2006-01-05 17:00:00

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
	SLPGenerator generator;
	
	generator.generateDocument("fdsa", "fdsa");
	
// 	SLPParser parser;
// 	QXmlSimpleReader reader;
// 	reader.setContentHandler(&parser);
// 	reader.setErrorHandler(&parser);
// 		
// 	QFile source("svnlog.xml");
// 	QXmlInputSource xmlsource(&source);
// 		
// 	if ( reader.parse(&xmlsource) )
// 	{
// 		foreach(SLPEntry entry, parser.entries() )
// 		{
// 			entry.debug();
// 		}
// 	}
// 	else
// 	{
// 		qDebug() << "ERROR";
// 	}
	
	
// 	return app.exec();
	return 0;
}
