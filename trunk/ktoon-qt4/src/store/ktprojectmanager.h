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
#include "ktserializableobject.h"
#include "ktdocument.h"

typedef QList<KTDocument *> Documents;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTProjectManager : public KTSerializableObject
{
	Q_OBJECT
	public:
		KTProjectManager(QObject *parent = 0);
		~KTProjectManager();
		QDomElement createXML( QDomDocument &doc );
		void setProjectName(const QString &name);
		void setProjectRender( const QString typeRender);
		void setProjectFPS( int fps );
		void setDocumentSize(const QSize& size );
		QSize documentSize() const;
		QString projectRender( ) const;
		int fps();
		
		bool open();
		
	public slots:
		void save();
		void close();
		void load(const QString &path);
		
		// Documents
		void setCurrentDocument(int index);
		KTDocument *currentDocument() const;
		KTDocument *createDocument(const QString &name = QString());
		
		Documents documents() const;
		void setDocuments(const Documents &);
		
		//Scenes
		void createScene(bool addToEnd = true);
		void removeScene();
		void renameScene(const QString &name, int index);
		void setCurrentScene(int index);
		KTScene *currentScene();
		
		
		// Layer
		void createLayer(bool addToEnd = true);
		void setCurrentLayer(int index);
		void setLayerVisibility(int idLayer, bool value);
		KTLayer *currentLayer();
		void removeLayer(int index);
				
		// Frames
		void createFrame(bool addToEnd = true);
		KTKeyFrame *currentKeyFrame();
		void setCurrentFrame(int index);
		void copyFrame(int index);
		void pasteFrame(int index);
		void moveFrame(bool up);
		void removeFrame();
		void lockCurrentFrame();
		
	private slots:
		// Layers
		void emitLayerVisibility(bool);
		
	signals:
		// Documents
		void documentCreated(const QString &name);
		
		// Scenes
		void sceneCreated(const QString &name, bool addedToEnd);
		void sceneRenamed(const QString &name, int id);
		
		
		// Layers
		void layerCreated(const QString &name, bool addedToEnd);
		void layerVisibilityChanged(int index, bool );
		void layerRemoved(int );
		void layerSelected(int );
		
		// Frames
		void frameCreated( const QString &name, bool addedToEnd);
		void frameMoved(bool up);
		void frameRemoved();
		void frameLocked();
		
	private:
		Documents m_documents;
		KTDocument *m_currentDocument;
		QSize m_size;
		const KTKeyFrame *m_copyFrame;
		mutable QString m_name;
		bool m_open;
		
		QString m_typeRender;
		int m_fps;
		
};

#endif
