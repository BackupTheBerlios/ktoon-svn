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
 
#ifndef ATEXTTOOL_H
#define ATEXTTOOL_H

#include <kttoolplugin.h>
#include <kttextitem.h>

#include "textconfigurator.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class TextTool : public KTToolPlugin
{
	Q_OBJECT;
	
	public:
		TextTool();
		virtual ~TextTool();
		
		virtual QStringList keys() const;
		
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view);
		virtual void doubleClick(const KTInputDeviceInformation *input, KTGraphicsScene *scene, QGraphicsView *view );
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view);
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view);
		virtual bool itemPressed(QGraphicsItem *item);
		
		virtual QMap<QString, DAction *>actions() const;
		
		int toolType() const;
		
		virtual QWidget *configurator();
		
		void aboutToChangeTool();
		
	private:
		void setupActions();
		
	private:
		KTTextItem *m_item;
		
		TextConfigurator *m_configurator;
		
		QPoint m_position;
		
		QMap<QString, DAction *> m_actions;

};

#endif
