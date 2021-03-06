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

#include "ktconfigurationarea.h"

#include <QLabel>
#include <QTextBrowser>
#include <QMainWindow>
#include <QMouseEvent>
#include <QApplication>
#include <QPushButton>
#include <QToolTip>
#include <QPainter>
#include <QPainterPath>

#include <ddebug.h>

KTConfigurationArea::KTConfigurationArea(QWidget *parent) : QDockWidget(parent), m_separator(0), m_toolTipShowed(false)
{
	setAllowedAreas ( Qt::RightDockWidgetArea );
	
	QTimer::singleShot(1000,this, SLOT(findSeparator()));
	
	connect(&m_locker, SIGNAL(timeout()), this, SLOT(toggleLock()));
	connect(&m_shower, SIGNAL(timeout()), this, SLOT(showConfigurator()));
}

KTConfigurationArea::~KTConfigurationArea()
{
	if ( widget() )
	{
		widget()->hide();
		widget()->setParent(0);
	}
}

void KTConfigurationArea::setConfigurator(QWidget *w)
{
	QWidget *old = widget();
	
	if ( !w || old == w ) return;
	
	if ( old )
	{
		old->hide();
	}
	
	setWidget(w);
	
	showConfigurator();
}

void KTConfigurationArea::findSeparator()
{
	D_FUNCINFO;
	if ( m_separator )
	{
		return;
	}
	
	QMainWindow *mw = qobject_cast<QMainWindow *>(parentWidget());
	
	if (mw)
	{
		QList<QWidget*> widgets = mw->findChildren<QWidget*>();
		QList<QWidget*>::ConstIterator it = widgets.begin();
		
		while (it != widgets.end())
		{
			if ((*it)->metaObject()->className() == QString("QDockSeparator")) // HACK
			{
				m_separator = (*it);
				break;
			}
			++it;
		}
	}
}

void KTConfigurationArea::toggleLock()
{
	m_locker.stop();
	hideConfigurator();
}

void KTConfigurationArea::shrink()
{
	D_FUNCINFO;
	if ( ! m_separator )
	{
		findSeparator();
		return;
	}
	
	bool hmt = m_separator->hasMouseTracking();
	
	m_separator->setMouseTracking(true);
	QMouseEvent press(QEvent::MouseButtonPress,
			  QPoint(m_separator->x(), m_separator->y()),
			  Qt::LeftButton, 0, 0);
	QApplication::sendEvent(m_separator, &press);
	
	int df = 0;
	int x1 = 0, x2= 0, y1= 0, y2= 0, xRelease= 0, yRelease= 0;

	df = width() - 20;
	x1 = press.pos().x() + df;
	y1 = press.pos().y();
	
	x2 = press.globalPos().x() + df;
	y2 = press.globalPos().y();
	
	xRelease = width() - 20;
	yRelease = m_separator->y();
	
	QMouseEvent move(QEvent::MouseMove,
			 QPoint(x1, y1),
			 QPoint(x2, y2),
			 Qt::LeftButton, 0, 0);
	QApplication::sendEvent(m_separator, &move);

	QMouseEvent release(QEvent::MouseButtonRelease,
			    QPoint(xRelease, yRelease),
			    Qt::LeftButton, 0, 0);
	QApplication::sendEvent(m_separator, &release);
	
	m_separator->setMouseTracking(hmt);
}

void KTConfigurationArea::enterEvent(QEvent *)
{
	if ( m_locker.isActive()) m_locker.stop();
	
	if ( m_shower.isActive() )
	{
		return;
	}
	
	m_shower.start(300);
}

void KTConfigurationArea::leaveEvent(QEvent *)
{
	if ( m_shower.isActive()) m_shower.stop();
	
	if ( m_locker.isActive() || rect().contains(mapFromGlobal(QCursor::pos())) || hasFocus() )
	{
		return;
	}
	
	m_locker.start(1000);
}

void KTConfigurationArea::showConfigurator()
{
	QWidget *widget = this->widget();
	
	if ( widget && !isFloating () )
	{
// 		widget->setMinimumWidth(0);
		widget->setVisible(true);
		
		QPalette pal = parentWidget()->palette();
		setPalette(pal);
		
		setAutoFillBackground(false);
		
		setFeatures(QDockWidget::AllDockWidgetFeatures );
		
		
	}
	
	m_shower.stop();
	
	m_mousePos = QCursor::pos();
}

void KTConfigurationArea::hideConfigurator()
{
	QWidget *widget = this->widget();
	
	if ( widget && !isFloating () )
	{
// 		widget->setMinimumWidth(10);
		widget->setVisible(false);
		setFeatures(QDockWidget::NoDockWidgetFeatures );
		
		// =================
		
		QPalette pal = palette();
		pal.setBrush(QPalette::Background, pal.button() );
		setPalette(pal);
		
		setAutoFillBackground(true);
		
		
		//==================
		
		for (int i = 0; i < 2; ++i)
			qApp->processEvents();
		shrink();
		
		
		if ( !m_toolTipShowed )
		{
			QToolTip::showText (m_mousePos, tr("Cursor here for expand"), this );
			m_toolTipShowed = true;
		}
	}
	
	m_mousePos = QCursor::pos();
}

void KTConfigurationArea::paintEvent (QPaintEvent *e)
{
	QDockWidget::paintEvent(e);
	
	bool draw = false;
	
	if( widget() )
	{
		if ( widget()->isVisible() )
		{
			draw = false;
		}
		else
		{
			draw = true;
		}
	}
	else
	{
		draw = false;
	}
	
	if ( draw )
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		
		painter.setBrush( palette().highlight() );
		QPainterPath path;
		
		QPolygon pol;
		pol << rect().topRight()-QPoint(0,-40) << QPoint(2, height()/2) << rect().bottomRight()-QPoint(0,40);
		path.addPolygon(pol);
		
		painter.drawPath(path);
		
		painter.rotate(90);
		
		QFont font("Times", 16, QFont::Bold);
		
		painter.setFont(font);
		
		QString text = tr("Properties");
		
		QFontMetricsF fm(painter.font());
		
		
		painter.drawText(QPointF(height()/2-fm.width(text)/2, -(width()-fm.height()/2) ), text );
	}
}






