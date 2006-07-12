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
#ifndef KTLAYEREVENT_H
#define KTLAYEREVENT_H

#include <ktsceneevent.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_DECL_EXPORT KTLayerEvent : public KTSceneEvent
{
	public:
		KTLayerEvent(Action action, int sceneIndex, int layerIndex, QObject *parent = 0);
		~KTLayerEvent();
		
		virtual int id() const;
		int layerIndex() const;
		virtual bool isValid() const;
		virtual KTProjectEvent *clone() const;
		
	private:
		int m_layerIndex;
};


/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTMoveLayerEvent : public KTLayerEvent
{
	public:
		KTMoveLayerEvent(int sceneIndex, int layerIndex, int newIndex, QObject *parent = 0);
		~KTMoveLayerEvent();
		
		int newLayerIndex() const;
		
		virtual bool isValid() const;
		virtual KTProjectEvent *clone() const;
		
	private:
		int m_newLayerIndex;
};



/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTLockLayerEvent: public KTLayerEvent
{
	public:
		KTLockLayerEvent(int sceneIndex, int layerIndex, bool lock, QObject *parent = 0 );
		~KTLockLayerEvent();
		
		bool isLocked() const;
		virtual KTProjectEvent *clone() const;
		
	private:
		bool m_isLocked;
};



/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTRenameLayerEvent : public KTLayerEvent
{
	public:
		KTRenameLayerEvent(int sceneIndex, int layerIndex, const QString &newName, QObject *parent = 0);
		~KTRenameLayerEvent();
		
		QString newName() const;
		
		virtual KTProjectEvent *clone() const;
		
	private:
		QString m_newName;
};


#endif
