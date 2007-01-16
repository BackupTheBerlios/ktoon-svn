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

#ifndef KTFRAME_H
#define KTFRAME_H

#include <QGraphicsScene>
#include "ktabstractserializable.h"

#include <QDomDocument>
#include <QDomElement>
#include "ktglobal_store.h"

class KTFrame;
class KTLayer;
class KTGraphicObject;
class KTProject;
class KTScene;

/**
 * @brief Esta clase representa un marco o frame de la animacion
 * @author David Cuadrado \<krawek@toonka.com\>
*/
class STORE_EXPORT KTFrame : public QObject, public KTAbstractSerializable
{
	public:
		/**
		 * Constructor por defecto
		 */
		KTFrame(KTLayer *parent);
		
		/**
		 * Destructor
		 */
		~KTFrame();
		
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
		bool isLocked() const;
		
		
		void setVisible(bool isVisible);
		bool isVisible() const;
		
		void addItem(QGraphicsItem *item);
		
		void replaceItem(int position, QGraphicsItem *item);
		
		bool removeGraphicAt(int position);
		QGraphicsItem *createItem(int position, const QString &xml, bool loaded = false);
		
		QList<KTGraphicObject *> graphics() const;
		
		
		KTGraphicObject *graphic(int position) const;
		QGraphicsItem *item(int position) const;
		
		QGraphicsItemGroup *createItemGroupAt( int position, QList<qreal> group );
		QList<QGraphicsItem *> destroyItemGroup(int position);
				
		KTLayer *layer() const;
		KTScene *scene() const;
		KTProject *project() const;
		
		int indexOf(KTGraphicObject *object);
		int indexOf(QGraphicsItem *item);
		int index() const;
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc) const;
		
	private:
		QString m_name;
		bool m_isLocked;
		bool m_isVisible;
		
		QList<QGraphicsItem *> m_items;
		QList<KTGraphicObject *> m_graphics;
};

#endif
