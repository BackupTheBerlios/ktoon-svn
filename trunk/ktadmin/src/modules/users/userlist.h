//
// C++ Interface: usersinfoparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PARSERSUSERSINFOPARSER_H
#define PARSERSUSERSINFOPARSER_H

#include <ktxmlparserbase.h>

namespace Parsers {

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class UsersInfoParser : public KTXmlParserBase
{
	public:
		UsersInfoParser();
		~UsersInfoParser();
		
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
		QList<QStringList> info();
		
	private:
		struct Private;
		Private *const d;
		
};

}

#endif
