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

#ifndef KTSPLASH_H
#define KTSPLASH_H

/**
 * @author Fernando Roldan - David Cuadrado
 * @file splash.h
 * @brief Include this file if you need the class KTSplash
 */

#include <qpainter.h>
#include <qsplashscreen.h>

#include <QTimer>

/**
 * @brief Class that handles the application's splash screen
 *
 * <b>Date of Creation: July 28 - 2004.</b>\n
 * This is a widget that it is shown at the application's startup.
*/
class KTSplash : public QSplashScreen
{
    Q_OBJECT
    public:
        /**
         * @brief Default Constructor
         *
         * Constructs a KTSplash object.
         */
         KTSplash();
         /**
          * @brief Default Destructor
          *
          * Destroys the KTSplash object.
          */
         ~KTSplash();

         void setMessage(const QString &msg);

    private slots:
         void animate();

    private:
         QString m_message, m_version;
         int m_size;
         int m_state;
         int m_counter;

         QTimer *m_timer;

    protected:
         void drawContents(QPainter * painter);
};

#endif
