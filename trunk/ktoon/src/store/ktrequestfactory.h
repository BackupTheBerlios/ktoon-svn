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

#ifndef KTREQUESTFACTORY_H
#define KTREQUESTFACTORY_H

#include <QXmlDefaultHandler>
#include <QString>
#include <QVariant>

class KTProjectRequest;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTRequestFactory : public QXmlDefaultHandler
{
	public:
		KTRequestFactory();
		~KTRequestFactory();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool characters ( const QString & ch );
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		KTProjectRequest *build(const QString &doc);
		
	private:
		KTProjectRequest *createRequest();
		
	private:
		QString m_qname;
		
		bool m_isParsing;
		bool m_readCharacters;
		
		struct RequestData
		{
			QVariant data;
			QString name;
			int id;
			int action;
			int scene;
			int layer;
			int frame;
			int item;
		} m_requestData;
};

#endif
