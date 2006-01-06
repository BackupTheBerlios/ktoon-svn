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
#include <QDebug>

SLPGenerator::SLPGenerator(const QString &command, QObject *parent) : QObject(parent), m_command(command)
{
	m_process = new QProcess(this);
	
	connect(m_process, SIGNAL(readyReadStandardOutput ()), this, SLOT(read()));
	connect(m_process, SIGNAL(readyReadStandardError ()), this, SLOT(read()));
}


SLPGenerator::~SLPGenerator()
{
	delete m_process;
}

bool SLPGenerator::generateDocument(const QString &user, const QString &passwd)
{
	m_process->setEnvironment(QStringList());
	m_process->start(m_command, QStringList() << "log" << "--xml" << "--username" << user << "--password" << passwd << "--non-interactive");
	
	if (! m_process->waitForStarted())
	{
		return false;
	}
	
	return true;
}

void SLPGenerator::read()
{
	QTextStream stream(m_process);
	
	QString readed = stream.readAll().trimmed();
	
	if ( ! readed.isEmpty() )
	{
		m_allTextReaded += readed;
	}
	else if ( readed.contains("Connection closed unexpectedly") )
	{
	}
	else
	{
	}
	
	qDebug() << readed;
}


