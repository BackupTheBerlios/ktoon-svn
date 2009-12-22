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

