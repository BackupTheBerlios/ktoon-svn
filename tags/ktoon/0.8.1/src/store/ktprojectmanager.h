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
#include "ktglobal.h"

typedef QList<KTDocument *> Documents;

/**
 * Esta clase maneja el proyecto, contiene KTDocument 's
 * 
 * @brief Project manager
 * @author David Cuadrado <krawek@toonka.com>
*/

class Q_DECL_EXPORT KTProjectManager : public KTSerializableObject
{
	Q_OBJECT
	public:
		/**
		 * Constructor por defecto
		 */
		KTProjectManager(QObject *parent = 0);
		
		/**
		 * Destructor por defecto
		 */
		~KTProjectManager();
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		/**
		 * Pone un nombre al proyecto
		 */
		void setProjectName(const QString &name);
		
		/**
		 * @if spanish
		 * Retorna el nombre del proyecto
		 * @endif
		 * 
		 * @if english
		 * Returns project name
		 * @endif
		 */
		QString projectName() const;
		
		/**
		 * Pone el tipo de render del proyecto
		 */
// 		void setProjectRender( KToon::RenderType);
		
		/**
		 * Pone los frames por segundo por defecto del proyecto, este valor sera usado para crear nuevas escenas
		 */
		void setProjectFPS( int fps );
		
		/**
		 * Pone un tamaño al documento
		 */
		void setDocumentSize(const QSize& size );
		
		/**
		 * Retorna el tamaño del documento
		 */
		QSize documentSize() const;
		
		/**
		 * Retorna el tipo de render del proyecto
		 */
// 		QString projectRender( ) const;
		
		/**
		 * Retorna el numero de frames por segundo por defecto del proyecto
		 */
		int fps();
		
		/**
		 * Retorna verdadero si el proyecto esta abierto
		 */
		bool isOpen();
		
		/**
		 * Inicializa el proyecto, esta funcion abre un nuevo proyecto
		 */
		void init();
		
	public slots:
		/**
		 * Guarda el proyecto
		 */
		void save();
		
		/**
		 * Cierra el proyecto
		 */
		void close();
		
		/**
		 * Carga del proyecto desde una ruta
		 */
		bool load(const QString &path);
		
		// Documents
		/**
		 * Pone el documento actual desde un indice
		 */
		void setCurrentDocument(int index);
		
		/**
		 * Retorna el documento actual
		 */
		KTDocument *currentDocument() const;
		
		/**
		 * Crea un nuevo documento
		 */
		KTDocument *createDocument(const QString &name = QString());
		
		/**
		 * Retorna una lista de documentos
		 */
		Documents documents() const;
		
		/**
		 * Pone los documentos, esta funcion sobre escribe los documentos anteriores
		 */
		void setDocuments(const Documents &);
		
		//Scenes
		/**
		 * Crea una escena
		 */
		void createScene(bool addToEnd = true);
		
		/**
		 * Remueve una escena
		 */
		void removeScene();
		
		/**
		 * Renombra una escena
		 */
		void renameScene(const QString &name, int index);
		
		/**
		 * Pone la escena actual desde un indice
		 */
		void setCurrentScene(int index);
		
		/**
		 * Retorna la escena actual
		 */
		KTScene *currentScene();
		
		
		// Layer
		/**
		 * Crea un layer
		 */
		void createLayer(bool addToEnd = true);
		
		/**
		 * Pone el layer actual desde un indice
		 */
		void setCurrentLayer(int index);
		
		/**
		 * Cambia la visibilidad de un layer
		 */
		void setLayerVisibility(int index, bool value);
		
		/**
		 * Retorna el layer actual
		 */
		KTLayer *currentLayer();
		
		
		/**
		 * Mueve el layer actual, si up es verdadero lo mueve hacia arriba
		 * @param up 
		 */
		void moveLayer(bool up);
		
		/**
		 * Remueve el layer con un indice
		 */
		void removeLayer(int index);
				
		// Frames
		/**
		 * Crea un frame
		 */
		void createFrame(bool addToEnd = true);
		
		/**
		 * Retorna el frame actual
		 */
		KTKeyFrame *currentKeyFrame();
		
		/**
		 * Pone el frame actual desde un indice
		 */
		void setCurrentFrame(int index);
		
		/**
		 * Copia un frame
		 */
		void copyFrame(int index);
		
		
		/**
		 * Pega un frame
		 */
		void pasteFrame(int index);
		
		void cloneFrame(int index, int nClones);
		
		/**
		 * Mueve el frame actual
		 */
		void moveFrame(bool up);
		
		/**
		 * Remueve el frame actual
		 */
		void removeFrame();
		
		/**
		 * Bloquea el frame actual
		 */
		void lockCurrentFrame();
		
		/**
		 * Renombra un Layer 
		*/
		void renameLayer(int indexLayer, const QString & name);
		
		/**
		 * Renombra un Frame 
		 */
		void renameFrame(int indexLayer, int indexFrame, const QString & name);
		
	private:
		void connectScene(const KTScene *scene);
		
		
	private slots:
		// Layers
		void emitLayerVisibility(bool);
		
	signals:
		// Documents
		/**
		 * Este signal se emite cuando un documento es creado
		 */
		void documentCreated(const QString &name);
		
		// Scenes
		/**
		 * Este signal se emite cuando una escena es creada
		 */
		void sceneCreated(const QString &name, bool addedToEnd);
		
		/**
		 * Este signal se emite cuando una escena es renombrado
		 */
		void sceneRenamed(const QString &name, int id);
		
		
		// Layers
		/**
		 * Este signal se emite cuando una layer es creado
		 */
		void layerCreated(const QString &name, bool addedToEnd);
		
		/**
		 * Este signal es emitido cuando la visibilidad del layer cambia
		 */
		void layerVisibilityChanged(int index, bool );
		
		/**
		 * Este signal se emite cuando un layer es removido
		 */
		void layerRemoved(int );
		
		/**
		 * Este signal se emite cuando un layer es seleccionado
		 */
		void layerSelected(int );
		
		/**
		 * Este signal se emite cuando un layer ha sido movido, si up es verdadero el layer se movio hacia arriba
		 * @param up 
		 */
		void layerMoved(bool up);
		
		/**
		 * Este signal se emite cuando un layer ha sido renombrado
		 */
		void layerRenamed(int indexLayer, const QString & name);
		
		// Frames
		/**
		 * Este signal se emite cuando un frame es creado
		 */
		void frameCreated( const QString &name, bool addedToEnd);
		
		/**
		 * Este signal se emite cuando un frame es movido
		 */
		void frameMoved(bool up);
		
		/**
		 * Este signal se emite cuando un frame es removido
		 */
		void frameRemoved();
		
		/**
		 * Este signal se emite cuando un frame es bloqueado
		 */
		void frameLocked();
		
		/**
		 * este signal se emite cuando un frame es renombrado
		 */
		void frameRenamed(int indexLayer, int indexFrame, const QString & name);
		
		
	private:
		Documents m_documents;
		KTDocument *m_currentDocument;
		QSize m_size;
		const KTKeyFrame *m_copyFrame;
		mutable QString m_name;
		bool m_isOpen;
		int m_fps;
		
};

#endif
