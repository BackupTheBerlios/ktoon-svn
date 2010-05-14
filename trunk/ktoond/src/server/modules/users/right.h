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

#ifndef USERSRIGHT_H
#define USERSRIGHT_H

#include <QString>

namespace Users {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Right
{
    public:
        explicit Right();
        Right(const QString &module, bool read, bool write);
        ~Right();
        
        void setRead(bool enable);
        void setWrite(bool enable);
        
        QString module() const;
        bool write() const;
        bool read() const;
        
        Q_DISABLE_COPY(Right);
        
    private:
        struct Private;
        Private *const k;
};

}

#endif
