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

#include "dsoap.h"

#include <QDomDocument>
#include <QRegExp>
#include <QSocketNotifier>
#include <QUrl>
#include <QStringList>
#include <QHttp>
#include <QtDebug>

#include <cstring>

DSoap::DSoap() : QObject()
{
	//m_model = canonicaltree;
	m_model = soap;
	m_socket = 0;
	m_inprogress = false;
	
	m_socket = new QTcpSocket(this);
	
	m_http = new QHttp(this);
	
	connect(m_socket, SIGNAL(readyRead()), SLOT(slotSocket()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError socketError)), SLOT(slotSocketError(QAbstractSocket::SocketError socketError)));
	
}

DSoap::~DSoap()
{
}

void DSoap::call(const QDomElement &element, const QString &endpoint)
{
	if(m_inprogress)
	{
		qWarning("Error: in progress...");
		return;
	}

	if(m_model == canonicaltree)
	{
		call_tree(element, endpoint);
	}
	else
	{
		call_soap(element, endpoint);
	}

	m_inprogress = true;
}

void DSoap::call_tree(const QDomElement &element, const QString  &endpoint)
{
	Q_UNUSED(endpoint);

	QString s = QString();

	s += localname(element);
	s += "(";
	QDomNodeList l = element.childNodes();
	for(unsigned int i = 0; i < l.count(); i++)
	{
		QDomNode tmp = l.item(i);
		s += localname(tmp);
		s += "(";
		s += xpath(tmp, "/");
		s += ")";
		s += "\n";
	}
	s += ")\n";

	qDebug() << "<CanonicalTree>" << s << endl;

	QByteArray data = s.toUtf8();
	data.truncate(data.size() - 1);

	qDebug() << "Call(socket)!" << endl;

// 	m_socket = new QTcpSocket(this);
	m_socket->connectToHost("localhost", 10000); // FIXME

	m_socket->write(data, data.size());

	m_buffer = QByteArray();
}

void DSoap::call_soap(QDomElement element, const QString &endpoint)
{
	QUrl url(endpoint);

	QDomDocument doc;
	QDomElement env = doc.createElement("SOAP-ENV:Envelope");
	env.setAttribute("xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/");
	doc.appendChild(env);
	QDomElement body = doc.createElement("SOAP-ENV:Body");
	env.appendChild(body);
	element.setAttribute("xmlns:ns", "urn:GHNS");
	body.appendChild(element);

	QString s = doc.toString();

	QByteArray data = s.toUtf8();
	data.truncate(data.size() - 1);

	qDebug() << "HTTP-POST" << url.path() << endl;
	qDebug() << "HTTP-POST" << data << endl;
	
	m_http->post(url.path(), data);

// 	KIO::Job *job;
// 	job = KIO::http_post(url, data, false);
// 	job->addMetaData("content-type", "Content-Type: text/xml");
// 
	qDebug() << "Call!" << endl;
	
	connect(m_http, SIGNAL(dataReadProgress ( int, int)), this, SLOT(slotDataRead(int, int)));
	connect(m_http, SIGNAL(done(bool)), this, SLOT(slotResult(bool)));

	m_buffer = QByteArray();
}

void DSoap::slotDataRead(int done, int total  )
{
	QByteArray data = m_http->readAll();
	
	qDebug() << "Length(before): " << m_buffer.size() << endl;
	qDebug() << "Append-Length: " << data.size() << endl;
	
	int bufferlen = m_buffer.size();
	m_buffer.resize(bufferlen + data.size());

	//m_buffer.append(data);
	// FIXME: No memcpy() in Qt??? Really, no qMemCopy()? :-)
	
	
	memcpy(m_buffer.data() + bufferlen, data.data(), data.size());

	qDebug() << "Length(after): " << m_buffer.size() << endl;
}

void DSoap::slotResult(bool error)
{
	qDebug() << "Result!" << endl;

	if(error)
	{
		qWarning("ERROR: in result");
		return;
	}

	qDebug() << "CSTRING: '" << m_buffer << "'" << endl;

	int bufferlen = m_buffer.size();
	m_buffer.resize(bufferlen + 1);
	m_buffer.data()[bufferlen] = 0;
	m_data = QString::fromUtf8(m_buffer);

	qDebug() << "STRING: '" << m_data << "'" << endl;

	if(m_model == soap)
	{
		QDomDocument doc;
		doc.setContent(m_data);

		QDomElement envelope = doc.documentElement();
		QDomNode bodynode = envelope.firstChild();
		QDomNode contentnode = bodynode.firstChild();

		qDebug() << "(signal) Result!" << endl;

		emit signalResult(contentnode);
		m_inprogress = false;
	}
	else
	{
		QDomDocument doc;

		// FIXME: dummy data
		//m_data = QString("GHNSRemovalResponse(ok(true)\nauthorative(true))");
		qDebug() << m_data << endl;

		doc = buildtree(doc, doc.documentElement(), m_data);

		QDomElement root = doc.documentElement();

		qDebug() << "(signal) Result!" << endl;
		qDebug() << doc.toString() << endl;

		emit signalResult(root);
		m_inprogress = false;
	}
}

QString DSoap::localname(const QDomNode &node)
{
	QDomElement el = node.toElement();
	QString s = el.tagName().section(":", -1);
	return s;
}

QString DSoap::xpath(const QDomNode &node,QString expr)
{
	if(m_model == canonicaltree)
	{
		//QString provider = m_soap->xpath(node, "/SOAP-ENC:Array/provider");
		expr = expr.section("/", 2);
		// FIXME: Array handling for Canonical Tree Structures?
		qDebug() << "EXPR " << expr << endl;
	}

	QDomNode n = node;
	QStringList explist = expr.split('/');
	for(QStringList::Iterator it = explist.begin(); it != explist.end(); it++)
	{
		QDomElement el = n.toElement();
		QDomNodeList l = el.elementsByTagName((*it));
		n = l.item(0);
	}
	QString s = n.toElement().text();
	return s;
}

void DSoap::setModel(Model m)
{
	m_model = m;
}

void DSoap::slotSocketError(QAbstractSocket::SocketError error)
{
	Q_UNUSED(error);

	qDebug() << "socket: error" << endl;

// 	delete m_socket;
// 	m_socket = 0;

	m_inprogress = false;
}

void DSoap::slotSocket()
{
	qDebug() << "socket: data" << endl;

	QByteArray a;
	a.resize(m_socket->bytesAvailable());
	m_socket->read(a.data(), m_socket->bytesAvailable());

	qDebug() << "DATA" << a << endl;

	// 	slotData(0, a); // FIXME

	if(m_socket->atEnd())
	{
		m_socket->close();
//		delete m_socket;
// 		m_socket = 0;
		m_socket->disconnectFromHost ();
		// 		slotResult(0); // FIXME
	}
}

QDomDocument DSoap::buildtree( QDomDocument doc, QDomElement cur, const QString &data)
{
	qDebug() << "MATCH " << data << endl;

	//QRegExp e("(\\S+)\\(((?:.*\\n?)+)\\)");
	//QRegExp e("^(\\S+)\\((.*)\\)$"); // FIXME: Qt regexp bug?
	//QRegExp e("^([^(]+)\\((.*)\\)$");
	//QRegExp e("^([^(]+)(?:\\((.*)\\))?\n*$");
	//QRegExp e("^([^\(]+(?!\()[^\\\(])(?:\\((.*)[^\\]\\))?\n*$");
	//QRegExp e("^([^\\(]+[^\\\\\\(])(?:\\((.*[^\\\\])\\))?\n*$");
	//QRegExp e("^((?:\\\\\\(|[^\\(])+)(?:\\(((?:\\\\\\)|\\\\\\(|.)*)\\))?\n*$");
	QRegExp e("^((?:\\\\\\(|[^\\(])+)(?:\\(((?:\\\\\\)|.)*)\\))?\n*$");
	e.indexIn(data); // FIXME: correct method?
	//qDebug() << "Captures: " << e.numCaptures() << endl;
	qDebug() << "Captures: " << e.numCaptures() << endl;
	//for(int i = 0; i < e.numCaptures(); i++)
	//{
	//	qDebug() << "Cap(" << i + 1 << "): " << e.cap(i + 1) << endl;
	//}

	// FIXME: another Qt regexp bug?
	//if(e.numCaptures() == 2)
//qDebug() << "cap1:" << e.cap(1) << endl;
	if(data.contains(QRegExp("[^\\\\]\\(")))
	{
//qDebug() << "contains-parentheses" << endl;
		QDomElement elem;
		if(cur.isNull())
		{
			elem = doc.createElement("ns:" + e.cap(1));
			doc.appendChild(elem);
		}
		else
		{
			elem = doc.createElement(e.cap(1));
			cur.appendChild(elem);
		}

		QStringList l = e.cap(2).split('\n');
		for(QStringList::iterator it = l.begin(); it != l.end(); it++)
		{
			qDebug() << "<rec>" << (*it) << endl;
			buildtree(doc, elem, (*it));
		}
	}
	else //if(e.numCaptures() == 1)
	{
		QString text = e.cap(1);
		//text = text.replace(QRegExp("\\\\(?:[^\\\\]|$)"), "");
		text = text.replace(QRegExp("\\\\\\("), "(");
		text = text.replace(QRegExp("\\\\\\)"), ")");
		QDomText t = doc.createTextNode(text);
		cur.appendChild(t);
		//cur.setNodeValue(e.cap(1));
	}

	return doc;
}