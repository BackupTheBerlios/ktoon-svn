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

#include "sproject.h"
#include <ktsaveproject.h>

#include <dapplicationproperties.h>
#include <ddebug.h>

SProject::SProject(const QString & filename, QObject *parent) : KTProject(parent), m_filename(filename)
{
	m_saver = new QTimer(this);
	m_saver->setInterval( 30000 );
	m_saver->start();
	QObject::connect(m_saver, SIGNAL(timeout ()), this, SLOT(save()));
	startTimer(300000);
}

void SProject::resetTimer()
{
	m_saver->stop();
	m_saver->start();
}

SProject::~SProject()
{
}

void SProject::save()
{
	D_FUNCINFOX("server");
	KTSaveProject *saver = new KTSaveProject;
	
	if(saver->save(m_filename, this))
	{
		emit requestSendErrorMessage( QObject::tr( "project saved"), Packages::Error::Info );
	}
	else
	{
		emit requestSendErrorMessage( QObject::tr( "Error saving project"), Packages::Error::Err );
	}
	delete saver;
}

void SProject::timerEvent ( QTimerEvent *  )
{
	save();
}

QDomElement SProject::info(QDomDocument &doc) const
{
	
	QDomElement project = doc.createElement("project");
	
	project.setAttribute("name", projectName());
	
	QDomElement file = doc.createElement("file");
	
	QString fileName = dAppProp->cacheDir() + "/" + projectName();
	if ( !fileName.endsWith(".ktn") )
	{
		fileName += ".ktn";
	}
	
	file.setAttribute("name", m_filename);
	
	project.appendChild(file);
	
	return project;
}
