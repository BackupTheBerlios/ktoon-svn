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

#include <qobject.h>
#include "kttoolinterface.h"
#include "ktbrushmanager.h"

#include <QGraphicsView>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_DECL_EXPORT KTToolPlugin : public QObject, public KTToolInterface
{
	Q_OBJECT;
	Q_INTERFACES(KTToolInterface);
	
	public:
		explicit KTToolPlugin(QObject * parent = 0);
		~KTToolPlugin();
		
		void setCurrentTool(const QString &tool);
		QString currentTool() const;
		
		virtual void init(QGraphicsView *view);
		
		virtual void press(const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view ) = 0;
		virtual void move(const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view) = 0;
		virtual void release(const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view) = 0;
		
		virtual QMap<QString, DAction *> actions() const = 0;
		
		virtual QWidget *configurator()  = 0;
		
	private:
		QString m_currentTool;
};

#endif

