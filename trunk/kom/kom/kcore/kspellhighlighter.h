/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef SPELLHIGHLIGHTER_H
#define SPELLHIGHLIGHTER_H

#include <qsyntaxhighlighter.h>

#include "kcore/kspeller.h"
#include "kcore/kglobal.h"

/**
 * @if english
 * This class represents a spell highlighter, the common use is in QTextEdit documents
 * @code
 * new KSpellHighlighter(textEdit->document());
 * @endcode
 * @elseif spanish
 * Esta clase representa un destacador de ortografia, su uso mas comun es en documentos de QTextEdit
 * @code
 * new KSpellHighlighter(textEdit->document());
 * @endcode
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_CORE_EXPORT KSpellHighlighter : public QSyntaxHighlighter
{
    public:
        /**
         * @if english
         * Default constructor
         * @param parent 
         * @return 
         */
        KSpellHighlighter(QTextDocument * parent);
        /**
         * Destructor
         * @return 
         */
        ~KSpellHighlighter();
        
        /**
         * @if english
         * Sets the highlight color
         * @param color 
         */
        void setResaltColor(const QColor &color);
        
    protected:
        virtual void highlightBlock(const QString & text);
        
    private:
        QColor m_resaltColor;
        KSpeller *m_speller;
};

#endif
