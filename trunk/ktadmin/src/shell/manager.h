/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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
#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

namespace Base {
	class  PackageObserver;
}

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Manager : public QObject
{
	Q_OBJECT;
	public:
		Manager(QObject *parent = 0);
		~Manager();
		
		void handlePackage(const QString &root, const QString &xml);
		
		void addObserver(Base::PackageObserver *obs);
		void removeObserver(Base::PackageObserver *obs);
		
		bool connectToServer(const QString &server, int port);
		void authenticate(const QString &login, const QString &password);
		
		bool isEnabled() const;
		
	public slots:
		void enable();
		void disable();
		void sendPackage(const QString &pkg);
		
	signals:
		void connected();
		void disconnected();
		
	protected:
		bool tryToHandle(const QString &root, const QString &xml);
		
	private:
		struct Private;
		Private *const d;
};

#endif
