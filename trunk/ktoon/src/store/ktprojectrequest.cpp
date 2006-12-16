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

KTProjectRequestArgument::KTProjectRequestArgument()
{
}

KTProjectRequestArgument::KTProjectRequestArgument(const QString &v) : m_value(v)
{
}

KTProjectRequestArgument::~KTProjectRequestArgument()
{
}

void KTProjectRequestArgument::operator = (const QString &value)
{
	setValue(value);
}

void KTProjectRequestArgument::setValue(const QString &value)
{
	m_value = value;
}

bool KTProjectRequestArgument::toBool()
{
	if ( m_value == "false" || m_value == "0" )
	{
		return false;
	}
	
	return true;
}

int KTProjectRequestArgument::toInt()
{
	return m_value.toInt();
}

double KTProjectRequestArgument::toReal()
{
	return m_value.toDouble();
}

QString KTProjectRequestArgument::toString()
{
	return m_value;
}

KTProjectRequest::KTProjectRequest(Action action, const QVariant &data) : m_action(action), m_data(data), m_id(Project)
{
}

KTProjectRequest::KTProjectRequest(const QString &doc) : m_data(doc) // FIXME
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

void KTProjectRequest::setId(int id)
{
	m_id = id;
}

int KTProjectRequest::id() const
{
	return m_id;
}


void KTProjectRequest::setPartName(const QString &name)
{
	m_partName = name;
}

bool KTProjectRequest::isValid() const
{
	return m_data.isValid();
}

KTProjectRequest *KTProjectRequest::clone() const
{
	KTProjectRequest *event = new KTProjectRequest( m_action, data() );
	
	event->setPartName( partName() );
	event->setId( m_id );
	
	
	return event;
}

QVariant KTProjectRequest::data() const
{
	return m_data;
}


