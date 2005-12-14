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

#ifndef KTSCENE_H
#define KTSCENE_H

#include <QObject>
#include "ktlayer.h"

typedef QList<KTLayer *> Layers;

#include "ktserializableobject.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTScene : public KTSerializableObject
{
	Q_OBJECT
	public:
		KTScene(QObject *parent = 0);
		~KTScene();
		
		void setSceneName(const QString &name);
		QString sceneName();
		Layers layers() const;
		void setLayers(const Layers &);
		KTLayer *createLayer(bool addToEnd );
		
		KTLayer *currentLayer();
		void setCurrentLayer(int index);
		int indexCurrentLayer();
		void removeLayer(int index);
				
		void setFPS(int fps);
		int fps();
		
	signals:
		void layerCreated(const QString &name, bool toEnd);
		void layerRemoved(int index);
				
	private:
		Layers m_layers;
		KTLayer *m_currentLayer;
		QString m_name;
		
		int m_layerCount;
		
		int m_fps;
};

#endif
