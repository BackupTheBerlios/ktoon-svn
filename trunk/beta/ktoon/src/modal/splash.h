/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#ifndef SPLASH_H
#define SPLASH_H

/**
 * @file splash.h
 * @brief Include this file if you need the class Splash
 */

#include <qpainter.h>
#include <qsplashscreen.h>

/**
 * @brief Class that handles the application's splash screen
 *
 * <b>Date of Creation: July 28 - 2004.</b>\n
 * This is a widget that it is shown at the application's startup.
*/
class Splash : public QSplashScreen
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Splash object.
     */
    Splash();
    /**
     * @brief Default Destructor
     *
     * Destroys the Splash object.
     */
    ~Splash();

private:

public slots:

protected:

};

#endif
