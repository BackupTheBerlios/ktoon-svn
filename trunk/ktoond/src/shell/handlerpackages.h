/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#ifndef HANDLERPACKGES_H
#define HANDLERPACKGES_H

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

#include <QString>
#include "abstracthandlerpackages.h"
#include "ktserverconnection.h"

class ProjectCollection;

class HandlerPackages : public AbstractHandlerPackages
{
	public:
		HandlerPackages(KTServer *parent = 0 );
		~HandlerPackages();
		
		void handle(const KTServerConnection *, const QString & );
		
	private:
		void handleProjectRequest(const KTServerConnection *cnn, const QString &request);
		
	private:
		ProjectCollection *m_projects;
};

#endif
