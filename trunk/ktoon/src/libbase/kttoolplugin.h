/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTTOOLPLUGIN_H
#define KTTOOLPLUGIN_H

#include <QObject>
#include "kttoolinterface.h"

#include "ktglobal.h"

class KTGraphicsScene;
class QGraphicsView;
class KTInputDeviceInformation;
class KTBrushManager;
class KTItemResponse;
class KTProjectRequest;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class KTOON_EXPORT KTToolPlugin : public QObject, public KTToolInterface
{
	Q_OBJECT;
	Q_INTERFACES(KTToolInterface);
	
	public:
		explicit KTToolPlugin(QObject * parent = 0);
		~KTToolPlugin();
		
		void setCurrentTool(const QString &tool);
		QString currentTool() const;
		
		virtual void init(KTGraphicsScene *scene);
		
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene ) = 0;
		virtual void doubleClick(const KTInputDeviceInformation *input, KTGraphicsScene *scene );
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene) = 0;
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene) = 0;
		
		virtual QMap<QString, DAction *> actions() const = 0;
		
		virtual QWidget *configurator()  = 0;
		virtual void aboutToChangeTool() = 0;
		virtual void aboutToChangeScene(KTGraphicsScene *scene);
		
		virtual void itemResponse(const KTItemResponse *event);
		
		virtual void keyPressEvent(QKeyEvent *event);
		
		virtual QMenu *menu() const;
		
	signals:
		void requested(const KTProjectRequest *request);
		
	public:
		// Events
		void begin();
		void end();
		
	private:
		struct Private;
		Private *const d;
};

#endif

