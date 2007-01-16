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

#ifndef KTSOCKETBASE_H
#define KTSOCKETBASE_H

#include <QTcpSocket>
#include <QDomDocument>

#include "ktglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTOON_EXPORT KTSocketBase : public QTcpSocket
{
	Q_OBJECT;
	public:
		KTSocketBase(QObject *parent = 0);
		~KTSocketBase();
		
		void send(const QString &str);
		void send(const QDomDocument &doc);
		
		virtual void readed(const QString &readed) = 0;
		
	protected slots:
		virtual void readFromServer();
};

#endif
