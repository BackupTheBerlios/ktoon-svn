/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#include "ktxmlparser.h"
#include <iostream>

KTXmlParser::KTXmlParser()
	: QXmlDefaultHandler(), fileNumber(0)
{
	qDebug("[Initializing KTXmlParser]");
}


KTXmlParser::~KTXmlParser()
{
}


bool KTXmlParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
		//std::cout << "Rooting to " << qname << std::endl;
	}
	else if ( m_root == "KTConfig" ) // Configuration document
	{
		if ( qname == "KTHome" )
		{
			m_results.insert(qname, atts.value("path"));
		}
		else if ( qname == "Repository")
		{
			m_results.insert(qname, atts.value("path"));
		}
		else if (qname == "file" )
		{
			fileNumber++;
			m_results.insert(qname+QString::number(fileNumber), atts.value("name"));
			m_tempList << atts.value("name");
		}
		else if ( qname == "KTTheme")
		{
			m_results.insert(qname, atts.value("path") );
		}
	}
	return true;
}

bool KTXmlParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "KTConfig" ) // Configuration document
	{
		if ( qname == "Recent" )
		{
			m_resultsList.insert(qname,m_tempList);
			m_tempList.clear();
		}
	}
	
	return true;
}

bool KTXmlParser::characters(const QString &)
{
	return true;
}

XMLSingleResult KTXmlParser::getResult()
{
	return m_results;
}

XMLTotalResults KTXmlParser::getResults()
{
	return m_resultsList;
}


