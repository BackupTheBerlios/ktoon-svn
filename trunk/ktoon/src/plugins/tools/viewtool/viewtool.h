/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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
 
#ifndef VIEWTOOL_H
#define VIEWTOOL_H

#include <QObject>
#include <QLabel>

#include <kttoolplugin.h>
#include <QGraphicsRectItem>

class KTGraphicsScene;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class ViewTool: public KTToolPlugin
{
	public:
		ViewTool();
		~ViewTool();
		
		virtual QStringList keys() const;
		
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);

		virtual QMap<QString, KAction *> actions() const;
		
		int toolType() const;
		
		virtual QWidget *configurator();
		void aboutToChangeScene(KTGraphicsScene *scene);
		virtual void aboutToChangeTool();
		
	private:
		void setupActions();
		
	private:
		QMap<QString, KAction *> m_actions;
		
		QGraphicsRectItem *m_rect;
		KTGraphicsScene *m_scene;
};

#endif
