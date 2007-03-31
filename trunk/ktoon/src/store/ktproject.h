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

#ifndef KTPROJECT_H
#define KTPROJECT_H

#include "ktabstractserializable.h"
#include "ktglobal.h"

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include "ktglobal_store.h"


class KTScene;
class KTLayer;
class KTFrame;
class KTProjectRequest;
class KTProjectResponse;
class QGraphicsItem;
class KTLibrary;

typedef QList<KTScene *> Scenes;


/**
 * Esta clase maneja el proyecto, además contiene las diferentes escenas que componen todo el proyecto.
 * 
 * @brief Project manager
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class STORE_EXPORT KTProject : public QObject, public KTAbstractSerializable
{
	Q_OBJECT;
	public:
		KTProject(QObject *parent = 0);
		~KTProject();
		
		void setProjectName(const QString &name);
		void setAuthor(const QString &author);
		void setDescription(const QString& description);
		
		QString projectName() const;
		QString author() const;
		QString description() const;
		
		KTScene *scene(int position);
		int indexOf(KTScene *scene) const;
		
		Scenes scenes() const;
		
		KTScene *createScene(int position, bool loaded = false);
		bool removeScene(int position);
		bool moveScene(int position, int newPosition);
		
		bool createSymbol(int type, const QString &name, const QByteArray &data);
		bool removeSymbol(const QString &name);
		
		bool addSymbolToProject(const QString &name, int scene, int layer, int frame);
		bool removeSymbolFromProject(const QString &name, int scene, int layer, int frame);
		
		void clear();
		
		void loadLibrary(const QString &filename);
		
		KTLibrary *library() const;
		void emitResponse(KTProjectResponse *response);
		
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc) const;
		
		void setOpen(bool open);
		bool isOpen();
		
	signals:
		void responsed(KTProjectResponse *response);
		
	private:
		struct Private;
		Private *const d;
};

#endif
