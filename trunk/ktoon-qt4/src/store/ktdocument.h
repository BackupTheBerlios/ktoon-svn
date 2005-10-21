/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                          	   *
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

#ifndef KTDOCUMENT_H
#define KTDOCUMENT_H

#include <QObject>
#include "ktscene.h"

typedef QList<KTScene *> Scenes;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTDocument : public QObject
{
	Q_OBJECT
	public:
		KTDocument(QObject *parent = 0);
		~KTDocument();
		Scenes scenes() const;
		void setScenes(const Scenes &);
		KTScene *createScene();
		
		KTScene *currentScene();
		void setCurrentScene(int index);
		
	signals:
		void sceneCreated(const QString &name);
		
	private:
		Scenes m_scenes;
		KTScene *m_currentScene;
		
		int m_sceneCount;
};

#endif
