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

#ifndef KTSCENEEVENT_H
#define KTSCENEEVENT_H

#include <ktprojectevent.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_DECL_EXPORT KTSceneEvent : public KTProjectEvent
{
	public:
		KTSceneEvent(Action action, int sceneIndex, QObject *parent = 0);
		~KTSceneEvent();
		
		virtual int id() const;
		int sceneIndex() const;
		
		virtual bool isValid() const;
		
		virtual KTProjectEvent *clone() const;
		
	private:
		int m_sceneIndex;
};

/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTMoveSceneEvent : public KTSceneEvent
{
	public:
		KTMoveSceneEvent(int sceneIndex, int newIndex, QObject *parent = 0);
		~KTMoveSceneEvent();
		
		int newSceneIndex() const;
		
		virtual bool isValid() const;
		virtual KTProjectEvent *clone() const;
		
	private:
		int m_newSceneIndex;
};



/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTLockSceneEvent: public KTSceneEvent
{
	public:
		KTLockSceneEvent(int sceneIndex, bool lock, QObject *parent = 0 );
		~KTLockSceneEvent();
		
		bool isLocked() const;
		virtual KTProjectEvent *clone() const;
		
	private:
		bool m_isLocked;
};



/**
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Q_DECL_EXPORT KTRenameSceneEvent : public KTSceneEvent
{
	public:
		KTRenameSceneEvent(int sceneIndex, const QString &newName, QObject *parent = 0);
		~KTRenameSceneEvent();
		
		QString newName() const;
		
		virtual KTProjectEvent *clone() const;
		
	private:
		QString m_newName;
};


#endif


