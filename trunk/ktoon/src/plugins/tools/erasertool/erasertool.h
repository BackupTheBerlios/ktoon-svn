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

#ifndef ERASERTOOLPLUGIN_H
#define ERASERTOOLPLUGIN_H

#include <QObject>
#include <QLabel>

#include <kttoolplugin.h>

class KTPathItem;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class EraserTool : public KTToolPlugin
{
	Q_OBJECT;
	
	public:
		EraserTool();
		~EraserTool();
		
		virtual QStringList keys() const;
		
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		
		void itemPressed(QGraphicsItem *item, const KTBrushManager *brush, const QPointF &pos);
		
		virtual QMap<QString, DAction *> actions() const;
		
		int toolType() const;
		
		virtual QWidget *configurator();
		
		void aboutToChangeScene(KTGraphicsScene *scene);
		virtual void aboutToChangeTool();
		
	private:
		void setupActions();
		
	private:
		QMap<QString, DAction *> m_actions;
};

#endif
