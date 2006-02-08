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

#include "spellhighlighter.h"
#include <QtDebug>

SpellHighlighter::SpellHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent)
{
	m_resaltColor = Qt::darkMagenta;
	m_speller = new Speller;
}


SpellHighlighter::~SpellHighlighter()
{
	delete m_speller;
}

void SpellHighlighter::setResaltColor(const QColor &color)
{
	m_resaltColor = color;
}

void SpellHighlighter::highlightBlock ( const QString & text )
{
	QTextCharFormat format;
	format.setFontWeight(QFont::Bold);
	format.setForeground(m_resaltColor);
	
	QStringList words = text.split(' ');
	
	foreach(QString w, words)
	{
		if ( !m_speller->checkWord( w ) )
		{
			setFormat( text.indexOf(w), w.length(), format);
		}
	}
}


