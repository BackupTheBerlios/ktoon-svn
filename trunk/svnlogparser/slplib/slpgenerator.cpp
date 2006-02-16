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

#include "slpgenerator.h"

#include <QTextStream>
#include <QtDebug>
#include <QCoreApplication>

#include <svn_client.h> // from subversion lib

SLPGenerator::SLPGenerator(QObject *parent) : QObject(parent)
{
}


SLPGenerator::~SLPGenerator()
{
}

bool SLPGenerator::generateDocument(const QString &user, const QString &passwd)
{
// 	svn_error_t *error = svn_client_log2 (target.array (pool),
// 	revisionStart.revision (),
// 	revisionEnd.revision (),
// 	limit,
// 	discoverChangedPaths ? 1 : 0,
// 	strictNodeHistory ? 1 : 0,
// 	logReceiver,
// 	entries,
// 	*m_context, // client ctx
// 	pool);
	
	
	return true;
}

