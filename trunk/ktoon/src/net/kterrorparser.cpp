//
// C++ Implementation: kterrorparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kterrorparser.h"

KTErrorParser::KTErrorParser(): KTXmlParserBase()
{
}


KTErrorParser::~KTErrorParser()
{
}


bool KTErrorParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "error")
	{
		if(tag == "message")
		{
			m_error.level = atts.value("level").toInt();
			setReadText(true);
		}
	}
	return true;
}

bool KTErrorParser::endTag(const QString &tag)
{
	return true;
}

void KTErrorParser::text(const QString &text)
{
	if(currentTag() == "message")
	{
		m_error.message = text;
	}
}

KTErrorParser::Error KTErrorParser::error()
{
	return m_error;
}

