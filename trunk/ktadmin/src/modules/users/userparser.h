//
// C++ Interface: userparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PARSERSUSERPARSER_H
#define PARSERSUSERPARSER_H

#include <ktxmlparserbase.h>
#include "permission.h"


namespace Parsers {

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class UserParser : public KTXmlParserBase
{
	public:
		UserParser();
		~UserParser();
		
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
		QString login();
		QString name();
		QList<Users::Permission> permissions();
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
