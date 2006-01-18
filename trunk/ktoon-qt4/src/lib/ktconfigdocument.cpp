/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktconfigdocument.h"

#include <qfile.h>
#include <QTextStream>
#include <qobject.h>

#include "ktdebug.h"

KTConfigDocument::KTConfigDocument(const QString &path) : QDomDocument(), m_path(path)
{
	KTINIT;
	setup();

	if ( !m_isOk )
	{
		QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
		this->appendChild(header);
		
		QDomElement root = createElement( "KTConfig" );
		appendChild( root );
	}
}

void KTConfigDocument::setup()
{
	QFile config( m_path );
	m_isOk = false;
	
	if ( config.exists() )
	{
		QString errorMsg = "";
		int errorLine; 
		int errorColumn;
		if ( setContent(&config, &errorMsg, &errorLine, &errorColumn) )
		{
			m_isOk = true;
		}
		else
		{
			ktDebug() << QObject::tr("Configuration file is corrupted %1:%2: %3").arg(errorLine).arg(errorColumn).arg(errorMsg);
			m_isOk = false;
		}
		config.close();
	}
}


KTConfigDocument::~KTConfigDocument()
{
	KTEND;
}


void KTConfigDocument::beginGroup(const QString & prefix )
{
	if ( m_groups.contains(prefix) )
	{
		m_currentGroup = m_groups[prefix];
	}
	else // Create element
	{
		m_currentGroup = find(documentElement(), prefix);
		
		if ( m_currentGroup.isNull() )
		{
// 			ktDebug() << "Creando GRUPO: " << prefix;
			m_currentGroup = createElement(prefix);
			documentElement().appendChild(m_currentGroup);
		}
		
// 		m_currentGroup = element;
	}
}

void KTConfigDocument::setValue ( const QString & key, const QVariant & value )
{
	QDomElement element = find(m_currentGroup, key);
	
	if ( !element.isNull () )
	{
		element.setAttribute("value", value.toString());
	}
	else
	{
		element = createElement(key);
		element.setAttribute("value", value.toString());
		m_currentGroup.appendChild(element);
	}
}

QVariant KTConfigDocument::value ( const QString & key, const QVariant & defaultValue) const
{
	QDomElement element = find(m_currentGroup, key); // Current group or root?
	
	if ( element.isNull() )
	{
		return defaultValue;
	}
	
	return element.attribute("value");
}


QDomElement KTConfigDocument::find(const QDomElement &element, const QString &key) const 
{
	QDomElement recent;
	QDomNode n = element.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if ( e.tagName() == key )
			{
				recent = e;
				break;
			}
		}
		n = n.nextSibling();
	}
	
	return recent;
}

QString KTConfigDocument::path()
{
	return m_path;
}

void KTConfigDocument::saveConfig(const QString &file)
{
	QFile *f = 0;
	if ( file.isNull() || file.isEmpty() )
	{
		f = new QFile(path());
	}
	else
	{
		f = new QFile(file);
	}
		
	if ( f->open(QIODevice::WriteOnly) )
	{
		QTextStream st( f );
		st << toString() << endl;
		m_isOk = true;
		f->close();
	}
	else
	{
		m_isOk = false;
	}
}

bool KTConfigDocument::isOk()
{
	return m_isOk;
}
