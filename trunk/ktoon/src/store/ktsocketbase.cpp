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

#include "ktsocketbase.h"

#include <QTextStream>

KTSocketBase::KTSocketBase(QObject *parent)
 : QTcpSocket(parent)
{
	connect(this, SIGNAL(readyRead ()), this, SLOT(readFromServer()) );
}


KTSocketBase::~KTSocketBase()
{
}

void KTSocketBase::send(const QString &str)
{
	if ( state() == QAbstractSocket::ConnectedState )
	{
		QTextStream stream(this);
		stream << str.toLocal8Bit().toBase64() << "%%" << endl;
	}
}

void KTSocketBase::send(const QDomDocument &doc)
{
	send(doc.toString(0));
}

void KTSocketBase::readFromServer()
{
	QString readed;
	while(this->canReadLine())
	{
		readed += this->readLine();
		if ( readed.endsWith("%%\n") )
		{
			break;
		}
	}
	
	if ( !readed.isEmpty() )
	{
		readed.remove(readed.lastIndexOf("%%"), 2);
		readed = QString::fromLocal8Bit( QByteArray::fromBase64(readed.toLocal8Bit()) );
		
		this->readed(readed);
	}
	
	if (this->canReadLine()) readFromServer();
}

