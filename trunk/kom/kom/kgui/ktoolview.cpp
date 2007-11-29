/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktoolview.h"
#include "kviewbutton.h"
#include "kmainwindow.h"

#include <QtDebug>
#include <QAction>
#include <QMainWindow>
#include <QMouseEvent>
#include <QLayout>
#include <QEvent>

KToolView::KToolView(const QString &title, const QIcon &icon, QWidget * parent)
	: QDockWidget(title, parent), m_size(-1), m_perspective(0)
{
	setWindowIcon(icon);
	setup();
	
	setObjectName("KToolView-"+title);
}


KToolView::~KToolView()
{
}

void KToolView::setup()
{
	
#if QT_VERSION < 0x040200
	m_area = Qt::AllDockWidgetAreas;
#endif

	setFeatures(AllDockWidgetFeatures);
// 	setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	
	m_button = new KViewButton(this);
	
	connect(toggleViewAction(), SIGNAL(toggled(bool)), this, SLOT(saveSize(bool)));
}


KViewButton *KToolView::button() const
{
	return m_button;
}

void KToolView::saveSize(bool checked)
{
	if ( m_button->area() == Qt::LeftToolBarArea || m_button->area() == Qt::RightToolBarArea )
	{
		m_size = width();
	}
	else
	{
		m_size = height();
	}
	
	setVisible(checked);
}

QSize KToolView::sizeHint() const
{
	QSize size = QDockWidget::sizeHint();
	
	if ( m_size < 0 ) return size;
	
	if ( m_button->area() == Qt::LeftToolBarArea || m_button->area()  == Qt::RightToolBarArea )
	{
		size.setWidth(m_size);
	}
	else
	{
		size.setHeight(m_size);
	}
	
	return size;
}


void KToolView::setDescription(const QString &description)
{
	m_button->setStatusTip ( description );
}

void KToolView::setPerspective(int wsp)
{
	m_perspective = wsp;
}

int KToolView::perspective() const
{
	return m_perspective;
}

int KToolView::fixedSize() const
{
	return m_size;
}

void KToolView::setFixedSize(int s)
{
	m_size = s;
}

void KToolView::showEvent(QShowEvent *e)
{
	if ( KMainWindow *mw = dynamic_cast<KMainWindow *>(parentWidget() ) )
	{
		if ( !(mw->currentPerspective() & m_perspective) ) 
		{
			e->ignore(); // make sure!
			return;
		}
	}
	
	QDockWidget::showEvent(e);
}

#if QT_VERSION < 0x040200

bool KToolView::event(QEvent *e)
{
	bool toReturn =  QDockWidget::event(e);
	
	if (e->type() == QEvent::MouseButtonPress )
	{
		if ( QMainWindow *mw = dynamic_cast<QMainWindow *>(parentWidget()) )
		{
			m_area = mw->dockWidgetArea(this);
		}
	}
	else if ( e->type() == QEvent::MouseButtonRelease )
	{
		if ( QMainWindow *mw = dynamic_cast<QMainWindow *>(parentWidget()) )
		{
			Qt::DockWidgetArea newArea = mw->dockWidgetArea(this);
			if ( m_area !=  newArea)
			{
				mw->removeDockWidget(this);
				mw->addDockWidget(newArea, this );
				
				emit topLevelChanged(false);
			}
		}
	}
	return toReturn;
}

#endif





