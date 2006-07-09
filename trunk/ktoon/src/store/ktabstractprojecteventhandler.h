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

#ifndef KTABSTRACTPROJECTEVENTHANDLER_H
#define KTABSTRACTPROJECTEVENTHANDLER_H

#include <QObject>

class KTProjectEvent;
class KTFrameEvent;
class KTLayerEvent;
class KTSceneEvent;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class Q_DECL_EXPORT KTAbstractProjectEventHandler
{
	public:
		KTAbstractProjectEventHandler();
		virtual ~KTAbstractProjectEventHandler();
		
		virtual bool handleEvent(KTProjectEvent *event);
		
	protected:
		virtual void frameEvent(KTFrameEvent *frameEvent) = 0;
		virtual void layerEvent(KTLayerEvent *layerEvent) = 0;
		virtual void sceneEvent(KTSceneEvent *sceneEvent) = 0;
		virtual void projectEvent(KTProjectEvent *projectEvent) = 0;
};

#endif
