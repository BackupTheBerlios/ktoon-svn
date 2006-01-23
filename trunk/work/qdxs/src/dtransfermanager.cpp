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

#include "dtransfermanager.h"
#include <QHttp>
#include <QFtp>
#include <QUrl>

#include <QtDebug>

DTransferManager::DTransferManager() : QObject(), m_provider(0)
{
}


DTransferManager::~DTransferManager()
{
	if ( m_provider ) delete m_provider;
}

void DTransferManager::get(const QUrl &url )
{
	if ( m_provider )
	{
		qDebug("Deleting old provider");
		delete m_provider;
		m_provider = 0;
	}
	
	if ( url.scheme() == "http" )
	{
		m_protocol = Http;
		m_provider = new QHttp(this);
		
		provider<QHttp *>()->setHost(url.host());
		provider<QHttp *>()->get(url.toString());
		
		connect(m_provider, SIGNAL(requestFinished(int, bool)), this, SLOT(emitFinished(int, bool)));
		
	}
	else if ( url.scheme() == "ftp" )
	{
		m_protocol = Ftp;
		
		m_provider = new QFtp(this);
		
		provider<QFtp *>()->connectToHost( url.host(), url.port(21) );
		QString login = url.userName();
		QString password = url.password();
		
		if ( login.isEmpty() )
		{
			login = "ftp"; // Or anonymous ?
		}
		if ( password.isEmpty() )
		{
			password = "user@domain.com";
		}
		
		provider<QFtp *>()->login(login, password);
		provider<QFtp *>()->get(url.toString());
		
		connect(m_provider, SIGNAL(commandFinished(int, bool)), this, SLOT(emitFinished(int, bool)));
	}
	else
	{
		qDebug() << "DTransferManager::get: Protocol not supported URL=" << url;
	}
	
}

void DTransferManager::get(const QString &url)
{
	get(QUrl(url));
}

void DTransferManager::emitFinished(int id, bool err)
{
	if ( err )
	{
		switch(m_protocol)
		{
			case Http:
			{
				emit error( provider<QHttp *>()->errorString() );
			}
			break;
			case Ftp:
			{
				emit error(provider<QFtp *>()->errorString() );
			}
			break;
		}
	}
	else
	{
		QByteArray data;
		switch(m_protocol)
		{
			case Http:
			{
				data = provider<QHttp *>()->readAll();
				emit readed( data );
			}
			break;
			case Ftp:
			{
				data = provider<QFtp *>()->readAll();
				emit readed( data );
			}
			break;
		}
	}
}



