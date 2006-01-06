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

#include "slpparser.h"

#include <QDebug>

SLPParser::SLPParser() : QXmlDefaultHandler()
{
}

SLPParser::~SLPParser()
{
}

bool SLPParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		qDebug() << "Root: "<< qname;
		m_root = qname;
	}
	else if ( m_root == "log" )
	{
		if ( qname == "logentry" )
		{
			m_entry = SLPEntry();
			m_entry.setRevision(atts.value("revision").toInt() );
		}
	}
	
	return true;
}

bool SLPParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "log")
	{
		if ( qname == "logentry" )
		{
// 			qDebug() << "AñadientD";
			m_entries << m_entry;
		}
	}
	
	m_qname = "";
	
	return true;
}

bool SLPParser::characters ( const QString & ch )
{
	if ( m_qname == "msg" )
	{
		m_entry.setMsg(ch.trimmed());
	}
	else if ( m_qname == "date")
	{
		m_entry.setDate( QDateTime::fromString( ch.trimmed().left( ch.length() - 4),"yyyy-MM-ddTHH:mm:ss.zzz") );
	}
	else if ( m_qname == "author" )
	{
		m_entry.setAuthor(ch.trimmed());
	}
	
	return true;
}

bool SLPParser::error ( const QXmlParseException & exception )
{
	qDebug() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}


bool SLPParser::fatalError ( const QXmlParseException & exception )
{
	qDebug() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

SLPEntries SLPParser::entries() const
{
	return m_entries;
}



