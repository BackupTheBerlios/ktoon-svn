/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                *
 *   krawek@gmail.com                                                      *
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

#include "dviewbutton.h"

#include <QToolBar>
#include <QStylePainter>
#include <QMenu>
#include <QIcon>
#include <QMouseEvent>

#include <QTimer>
#include <QtDebug>

class DViewButton::Animator
{
	public:
		Animator(QObject *parent) : count(0), MAXCOUNT(20), INTERVAL(30), isEnter(false)
		{
			timer = new QTimer(parent);
		}
		~Animator() {}
		void start()
		{
			timer->start(INTERVAL);
		}
		
		QColor blendColors( const QColor& color1, const QColor& color2, int percent )
		{
			const float factor1 = ( 100 - ( float ) percent ) / 100;
			const float factor2 = ( float ) percent / 100;

			const int r = static_cast<int>( color1.red() * factor1 + color2.red() * factor2 );
			const int g = static_cast<int>( color1.green() * factor1 + color2.green() * factor2 );
			const int b = static_cast<int>( color1.blue() * factor1 + color2.blue() * factor2 );
			
			QColor result;
			result.setRgb( r % 255, g % 255, b % 255 );
			
			return result;
		}
			
		QTimer *timer;
		int count;
		const int MAXCOUNT;
		const int INTERVAL;
			
		bool isEnter;
};

DViewButton::DViewButton(Qt::ToolBarArea area, QWidget * parent) : QToolButton(parent), m_area(area)
{
	setup();
}


DViewButton::DViewButton(QWidget *parent) : QToolButton(parent), m_area(Qt::LeftToolBarArea)
{
	setup();
}

void DViewButton::setup()
{
	setAutoExclusive(true);
	setCheckable( true );
	
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setFocusPolicy(Qt::NoFocus);
	
	m_isSensible = false;
	
	m_menu = new QMenu();
	m_menu->addAction(tr("Only icon"), this, SLOT(setOnlyIcon()) );
	m_menu->addAction(tr("Only text"), this, SLOT(setOnlyText()) );
	m_menu->addSeparator();
	m_menu->addAction(tr("Mouse sensibility"), this, SLOT(toggleSensibility()))->setCheckable(true);
	
	m_animator = new Animator(this);
	connect( m_animator->timer, SIGNAL( timeout() ), this, SLOT( animate() ) );
	
	setChecked(false);
	setDown(false);
}

DViewButton::~DViewButton()
{
	delete m_animator;
}

void DViewButton::setOnlyText()
{
	setToolButtonStyle(Qt::ToolButtonTextOnly);
}

void DViewButton::setOnlyIcon()
{
	setToolButtonStyle(Qt::ToolButtonIconOnly);
}


void DViewButton::setArea(Qt::ToolBarArea area)
{
	m_area = area;
	update();
}

Qt::ToolBarArea DViewButton::area() const
{
	return m_area;
}

QSize DViewButton::sizeHint() const
{
	QSize size = QToolButton::sizeHint();
	
	if ( m_area == Qt::RightToolBarArea || m_area == Qt::LeftToolBarArea )
	{
		size.transpose();
	}
	
	return size;
}


QStyleOptionToolButton DViewButton::styleOption() const
{
	QStyleOptionToolButton opt;
	opt.init(this);
	
	bool down = isDown();
	bool checked = defaultAction() ? defaultAction()->isChecked() : isChecked();
	bool forceNoText = false;
	
	
	opt.toolButtonStyle = toolButtonStyle();
	if ( m_area == Qt::LeftToolBarArea )
	{
		QSize size = opt.rect.size();
		size.transpose();
		opt.rect = QRect(QPoint(opt.rect.x(), opt.rect.y()), size );
	}
	else if ( m_area == Qt::RightToolBarArea )
	{
		QSize size = opt.rect.size();
		size.transpose();
		opt.rect = QRect(QPoint(opt.rect.x(), opt.rect.y()), size );
	}
	
	if (parentWidget())
	{
		if (QToolBar *toolBar = qobject_cast<QToolBar *>(parentWidget()))
		{
			opt.iconSize = toolBar->iconSize();
		}
		else 
		{
			opt.iconSize = iconSize();
		}
	}

	if (!forceNoText)
	{
		opt.text = text();
	}
	
	opt.icon = icon();
	
	opt.arrowType = arrowType();
	
#if 1
	if (down)
		opt.state |= QStyle::State_Sunken;
// 	if (checked)
// 		opt.state |= QStyle::State_On;
	if (autoRaise())
		opt.state |= QStyle::State_AutoRaise;
	if (!checked && !down)
		opt.state |= QStyle::State_Raised;
#endif 

	opt.subControls = QStyle::SC_ToolButton;
	opt.activeSubControls = QStyle::SC_None;

	opt.features = QStyleOptionToolButton::None;
// 	if (popupMode() == QToolButton::MenuButtonPopup) {
// 		opt.subControls |= QStyle::SC_ToolButtonMenu;
// 		opt.features |= QStyleOptionToolButton::Menu;
// 		if (menuButtonDown || down) {
// 			opt.state |= QStyle::State_MouseOver;
// 			opt.activeSubControls |= QStyle::SC_ToolButtonMenu;
// 		}
// 	} 
// 	else 
// 	{
// 		if (menuButtonDown)
// 			opt.state  |= QStyle::State_Sunken;
// 	}
	
	if (arrowType() != Qt::NoArrow)
	{
		opt.features |= QStyleOptionToolButton::Arrow;
	}
	
	if (popupMode() == QToolButton::DelayedPopup)
	{
		opt.features |= QStyleOptionToolButton::PopupDelay;
	}
	
// 	opt.toolButtonStyle = toolButtonStyle();
// 	
// 	if (icon().isNull() && arrowType() == Qt::NoArrow && !forceNoText) 
// 	{
// 		if (!text().isEmpty())
// 		{
// 			opt.toolButtonStyle = Qt::ToolButtonTextOnly;
// 		}
// 		else if (opt.toolButtonStyle != Qt::ToolButtonTextOnly)
// 		{
// 			opt.toolButtonStyle = Qt::ToolButtonIconOnly;
// 		}
// 	}
// 	else
// 	{
// 		if (text().isEmpty() && opt.toolButtonStyle != Qt::ToolButtonIconOnly)
// 		{
// 			opt.toolButtonStyle = Qt::ToolButtonIconOnly;
// 		}
// 	}

	opt.pos = pos();
	opt.font = font();
	return opt;
}

void DViewButton::paintEvent(QPaintEvent *e)
{
	QStyleOptionToolButton opt = styleOption();
	
	QRect r = opt.rect;
	QColor fillColor, textColor;
	
	bool checked = defaultAction() ? defaultAction()->isChecked() : isChecked();
	
	if ( m_animator->count <= 0 ) m_animator->count = 1;
	
	if ( checked )
	{
		fillColor = m_animator->blendColors( palette().highlight().color(), palette().background().color(), static_cast<int>( m_animator->count * 3.5 ) % 100 );
		textColor = m_animator->blendColors( palette().highlightedText().color(), palette().text().color(), static_cast<int>( m_animator->count * 4.5 ) % 100 );
	}
	else
	{
		fillColor = m_animator->blendColors( palette().background().color(), palette().highlight().color(), static_cast<int>( m_animator->count * 3.5 ) % 100 );
		textColor = m_animator->blendColors( palette().text().color(), palette().highlightedText().color(), static_cast<int>( m_animator->count * 4.5 ) % 100 );
	}
	
	opt.palette.setColor(QPalette::Background, fillColor);
// 	opt.palette.setColor(QPalette::Text, textColor);
	
// 	opt.palette.setColor(QPalette::Button, fillColor);
	opt.palette.setColor(QPalette::ButtonText, textColor);

	QPixmap pm(r.width(), r.height());
	pm.fill(fillColor);
	
	
	QStylePainter p(&pm, this);
	p.drawComplexControl(QStyle::CC_ToolButton, opt);
	
	QPainter painter(this);

	switch (m_area)
	{
		case Qt::LeftToolBarArea:
			painter.rotate(-90);
			painter.drawPixmap(1-pm.width(), 0, pm);
			break;
		case Qt::RightToolBarArea:
			painter.rotate(90);
			painter.drawPixmap(0, 1-pm.height(), pm);
			break;
		default:
			painter.drawPixmap(0, 0, pm);
			break;
	}
}

void DViewButton::mousePressEvent(QMouseEvent *e)
{
	if ( e->button() == Qt::RightButton )
	{
		m_menu->exec(e->globalPos());
	}
	
	QToolButton::mousePressEvent(e);
}

void DViewButton::enterEvent( QEvent* )
{
	bool checked = defaultAction() ? defaultAction()->isChecked() : isChecked();
	
// 	qDebug() << "CHECKED: " << checked << " DOWN: " << isDown();
	
	if ( m_isSensible )
	{
		m_isSensible = false;
		animateClick();
		
		QTimer::singleShot(1000, this, SLOT(toggleSensibility()));
	}
		
	m_animator->isEnter = true;
	m_animator->count = 1;

	m_animator->start();
}

void DViewButton::leaveEvent( QEvent* )
{
	if ( m_animator->count == 0 )
	{
		m_animator->count = 1;
	}
	
	m_animator->isEnter = false;
	m_animator->timer->start();
}

void DViewButton::animate()
{
	if ( m_animator->isEnter ) 
	{
		m_animator->count += 1;
		update();
		if ( m_animator->count > m_animator->MAXCOUNT )
		{
			m_animator->timer->stop();
		}
	} else 
	{
		m_animator->count -= 1;
		if ( m_animator->count < 1 )
		{
			m_animator->timer->stop();
			m_animator->count = 1;
		}
		
		update();
	}
}

void DViewButton::toggleSensibility()
{
	m_isSensible = !m_isSensible;
}
