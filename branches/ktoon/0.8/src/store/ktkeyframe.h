/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTKEYFRAME_H
#define KTKEYFRAME_H

#include <QObject>
#include <QList>

#include "agraphiccomponent.h"
#include "ktserializableobject.h"

class KTKeyFrame;

/**
 * @brief Esta clase representa un marco o frame de la animacion
 * @author David Cuadrado <krawek@toonka.com>
*/
class Q_DECL_EXPORT KTKeyFrame : public KTSerializableObject
{
	public:
		/**
		 * Constructor por defecto
		 */
		KTKeyFrame(QObject *parent = 0);
		
		/**
		 * Construye un frame con un nombre
		 */
		KTKeyFrame(const QString &frameName, QObject * parent = 0);
		
		/**
		 * Constructor de copia
		 */
		KTKeyFrame(const KTKeyFrame &kf);
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		/**
		 * Destructor
		 */
		~KTKeyFrame();
		
		/**
		 * Pone los componentes
		 */
		void setComponents(const QList<AGraphicComponent *> &components);
		
		/**
		 * Añade un componente al frame
		 */
		void addComponent(AGraphicComponent *comp);
		
		
		void insertComponent(int pos, AGraphicComponent *comp);
		
		/**
		 * Añade una lista de componentes al frame, esta funcion sobreescribe los componentes anteriores
		 */
		void addComponents(QList<AGraphicComponent *> comp);
		
		
		/**
		 * Remueve un componente
		 */
		void removeComponent(AGraphicComponent *comp);
		
		/**
		 * Toma el ultimo componente
		 */
		AGraphicComponent *takeLastComponent();
		
		/**
		 * Retorna la lista de componentes graficos
		 */
		QList<AGraphicComponent *> components() const;
		
		/**
		 * Pone el nombre del frame
		 */
		void setFrameName(const QString &name);
		
		/**
		 * Bloquea el frame
		 */
		void setLocked(bool isLocked);
		
		/**
		 * Retorna el nombre del frame
		 */
		QString frameName() const;
		
		/**
		 * Returna verdadero cuando el frame esta bloqueado
		 */
		bool isLocked();
		
		/**
		 * AÃ±ade un componenente seleccionado al frame
		 */
		void addSelectedComponent(AGraphicComponent *toSelect);
		
		/**
		 * Deselecciona un componente del frame
		 */
		void deselectComponent(AGraphicComponent *toDeSelect);
		
		/**
		 * Deselecciona todos los componentes seleccionados
		 */
		void clearSelections();
		
		/**
		 * Deselecciona todos los componentes seleccionados y remueve los puntos de control
		 */
		void removeSelections();
		
		/**
		 * Selecciona todos los componentes
		 */
		void selecteAllComponents();
		
		/**
		 * Selecciona los componentes que estan en el rect
		 */
		void selectContains (const QRect & rect);
		
		/**
		 * Retorna la lista de componentes seleccionados
		 */
		 QList<AGraphicComponent *> selectedComponents();
		 
		 /**
		  * Escala el frame
		  */
		 void scale(int sX, int sY);
		 
		 /**
		  * Esta funcion retorna verdadero si el frame tiene componentes seleccionados
		  */
		bool hasSelections() const;
		
		/**
		 * Replace the orig component with newComponent
		 * @param orig 
		 * @param newComponent 
		 */
		void replace(AGraphicComponent *orig, AGraphicComponent *newComponent);
		
		void bringToFromSelected(); 
		
		void sendToBackSelected();
		
		void oneStepForwardSelected();
		void oneStepBackwardSelected();
		
		void setClonesNumber(int nClones);
		
		void clear(bool alsoDelete = false);
		
		int clonesNumber();
		
	private:
		QList<AGraphicComponent *> m_components;
		QList<AGraphicComponent *> m_selectedComponents;
		QString m_name;
		bool m_isLocked;
		int m_nClones;
};

#endif
