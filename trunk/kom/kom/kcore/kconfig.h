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

#ifndef KCONFIG_H
#define KCONFIG_H

#include <QObject>
#include <QDir>
#include <QHash>
#include <QDomDocument>
#include <QVariant>

#include "kcore/kglobal.h"


class KConfig;

/**
 * @author David Cuadrado
 * this is a dom config handler
*/

class K_CORE_EXPORT KConfig : public QObject
{
    public:
        ~KConfig();

    protected:
        explicit KConfig();
        void init();

    public:
        void beginGroup(const QString & prefix);
        void endGroup();

        void setValue(const QString & key, const QVariant & value);

        QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;

        static KConfig *instance();

        bool isOk();
        QDomDocument document();

        void sync();

    private:
        QDomElement find(const QDomElement &element, const QString &key) const;

    private:
        static KConfig *m_instance;

        class Private;
        Private * const k;
};

#define KCONFIG KConfig::instance()

#endif
