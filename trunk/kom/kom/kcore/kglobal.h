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

#define DATA_DIR kAppProp->dataDir()
#define THEME_DIR kAppProp->themeDir()
#define HOME_DIR kAppProp->homeDir()
#define CONFIG_DIR kAppProp->configDir()

#define CACHE_DIR kAppProp->cacheDir()

#endif
