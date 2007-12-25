/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTAPPLICATION_H
#define KTAPPLICATION_H

#include <kgui/kapplication.h>

/**
 * Support Class for main.cpp
 * This class contains some of the basic methods required when KToon is launched
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTApplication : public KApplication
{
	Q_OBJECT;
	public:
		KTApplication(int &argc, char **argv);
		~KTApplication();
		
	public slots:
		/**
		 * @if english
		 * Opens a settings wizard the first time KToon is launched
		 * @endif
		 * @if spanish
		 * Lanza un wizard de configuracion la primera vez que se inicia la aplicacion
		 * @endif
		 * @return true/false if the application has the settings file created
		 */
		virtual bool firstRun();

		/**
		 * @if english
		 * Creates a cache directory with the path defined at cacheDir variable
		 * @endif
		 * @if spanish
		 * Crea el cache en la ruta especificada por la variable cacheDir
		 * @endif
		 * @param cacheDir the path for the repository dir
		 */
		void createCache(const QString &cacheDir);
};

#endif
