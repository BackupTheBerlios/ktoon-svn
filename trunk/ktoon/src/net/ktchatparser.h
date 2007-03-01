/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#ifndef KTCHATPARSER_H
#define KTCHATPARSER_H

#include <ktxmlparserbase.h>

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTChatParser : public KTXmlParserBase
{
	public:
		KTChatParser();
		virtual ~KTChatParser();
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
	public:
		QString message() const;
		QString login() const;
		
	private:
		struct Private;
		Private *const d;

};

#endif