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
#include <dcore/ddebug.h>

#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include "ktprojectresponse.h"

struct KTRequestParser::Private
{
	QString sign;
	KTProjectResponse *response;
};

KTRequestParser::KTRequestParser()
	: KTXmlParserBase(), d(new Private())
{
	d->response = 0;
}


KTRequestParser::~KTRequestParser()
{
	delete d;
}

void KTRequestParser::initialize()
{
	d->response = 0;
}


bool KTRequestParser::startTag(const QString& qname, const QXmlAttributes& atts)
{
	if( qname == "request" )
	{
		d->sign = atts.value("sign");
	}
	else if ( qname == "item" )
	{
		static_cast<KTItemResponse *>(d->response)->setItemIndex(atts.value("index").toInt());
	}
	else if ( qname == "frame" )
	{
		static_cast<KTFrameResponse *>(d->response)->setFrameIndex(atts.value("index").toInt());
	}
	else if ( qname == "data" )
	{
		setReadText(true);
	}
	else if ( qname == "layer" )
	{
		static_cast<KTLayerResponse *>(d->response)->setLayerIndex(atts.value("index").toInt());
	}
	else if ( qname == "scene" )
	{
		static_cast<KTSceneResponse *>(d->response)->setSceneIndex(atts.value("index").toInt());
	}
	else if ( qname == "symbol" )
	{
		static_cast<KTLibraryResponse*>(d->response)->setSymtype(atts.value("type").toInt());
	}
	else if ( qname == "action" )
	{
		d->response = KTProjectResponseFactory::create( atts.value("part").toInt(), atts.value("id").toInt());
		d->response->setArg(atts.value("arg"));
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
		d->response->setData( QByteArray::fromBase64( QByteArray(ch.toLocal8Bit()) ) );
	}
}

KTProjectResponse *KTRequestParser::response() const
{
	return d->response;
}

QString KTRequestParser::sign() const
{
	return d->sign;
}
