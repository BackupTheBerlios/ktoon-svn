/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef DSOAP_H
#define DSOAP_H

#include <qobject.h>
#include <QTcpSocket>

class QDomDocument;
class QDomNode;
class QDomElement;
class QHttp;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class DSoap : public QObject
{
	public:
		DSoap();
		~DSoap();
		
		enum Model
		{
			soap,
			canonicaltree
		};

		void setModel(Model m);

		void call(QDomElement element, QString endpoint);
		QString localname(QDomNode node);
		QString xpath(QDomNode node, QString expr);

	signals:
		void signalResult(QDomNode node);

	private slots:
		void slotDataRead(int step, int total);
		void slotResult(bool error);
		void slotSocket();
		void slotSocketError(QAbstractSocket::SocketError error);

	private:
		void call_soap(QDomElement element, QString endpoint);
		void call_tree(QDomElement element, QString endpoint);
		QDomDocument buildtree(QDomDocument doc, QDomElement cur, QString data);

		QString m_data;
		Model m_model;
		QTcpSocket *m_socket;
		QByteArray m_buffer;
		bool m_inprogress;
		
		QHttp *m_http;
};

#endif
