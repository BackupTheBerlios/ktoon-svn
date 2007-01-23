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

SProject::SProject(QObject *parent) : KTProject(parent)
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
	bool result = false;
	QString fileName = dAppProp->cacheDir() + "/" + projectName();
	SHOW_VAR(fileName);
	KTSaveProject *saver = 0;
	
	if ( !fileName.endsWith(".ktn") )
	{
		fileName += ".ktn";
	}
	saver = new KTSaveProject;
	result = saver->save(fileName, this);
	delete saver;
}

void SProject::timerEvent ( QTimerEvent * event )
{
	save();
}
