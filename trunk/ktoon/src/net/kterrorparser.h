//
// C++ Interface: kterrorparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTERRORPARSER_H
#define KTERRORPARSER_H

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
#include <ktxmlparserbase.h>
class KTErrorParser : public KTXmlParserBase
{
	public:
		
		struct Error
		{
			int level;
			QString message;
		};
		KTErrorParser();
		~KTErrorParser();
		
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
		KTErrorParser::Error error();
	
	private:
		KTErrorParser::Error m_error;
		
		
};

#endif
