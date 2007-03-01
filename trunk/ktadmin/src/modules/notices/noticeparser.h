/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#ifndef NOTICESNOTICEPARSER_H
#define NOTICESNOTICEPARSER_H

#include <ktxmlparserbase.h>

namespace Notices
{

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class NoticeParser : public KTXmlParserBase
{
	public:
		NoticeParser();
		~NoticeParser();
		
		bool startTag(const QString &tag, const QXmlAttributes &atts);
		bool endTag(const QString &tag);
		void text(const QString &msg);
		
		QString message() const;
		QString from() const;
		
	private:
		struct Private;
		Private * const d;
};

}

#endif
