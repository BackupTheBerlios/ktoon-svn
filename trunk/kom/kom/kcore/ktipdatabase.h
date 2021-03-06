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

#ifndef KTIPDATABASE_H
#define KTIPDATABASE_H

#include <QWidget>
#include <QString>
#include <QList>
#include "kcore/kglobal.h"

/**
 * @struct KTip
 * @author David Cuadrado <krawek@gmail.com>
 */

struct KTip
{
    QString text;
};

/**
 * @class KTipDatabase
 * 
 * This class handles the tips database about the application
 * 
 * El archivo de base de datos tiene la siguiente forma:
 * 
 * @verbatim
 * \<TipOfDay\>
 *     \<tip>Tip1.\</tip\>
 *     \<tip>Tipn\</tip\>
 * \</TipOfDay\>
 * @endverbatim
 * 
 * @author David Cuadrado \<krawek@gmail.com\>
 */

class K_CORE_EXPORT KTipDatabase : public QWidget
{
    public:
        KTipDatabase(const QString &file, QWidget *parent=0);
        ~KTipDatabase();
        KTip tip() const;
        void nextTip();
        void prevTip();
        
    private:
        void loadTips(const QString &file);
        
    private:
        struct Private;
        Private *const k;
};

#endif
