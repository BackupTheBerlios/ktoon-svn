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

#ifndef KTPROJECTMANAGER_H
#define KTPROJECTMANAGER_H

#include <QObject>
#include "ktdocument.h"

typedef QList<KTDocument *> Documents;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTProjectManager : public QObject
{
	Q_OBJECT
	public:
		KTProjectManager(QObject *parent = 0);
		~KTProjectManager();
		
		Documents documents() const;
		void setDocuments(const Documents &);
		
		KTDocument *createDocument(const QString &name);
		
		KTDocument *currentDocument();
		KTScene *currentScene();
		KTLayer *currentLayer();
		KTKeyFrame *currentKeyFrame();
		
		void setCurrentDocument(int index);
		
	public slots:
		//Scenes
		void addScene();
		void removeScene();
		void renameScene(const QString &name, int index);
		
		// Layer
		void addLayer();
		
		// Frames
		void addFrame();
		void setCurrentFrame(int index);
		
	signals:
		// Scenes
		void sceneAdded(const QString &name);
		void sceneRenamed(const QString &name, int id);
		
		// Layers
		void layerAdded(const QString &name);
		
		// Frames
		void frameAdded(const QString &name);

		
		// <FIXME>
		void documentInserted();
		void LayerInserted();
		// </FIXME>
	private:
		Documents m_documents;
		
		KTDocument *m_currentDocument;
};

#endif
