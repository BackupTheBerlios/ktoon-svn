/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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


#ifndef DGLOBAL_H
#define DGLOBAL_H

#if defined(QT_SHARED) || defined(QT_PLUGIN)
# define D_GUI_EXPORT Q_GUI_EXPORT
# define D_CORE_EXPORT Q_DECL_EXPORT
# define D_SOUND_EXPORT Q_DECL_EXPORT
#else
# define D_GUI_EXPORT
# define D_CORE_EXPORT
# define D_SOUND_EXPORT
#endif


#include "dapplicationproperties.h"

#define DATA_DIR dAppProp->dataDir()
#define THEME_DIR dAppProp->themeDir()
#define HOME_DIR dAppProp->homeDir()
#define CONFIG_DIR dAppProp->configDir()

#define CACHE_DIR dAppProp->cacheDir()


#endif



