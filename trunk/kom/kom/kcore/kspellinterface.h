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


#ifndef KSPELLINTERFACE_H
#define KSPELLINTERFACE_H

#include <QString>
#include <QStringList>
#include "kcore/kglobal.h"

/**
 * @if english
 * Abstract class that define a interface for spell programs, such as aspell, ispell, pspell
 * @elseif spanish
 * Clase abstracta que define una interfaz para programas de ortografia
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KSpellInterface
{
    public:
        virtual ~KSpellInterface() {}
        /**
         * @if english
         * This function checks out if a word is well spelled 
         * @endif
         * @param word 
         * @return 
         */
        virtual bool checkWord(const QString &word) = 0;
        
        /**
         * @if english
         * This function returns a list of suggested words
         * @endif
         * @param word 
         * @return 
         */
        virtual QStringList suggestions(const QString &word) = 0;
};

#endif
