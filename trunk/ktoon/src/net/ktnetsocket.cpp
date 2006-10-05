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

#include "ktnetsocket.h"
#include <QTextStream>
#include <QDataStream>

#include <ddebug.h>

#include "ktrequestfactory.h"
#include "ktprojectrequest.h"
#include "ktnetprojectmanagerhandler.h"

KTNetSocket::KTNetSocket(KTNetProjectManagerHandler *handler) : QTcpSocket(handler), m_handler(handler)
{
	connect(this, SIGNAL(readyRead ()), this, SLOT(readFromServer()) );
}


KTNetSocket::~KTNetSocket()
{
}

void KTNetSocket::sendToServer(const QString &str)
{
	if ( state() == QAbstractSocket::ConnectedState )
	{
		QTextStream stream(this);
		stream << str << "%%" << endl;
	}
}

void KTNetSocket::sendToServer(const QDomDocument &doc)
{
	sendToServer(doc.toString(0));
}

void KTNetSocket::readFromServer()
{
	D_FUNCINFO;
	
	while(canReadLine())
	{
		m_readed += readLine();

		if ( m_readed.endsWith("%%\n") )
		{
			break;
		}
	}
	if ( m_readed.isEmpty() )
	{
		return;
	}
	
	m_readed.remove(m_readed.lastIndexOf("%%"), 2);
	
	QDomDocument doc;
	
	if ( doc.setContent(m_readed) )
	{
		QString root = doc.documentElement().tagName();
		if ( root == "request" )
		{
			KTRequestFactory factory;
			KTProjectRequest *request = factory.build( m_readed );
			
			if ( request )
			{
				qDebug("EMITIENDO REQUEST!!!!!!!");
				m_handler->emitRequest( request );
				delete request;
			}
			else
			{
				qDebug("FAILS BUILDING!");
			}
		}
		else
		{
			dDebug("net") << "Unknown package: " << root;
		}
	}
	else
	{
		qDebug("Isn't a document");
	}
	
	
	dDebug("net") << "Readed from server: " << m_readed;
	
	m_readed = "";
	if( canReadLine() ) emit readyRead();
}

