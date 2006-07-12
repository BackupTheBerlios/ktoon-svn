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


#ifndef KTPROJECTEVENT_H
#define KTPROJECTEVENT_H

#include <QObject>
#include <QString>

class KTProjectEvent;
class KTSceneEvent;
class KTLayerEvent;
class KTFrameEvent;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_DECL_EXPORT KTProjectEvent : public QObject
{
	public:
		enum Action
		{
			Add,
			Remove,
			Move,
			Lock,
			Rename
		};
		
		enum Part
		{
			Project = 1000,
			Frame,
			Layer,
			Scene
		};
		
		KTProjectEvent(Action action, QObject *parent = 0);
		virtual ~KTProjectEvent();
		
		
		Action action() const;
		
		void setPartName(const QString &name);
		QString partName() const;
		virtual int id() const;
		
		virtual bool isValid() const;
		
		virtual KTProjectEvent *clone() const;
		
	private:
		Action m_action;
		QString m_partName;
};

#endif


