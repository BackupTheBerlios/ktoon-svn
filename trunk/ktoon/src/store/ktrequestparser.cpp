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
	: KTXmlParserBase()
{
	m_response = 0;
}


KTRequestParser::~KTRequestParser()
{
}

void KTRequestParser::initialize()
{
	m_response = 0;
}


bool KTRequestParser::startTag(const QString& qname, const QXmlAttributes& atts)
{
	if ( qname == "item" )
	{
		static_cast<KTItemResponse *>(m_response)->setItemIndex(atts.value("index").toInt());
	}
	else if ( qname == "frame" )
	{
		static_cast<KTFrameResponse *>(m_response)->setFrameIndex(atts.value("index").toInt());
	}
	else if ( qname == "data" )
	{
		setReadText(true);
	}
	else if ( qname == "layer" )
	{
		static_cast<KTLayerResponse *>(m_response)->setLayerIndex(atts.value("index").toInt());
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
	
	return true;
}

bool KTRequestParser::endTag(const QString& qname)
{
	return true;
}

void KTRequestParser::text( const QString & ch )
{
	if ( currentTag() == "data" )
	{
		m_response->setData( QByteArray::fromBase64( QByteArray(ch.toLocal8Bit()) ) );
	}
}

KTProjectResponse *KTRequestParser::response() const
{
	return m_response;
}


