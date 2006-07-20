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
#include <QStyle>
#include <QStyleOptionButton>

#include <ddebug.h>

KTConfigurationArea::KTConfigurationArea(QWidget *parent) : QDockWidget(parent), m_toolTipShowed(false)
{
	setAllowedAreas ( Qt::RightDockWidgetArea );
	
// 	connect(&m_locker, SIGNAL(timeout()), this, SLOT(toggleLock()));
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

void KTConfigurationArea::toggleLock()
{
	m_locker.stop();
	hideConfigurator();
}

void KTConfigurationArea::shrink()
{
	D_FUNCINFO;
	
	QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(parentWidget());
	if ( !mainWindow || ! widget() )
	{
		D_FUNCINFO << "Fatal error";
		return;
	}
	
	bool hmt = mainWindow->hasMouseTracking();
	
	int pm = style()->pixelMetric(QStyle::PM_DockWidgetSeparatorExtent);
	
	mainWindow->setMouseTracking(true);
	
	int wOffset = 0, hOffset= 0;
	
	Qt::DockWidgetArea m_position = mainWindow->dockWidgetArea(this);
	
	if ( m_position == Qt::BottomDockWidgetArea )
	{
		wOffset = 20;
		hOffset = -(y() * 2 + pm - 1); // FIXME FIXME FIXME
	}
	else if ( m_position == Qt::LeftDockWidgetArea )
	{
		wOffset = width()+(pm/2)+1;
		hOffset = height() / 2;
	}
	else if (m_position == Qt::RightDockWidgetArea )
	{
		wOffset = -(pm/2)+1;
		hOffset = height() / 2;
	}
	
	QMouseEvent press(QEvent::MouseButtonPress,
			  mapToParent( QPoint(this->x(), this->y()))/2 + QPoint(wOffset, hOffset),
			  Qt::LeftButton, 0, 0);
	
	if ( ! QApplication::sendEvent(mainWindow, &press) )
	{
		qWarning("Fail pressing");
	}
	
	qApp->processEvents();
	
	int df = 0;
	int x1 = 0, x2= 0, y1= 0, y2= 0, xRelease= 0, yRelease= 0;
	
	if ( m_position == Qt::BottomDockWidgetArea )
	{
		df = widget()->height();
		x1 = press.pos().x();
		y1 = press.pos().y() + df;
		
		x2 = press.globalPos().x();
		y2 = press.globalPos().y() + df;
		
		xRelease = this->x();
		yRelease = 10;
	}
	else if ( m_position == Qt::LeftDockWidgetArea )
	{
		df = widget()->width();
		x1 = press.pos().x() - df;
		y1 = press.pos().y();
		
		x2 = press.globalPos().x() - df;
		y2 = press.globalPos().y();
		
		xRelease = 10;
		yRelease = this->y();
	}
	else if (m_position == Qt::RightDockWidgetArea )
	{
		df = widget()->width();
		x1 = press.pos().x() + df;
		y1 = press.pos().y();
		
		x2 = press.globalPos().x() + df;
		y2 = press.globalPos().y();
		
		xRelease = mainWindow->width();
		yRelease = this->y();
	}
	
	QMouseEvent move(QEvent::MouseMove,
			 QPoint(x1, y1),
			 QPoint(x2, y2),
			 Qt::LeftButton, 0, 0);
	
	if ( ! QApplication::sendEvent(mainWindow, &move) )
	{
		qWarning("Fail moving");
	}
	qApp->processEvents();

	QMouseEvent release(QEvent::MouseButtonRelease,
			    QPoint(xRelease, yRelease),
			    Qt::LeftButton, 0, 0);
	if ( ! QApplication::sendEvent(mainWindow, &release) )
	{
		qWarning("Fail releasing");
	}
	qApp->processEvents();
	
	mainWindow->setMouseTracking(hmt);
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
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setRenderHint(QPainter::TextAntialiasing, true);
		
// 		painter.setBrush( palette().highlight() );
// 		QPainterPath path;
// 		
// 		QPolygon pol;
// 		pol << rect().topRight()-QPoint(0,-40) << QPoint(2, height()/2) << rect().bottomRight()-QPoint(0,40);
// 		path.addPolygon(pol);
// 		
// 		painter.drawPath(path);
		
// 		painter.rotate(-90);
		
		QFont font("Times", 16, QFont::Bold);
		
		painter.setFont(font);
		
		QStyleOptionButton buttonOption;
		buttonOption.initFrom( this );
		
		buttonOption.text = tr("Properties");
		buttonOption.icon = QIcon();
		buttonOption.palette = palette();
// 		buttonOption.rect = QRect(rect().x(), rect().y()+(rect().width()-rect().y()), rect().height(), rect().width());
		buttonOption.rect = rect();
		buttonOption.state = QStyle::State_On;
		
		buttonOption.features = QStyleOptionButton::DefaultButton;
		
		style()->drawControl( QStyle::CE_PushButton, &buttonOption, &painter, this );
		
// 		QString text = tr("Properties");
// 		
// 		QFontMetricsF fm(painter.font());
// 		
// 		painter.drawText(QPointF(height()/2-fm.width(text)/2, -(width()-fm.height()/2) ), text );
	}
}






