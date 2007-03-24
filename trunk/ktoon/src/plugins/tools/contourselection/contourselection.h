/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#ifndef CONTOURSELECTION_H
#define CONTOURSELECTION_H

#include <QObject>
#include <kttoolplugin.h>
#include <QSpinBox>

#include "ktpathitem.h"

#include <dgui/dcontrolnode.h>
#include <dgui/dnodegroup.h>


class DDControlNode;
class KTItemResponse;
class KTGraphicsScene;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */

class ContourSelection : public KTToolPlugin
{
	Q_OBJECT;
	
	public:
		ContourSelection();
		
		virtual ~ContourSelection();
		
		virtual void init(KTGraphicsScene *scene);
		virtual QStringList keys() const;
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		
		virtual QMap<QString, DAction *>actions() const;
		
		int toolType() const;
		
		virtual QWidget *configurator();
		
		virtual bool isComplete() const;
		virtual void aboutToChangeTool();
		
		
		virtual void itemResponse(const KTItemResponse *event);
		virtual void keyPressEvent(QKeyEvent *event);
		
	private:
		void setupActions();
		
	private:
		QMap<QString, DAction *> m_actions;
		QList<DNodeGroup*> m_nodeGroups; 
		KTGraphicsScene *m_scene;
		
	private slots:
		void syncNodes();
};

#endif

