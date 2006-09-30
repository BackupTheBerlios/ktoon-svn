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

#include "srequestfactory.h"

#include <ktitemrequest.h>
#include <ddebug.h>

#include <QXmlSimpleReader>
#include <QXmlInputSource>

SRequestFactory::SRequestFactory()
	: QXmlDefaultHandler()
{
}


SRequestFactory::~SRequestFactory()
{
}


bool SRequestFactory::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
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
	else if ( qname == "request" )
	{
		m_requestData.id = atts.value("id").toInt();
		m_requestData.name = atts.value("name");
		m_requestData.action = atts.value("action").toInt();
	}
	
	m_qname = qname;
	
	return true;
}

bool SRequestFactory::endElement(const QString&, const QString& , const QString& qname)
{
	if ( qname == "request" )
	{
		m_isParsing = false;
	}
	
	return true;
}

bool SRequestFactory::characters ( const QString & ch )
{
	if (m_readCharacters )
	{
		m_readCharacters = false;
		
		if ( m_qname == "data" )
		{
			m_requestData.data = ch;
		}
	}
	else
	{
		Q_UNUSED(ch);
	}
	
	return true;
}

bool SRequestFactory::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


bool SRequestFactory::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

KTProjectRequest *SRequestFactory::createRequest()
{
	KTProjectRequest *request = 0;
	
	switch ( m_requestData.id )
	{
		case KTProjectRequest::Scene:
		{
			request = new KTSceneRequest( KTProjectRequest::Action(m_requestData.action), m_requestData.scene, m_requestData.data );
		}
		break;
		case KTProjectRequest::Layer:
		{
			request = new KTLayerRequest( KTProjectRequest::Action(m_requestData.action), m_requestData.scene, m_requestData.layer, m_requestData.data );
		}
		break;
		case KTProjectRequest::Frame:
		{
			request = new KTFrameRequest( KTProjectRequest::Action(m_requestData.action), m_requestData.scene, m_requestData.layer, m_requestData.frame, m_requestData.data );
		}
		break;
		case KTProjectRequest::Item:
		{
			request = new KTItemRequest( KTProjectRequest::Action(m_requestData.action), m_requestData.scene, m_requestData.layer, m_requestData.frame, m_requestData.item, m_requestData.data );
		}
		break;
	}
	
	return request;
}


KTProjectRequest *SRequestFactory::build(const QString &document)
{
	dDebug() << "Builds from: " << document;
	
	QXmlSimpleReader m_reader;
	m_reader.setContentHandler(this);
	m_reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(document);
	
	
	if( m_reader.parse(&xmlsource) )
	{
		return createRequest();
	}
	
	return 0;
}

