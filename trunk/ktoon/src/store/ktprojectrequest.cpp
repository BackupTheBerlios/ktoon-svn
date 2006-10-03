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

#include "ktprojectrequest.h"

#include <ddebug.h>

KTProjectRequest::KTProjectRequest(Action action, const QVariant &data) : m_action(action), m_data(data)
{
}


KTProjectRequest::~KTProjectRequest()
{
}


KTProjectRequest::Action KTProjectRequest::action() const
{
	return m_action;
}

QString KTProjectRequest::partName() const
{
	return m_partName;
}

int KTProjectRequest::id() const
{
	return Project;
}


void KTProjectRequest::setPartName(const QString &name)
{
	m_partName = name;
}

bool KTProjectRequest::isValid() const
{
	return false;
}

KTProjectRequest *KTProjectRequest::clone() const
{
	KTProjectRequest *event = new KTProjectRequest( m_action, data() );
	
	event->setPartName( partName() );
	
	return event;
}

QVariant KTProjectRequest::data() const
{
	return m_data;
}

