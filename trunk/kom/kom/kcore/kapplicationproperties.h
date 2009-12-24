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

#ifndef KAPPLICATIONPROPERTIES_H
#define KAPPLICATIONPROPERTIES_H

#include <QString>
#include "kglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KApplicationProperties
{
    protected:
        KApplicationProperties();
        virtual ~KApplicationProperties();

    public:
        void setShareDir(const QString &v);
        void setDataDir(const QString &v);
        void setHomeDir(const QString &v);
        void setThemeDir(const QString &v);
        void setCacheDir(const QString &v);
        void setVersion(const QString &v);

        virtual QString shareDir() const;
        virtual QString dataDir() const;
        virtual QString homeDir() const;
        virtual QString themeDir() const;
        virtual QString configDir() const;
        virtual QString cacheDir() const;
        virtual QString version() const;

        static KApplicationProperties *instance();
		
    private:
        static KApplicationProperties *s_instance;

    struct Private;
        Private *const k;
};

#define kAppProp KApplicationProperties::instance()

#endif
