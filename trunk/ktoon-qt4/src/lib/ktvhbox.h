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

#ifndef KTVHBOX_H
#define KTVHBOX_H

class KTVHBox;

#include <QFrame>
#include <QLayout>
#include <QBoxLayout>
#include <QEvent>
#include <QMouseEvent>

/**
 * @author David Cuadrado
*/

class KTVHBox : public QFrame
{
	Q_OBJECT
	public:
		KTVHBox(QWidget *parent, Qt::Orientation o);
		KTVHBox(QWidget *parent = 0, bool isVertical = true);
		~KTVHBox();
		void moveWidgetUp(QWidget *);
		void moveWidgetDown(QWidget *);
		void switchWidgetsPosition(QWidget *first, QWidget *second);
		virtual QSize sizeHint() const;
		void setSpacing( int space );
		void setStretchFactor( QWidget* w, int stretch );
		void addWidget(QWidget *child, Qt::Alignment alignment = Qt::AlignCenter); 
		
	signals:
		void mouseAt(const QPoint &);
		
	protected:
		virtual bool event( QEvent *e );
		virtual void mouseMoveEvent(QMouseEvent *e);
		
	protected:
		QBoxLayout *m_pLayout;

};

#endif
