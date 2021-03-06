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



#ifndef ASPELLIFACE_H
#define ASPELLIFACE_H

#ifdef HAVE_ASPELL

#include <QStringList>
#include <QString>

#include "kcore/kspellinterface.h"
#include "kglobal.h"

struct AspellSpeller;

/**
 * @if english
 * Interface for spell 
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KAspellChecker : public KSpellInterface
{
    public:
        /**
         * @if english
         * Translate
         * @endif
         */
        KAspellChecker();

        /**
         * @if english
         * Destructor
         * @endif
         */
        virtual ~KAspellChecker();

        /**
         * Reimplementing KSpellInterface, this function verifies if a word is well spelled
         */
        bool checkWord(const QString &word);

        /**
         * Returns a suggestions list for a word if it is bad spelled
         */
        QStringList suggestions(const QString &word);
		
    private:
        bool init();
		
    private:
        AspellSpeller *m_speller;
};

#endif

#endif // HAVE_ASPELL

