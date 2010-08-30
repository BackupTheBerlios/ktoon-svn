/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
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
