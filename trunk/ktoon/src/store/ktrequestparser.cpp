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

#include "ktrequestparser.h"
#include <ddebug.h>

#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include "ktprojectresponse.h"

KTRequestParser::KTRequestParser()
	: QXmlDefaultHandler()
{
	m_response = 0;
}


KTRequestParser::~KTRequestParser()
{
}


bool KTRequestParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (!m_isParsing)
	{
		m_isParsing = true;
		m_response = 0;
	}
	
	if ( qname == "item" )
	{
		static_cast<KTItemResponse *>(m_response)->setSceneIndex(atts.value("index").toInt());
	}
	else if ( qname == "frame" )
	{
		static_cast<KTFrameResponse *>(m_response)->setSceneIndex(atts.value("index").toInt());
	}
	else if ( qname == "data" )
	{
		m_readCharacters = true;
	}
	else if ( qname == "layer" )
	{
		static_cast<KTLayerResponse *>(m_response)->setSceneIndex(atts.value("index").toInt());
	}
	else if ( qname == "scene" )
	{
		static_cast<KTSceneResponse *>(m_response)->setSceneIndex(atts.value("index").toInt());
	}
	else if ( qname == "action" )
	{
		m_response = KTProjectResponseFactory::create( atts.value("part").toInt(), atts.value("id").toInt());
		m_response->setArg(atts.value("arg"));
	}
	
	m_qname = qname;
	
	return true;
}

bool KTRequestParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( qname == "request" )
	{
		m_isParsing = false;
	}
	
	return true;
}

bool KTRequestParser::characters ( const QString & ch )
{
	if (m_readCharacters )
	{
		m_readCharacters = false;
		
		if ( m_qname == "data" )
		{
			m_response->setData( QByteArray::fromBase64( QByteArray(ch.toLocal8Bit()) ) );
		}
	}
	else
	{
		Q_UNUSED(ch);
	}
	
	return true;
}

bool KTRequestParser::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


bool KTRequestParser::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

KTProjectResponse *KTRequestParser::response() const
{
	return m_response;
}


bool KTRequestParser::parse(const QString &document)
{
	QXmlSimpleReader m_reader;
	m_reader.setContentHandler(this);
	m_reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(document);
	
	
	return m_reader.parse(&xmlsource);
}


