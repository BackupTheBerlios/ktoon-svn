/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef KTGLOBAL_H
#define KTGLOBAL_H


#if 0
* @if english
* Translate
* @endif
* @if spanish
* 
* @endif
#endif

#include "ktapplication.h"


#define ktapp static_cast<KTApplication*>(qApp)
#define KTOON_HOME (static_cast<KTApplication*>(qApp))->home()

#define KTOON_REPOSITORY (static_cast<KTApplication*>(qApp))->repository()

#define KTOON_THEME_DIR (static_cast<KTApplication*>(qApp))->themeDir()

#define KTOON_DATA_DIR (static_cast<KTApplication*>(qApp))->dataDir()

// TODO: Permitir configuracion de esto
#define KTOON_TEMP_DIR QDir::tempPath()



#endif




