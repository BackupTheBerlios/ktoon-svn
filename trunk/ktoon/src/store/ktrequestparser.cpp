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

KTRequestParser::KTRequestParser()
	: QXmlDefaultHandler()
{
	m_requestData.arg = 0;
}


KTRequestParser::~KTRequestParser()
{
	delete m_requestData.arg;
}


bool KTRequestParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (!m_isParsing)
	{
		m_isParsing = true;
	}
	
	if ( qname == "item" )
	{
		m_requestData.item = atts.value("index").toInt();
	}
	else if ( qname == "frame" )
	{
		m_requestData.frame = atts.value("index").toInt();
	}
	else if ( qname == "data" )
	{
		m_readCharacters = true;
	}
	else if ( qname == "layer" )
	{
		m_requestData.layer = atts.value("index").toInt();
	}
	else if ( qname == "scene" )
	{
		m_requestData.scene = atts.value("index").toInt();
	}
	else if ( qname == "action" )
	{
		m_requestData.action = atts.value("id").toInt();
		m_requestData.arg = new KTProjectRequestArgument(atts.value("arg"));
		m_requestData.part = atts.value("part").toInt();
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
			m_requestData.data = QByteArray::fromBase64( QByteArray(ch.toLocal8Bit()) );
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

KTProjectRequestArgument KTRequestParser::arg() const
{
	return *m_requestData.arg;
}

int KTRequestParser::action() const
{
	return m_requestData.action;
}

int KTRequestParser::part() const
{
	return m_requestData.part;
}


int KTRequestParser::sceneIndex() const
{
	return m_requestData.scene;
}

int KTRequestParser::layerIndex() const
{
	return m_requestData.layer;
}

int KTRequestParser::frameIndex() const
{
	return m_requestData.frame;
}

int KTRequestParser::itemIndex() const
{
	return m_requestData.item;
}

QByteArray KTRequestParser::data() const
{
	return m_requestData.data;
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


