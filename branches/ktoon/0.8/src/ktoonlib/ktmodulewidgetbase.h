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

#include "ktmodulewidgettitle.h"

/**
 * @author Jorge Cuadrado
*/
class Q_GUI_EXPORT KTModuleWidgetBase : public QWidget
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
		bool m_isChildHidden;
		QWidget *m_originalParent;
		
	protected:
		KTModuleWidgetTitle *m_title;
		
	protected:
		virtual bool event( QEvent * e );
		
	public slots:
		void toggleView();
		void setCaption(const QString &text);
		
	protected:
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		
	signals:
		void documentModified(bool);
		void sendToStatus(const QString &);
		void sendToOSD(const QString &msg, int level);
		void toggle();
		void activate(bool);
};

#endif
