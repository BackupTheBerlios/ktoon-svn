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
		
		void recoverItems();
		
		void addGraphic(QGraphicsItem *item);
		void removeGraphic(QGraphicsItem *item);
		void replaceGraphic(int position, QGraphicsItem *item);
		
		bool removeItemAt(int position);
		QGraphicsItem *createItem(int position, const QString &xml);
		
		QList<QGraphicsItem *> graphics() const;
		
		
		QGraphicsItem *item(int position) const;
		
		QGraphicsItemGroup *createItemGroupAt( int position, const QList<qreal> & group );
		
		KTLayer *layer() const;
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc);
		
	private:
		void init();
		QString m_name;
		bool m_isLocked;
		bool m_isVisible;
		
		QList<QGraphicsItem *> m_items;
};

#endif
