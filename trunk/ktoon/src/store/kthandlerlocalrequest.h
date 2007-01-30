/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#ifndef KTHANDLERLOCALREQUEST_H
#define KTHANDLERLOCALREQUEST_H

#include<QObject>

class KTProjectRequest;
class KTProjectResponse;
class KTProjectManager;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 * @TODO cambiar nombre a LocalRequesHandler
*/
class KTHandlerLocalRequest : public QObject
{
	Q_OBJECT
	public:
		KTHandlerLocalRequest(const KTProjectManager *projectManager,QObject *parent = 0);
		~KTHandlerLocalRequest();
		
	public slots:
		void handle(const KTProjectRequest *);
		
	private:
		const KTProjectManager *m_projectManager;
		
	signals:
		void responsed( KTProjectResponse* );
		
};

#endif
