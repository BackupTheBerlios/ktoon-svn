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
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTKeyFrame : public KTSerializableObject
{
	public:
		KTKeyFrame(QObject *parent = 0);
		KTKeyFrame(const QString &frameName, QObject * parent = 0);
		KTKeyFrame(const KTKeyFrame &kf);
		
		QDomElement createXML( QDomDocument &doc );
		
		~KTKeyFrame();
		void addComponent(AGraphicComponent *comp);
		void removeComponent(AGraphicComponent *comp);
		AGraphicComponent *takeLastComponent();
		
		QList<AGraphicComponent *> components() const;
		
		void setFrameName(const QString &name);
		void setLocked(bool isLocked);
		
		QString frameName() const;
		bool isLocked();
		
	private:
		QList<AGraphicComponent *> m_components;
		QString m_name;
		bool m_isLocked;
};

#endif
