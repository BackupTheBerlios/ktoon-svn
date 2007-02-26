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

#ifndef DOMSERVERCLIENT_H
#define DOMSERVERCLIENT_H

#include "ktsocketbase.h"

namespace Server {

class Connection;

/**
 * Esta clase representa los clientes del servidor
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class Client : public KTSocketBase
{
	Q_OBJECT;
	public:
		Client(Server::Connection *connection);
		virtual ~Client();
		
		virtual void readed(const QString &readed);
		
	private:
		Server::Connection *m_connection;
};


}

#endif


