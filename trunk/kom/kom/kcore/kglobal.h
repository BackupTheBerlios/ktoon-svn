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


#ifndef KGLOBAL_H
#define KGLOBAL_H

#if defined(QT_SHARED) || defined(QT_PLUGIN)
# define K_GUI_EXPORT Q_GUI_EXPORT
# define K_CORE_EXPORT Q_DECL_EXPORT
# define K_SOUND_EXPORT Q_DECL_EXPORT
#else
# define K_GUI_EXPORT
# define K_CORE_EXPORT
# define K_SOUND_EXPORT
#endif

#include "kapplicationproperties.h"

#define SHARE_DIR kAppProp->shareDir()
#define DATA_DIR kAppProp->dataDir()
#define THEME_DIR kAppProp->themeDir()
#define HOME_DIR kAppProp->homeDir()
#define CONFIG_DIR kAppProp->configDir()

#define CACHE_DIR kAppProp->cacheDir()

#endif
