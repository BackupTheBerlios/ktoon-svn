//
// C++ Interface: useractionparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PARSERSUSERACTIONPARSER_H
#define PARSERSUSERACTIONPARSER_H

#include <ktxmlparserbase.h>

namespace Users {
	class User;
}

namespace Parsers {

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class UserActionParser : public KTXmlParserBase
{
	public:
		UserActionParser();
		~UserActionParser();
		
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
		Users::User user();
		
	private:
		struct Private;
		Private * const d;

};

}

#endif
