/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#ifndef KTMODULEWIDGETBASE_H
#define KTMODULEWIDGETBASE_H

#include <qlayout.h>
#include <qsizepolicy.h> 
#include <qobject.h>
#include <qevent.h>

#include <QBoxLayout>
#include <QDockWidget>

#include "dosd.h"
#include "ktprojectevent.h"
#include "ktabstractprojecteventhandler.h"
#include "ktglobal.h"

/**
 * @author Jorge Cuadrado
*/
class KTOON_EXPORT KTModuleWidgetBase : public QWidget, public KTAbstractProjectEventHandler
{
	Q_OBJECT
	public:
		KTModuleWidgetBase(QWidget *parent, const char *name = 0);
		~KTModuleWidgetBase();
		virtual void addChild(QWidget * child, Qt::Alignment alignment = 0);
		void setFont(const QFont &);
		
		QBoxLayout *boxLayout();
		
	private:
		QBoxLayout *m_container;
		QObjectList m_childs;
		
	protected:
		virtual bool event( QEvent * e );
		
	public slots:
		void setCaption(const QString &text);
		bool handleProjectEvent(KTProjectEvent *e);
		
		
	protected:
		virtual void enterEvent(QEvent *e);
		virtual void leaveEvent(QEvent *e);
		virtual void frameEvent(KTFrameEvent *frameEvent);
		virtual void layerEvent(KTLayerEvent *layerEvent);
		virtual void sceneEvent(KTSceneEvent *sceneEvent);
		virtual void projectEvent(KTProjectEvent *projectEvent);
		virtual void itemEvent(KTItemEvent *event);
		
	signals:
		void documentModified(bool);
		void sendToStatus(const QString &);
		void sendToOSD(const QString &msg, DOsd::Level level);
		void toggle();
		void activate(bool);
		
		void eventTriggered(const KTProjectEvent *event);
};

#endif