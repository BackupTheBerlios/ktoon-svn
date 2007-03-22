/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "daspellchecker.h"

#ifdef HAVE_ASPELL
#include <aspell.h>

#include <QLocale>
#include <QString>
#include <QtDebug>

DAspellChecker::DAspellChecker() : m_speller(0)
{
	init();
}

bool DAspellChecker::init()
{
	QString locale = QString(QLocale::system().name()).left(2);
	if ( locale.length() < 2 )
	{
		locale = "en";
	}
	
	AspellConfig * config = new_aspell_config();
	aspell_config_replace(config, "lang", locale.toLocal8Bit().data());
	
	AspellCanHaveError * ret = new_aspell_speller(config);;
	delete_aspell_config(config);
	
	if (aspell_error(ret) != 0) 
	{
		qDebug("Error: %s\n",aspell_error_message(ret));
		delete_aspell_can_have_error(ret);
		
		return false;
	}
	
	m_speller = to_aspell_speller(ret);
	config = aspell_speller_config(m_speller);
	
	qDebug() << "USING LANG= " << aspell_config_retrieve(config, "lang");
	
	return true;
}

DAspellChecker::~DAspellChecker()
{
	if ( m_speller ) delete_aspell_speller(m_speller);
}

bool DAspellChecker::checkWord(const QString &word)
{
	if ( !m_speller ) return true;
	
	return aspell_speller_check(m_speller, word.toLocal8Bit().data(), -1);
}

QStringList DAspellChecker::suggestions(const QString &word)
{
	if ( !m_speller ) return QStringList();
	
	QStringList suggs;
	
	const AspellWordList *wordList = aspell_speller_suggest(m_speller, word.toLocal8Bit().data(), -1);
	
	if ( wordList == 0 )
	{
		return suggs;
	}
	
	AspellStringEnumeration *els = aspell_word_list_elements(wordList);
	const char *ws;
	while ( (ws = aspell_string_enumeration_next(els)) != 0) 
	{
		suggs << QString::fromLatin1(ws);
	}
	return suggs;
}

#endif // HAVE_ASPELL
