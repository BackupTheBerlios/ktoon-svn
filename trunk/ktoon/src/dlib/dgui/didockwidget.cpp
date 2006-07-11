/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "didockwidget.h"

#include <QToolButton>
#include <QLayout>
#include <QStyle>
#include <QStackedWidget>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>

#include <QComboBox>
#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>
#include <QLabel>
#include <QScrollArea>
#include <QDialog>
#include <QMainWindow>
#include <QDebug>
#include <QWindowsStyle>
#include <QMenuBar>
#include <QStatusBar>

#include "dibuttonbar.h"
#include "dibutton.h"

#include "ddebug.h"

DiDockWidget::DiDockWidget(QWidget *parent, Position position) : QDockWidget( parent)
{
	setFeatures(QDockWidget::NoDockWidgetFeatures);
	
	m_centralWidget = new DiDockInternalWidget(this, position);

	setWidget(m_centralWidget);
	m_centralWidget->show();
	
	layout()->setSpacing(0);
	layout()->setMargin(0);
	
	connect(this, SIGNAL(topLevelChanged ( bool)), this, SLOT(setFloatingOption(bool)));
	
	setStyle(new QWindowsStyle());
}

DiDockWidget::~DiDockWidget()
{
}

void DiDockWidget::setFloatingOption(bool opt)
{
	if ( opt )
	{
		setFeatures(QDockWidget::AllDockWidgetFeatures);
	}
	else
	{
		setFeatures(QDockWidget::NoDockWidgetFeatures);
	}
}

void DiDockWidget::addWidget(const QString &title, QWidget *widget)
{
	m_centralWidget->addWidget(title, widget);
}
// 
// QSize DiDockWidget::sizeHint() const
// {
// 	return m_centralWidget->layout()->sizeHint();
// }

// QSize DiDockWidget::minimumSizeHint() const
// {
// 	return m_centralWidget->minimumSizeHint();
// }

void DiDockWidget::closeEvent(QCloseEvent *e)
{
	delete m_centralWidget;
	QDockWidget::closeEvent(e);
}

DiDockInternalWidget *DiDockWidget::centralWidget()
{
	return m_centralWidget;
}

// DiDockInternalWidget

DiDockInternalWidget::DiDockInternalWidget(QWidget *parent, DiDockWidget::Position position) : QWidget(parent), m_position(position), m_visible(false), m_toggledButton(0)
{
	Ideal::Place place = Ideal::Left;
	switch (position)
	{
		case DiDockWidget::Bottom:
		{
			m_name = "BottomToolWindow";
			place = Ideal::Bottom;
			m_internalLayout = new QVBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::BottomToTop);
			
			break;
		}
		case DiDockWidget::Left:
		{
			m_name = "LeftToolWindow";
			place = Ideal::Left;
			m_internalLayout = new QHBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::LeftToRight);

			break;
		}
		case DiDockWidget::Right:
		{
			m_name = "RightToolWindow";
			place = Ideal::Right;
			m_internalLayout = new QHBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::RightToLeft);

			break;
		}
	}
	
	setObjectName(m_name);

// 	m_internalLayout->setSizeConstraint(QLayout::SetMaximumSize);
	m_internalLayout->setMargin(0);
	m_internalLayout->setSpacing(0);
	
	QSettings config;
	config.beginGroup("DLSLib-"+objectName());
	int mode = config.value( "MDIStyle", 3 ).toInt();

	Ideal::ButtonMode buttonMode = Ideal::Text;
	if (mode == 0)
		buttonMode = Ideal::Icons;
	else if (mode == 1)
		buttonMode = Ideal::Text;
	else if (mode == 3)
		buttonMode = Ideal::IconsAndText;

	m_bar = new Ideal::DiButtonBar(place, buttonMode, this);
	m_internalLayout->addWidget(m_bar);

	m_bar->show();
    
	m_widgetStack = new QStackedWidget(this);
	m_internalLayout->addWidget(m_widgetStack);
	connect(m_widgetStack, SIGNAL(widgetRemoved (int)), this, SLOT(dialoged(int )));
	
// 	QPalette pal2 = QApplication::palette();
// 	pal2.setColor(QPalette::Background, Qt::magenta);
// 	m_widgetStack->setPalette(pal2);
	
	switch (position)
	{
		case DiDockWidget::Bottom:
		{
			m_internalLayout->setAlignment( m_bar, Qt::AlignBottom | Qt::AlignLeft );
			static_cast<QDockWidget *>(parentWidget())->setAllowedAreas (Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
			break;
		}
		case DiDockWidget::Left:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignTop | Qt::AlignLeft );
			static_cast<QDockWidget *>(parentWidget())->setAllowedAreas (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			break;
		}
		case DiDockWidget::Right:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignTop | Qt::AlignRight );
			static_cast<QDockWidget *>(parentWidget())->setAllowedAreas (Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea );
			break;
		}
	}

	setExpanded(false);
	loadSettings(); // FIXME

// 	setBaseSize(m_bar->size());
	show();	
}

DiDockInternalWidget::~DiDockInternalWidget()
{
	saveSettings();
	
	QListIterator<QWidget *> iterator( m_widgets.values() );
	
	while( iterator.hasNext() )
	{
		iterator.next()->close();
	}
}

void DiDockInternalWidget::setExpanded(bool v)
{
	QSettings config;
	config.beginGroup("DLSLib-"+objectName());
	
	if (m_visible)
	{
		config.setValue( "ViewWidth", m_position == DiDockWidget::Bottom ? height() : width() );
	}
	
	m_widgetStack->setVisible(v);
	
	m_internalLayout->invalidate();
	
	m_visible = v;

	if ( ! v)
	{
		for (int i = 0; i < 2; ++i)
			qApp->processEvents();
		shrink();
		
		if ( m_toggledButton) m_toggledButton->setChecked(false);
	}
}

void DiDockInternalWidget::loadSettings()
{
}

void DiDockInternalWidget::saveSettings()
{
	QSettings config;
	config.beginGroup("DLSLib-"+objectName());
    
	int invisibleWidth = 0;
   
	invisibleWidth = config.value("ViewWidth").toInt();
	
	if (m_toggledButton && m_visible)
	{
		config.setValue("ViewWidth", m_position == DiDockWidget::Bottom ? height() : width());
		config.setValue("ViewLastWidget", m_toggledButton->realText());
	}
	else if (invisibleWidth != 0)
	{
		config.setValue("ViewWidth", invisibleWidth);
		config.remove("ViewLastWidget");
	}
}

QWidget *DiDockInternalWidget::currentWidget() const
{
	return m_widgetStack->currentWidget();
}

void DiDockInternalWidget::addWidget(const QString &title, QWidget *widget)
{
	QPixmap pm = widget->windowIcon().pixmap(16,16);
	Ideal::DiButton *button = 0;
	if (!pm.isNull())
	{
		button = new Ideal::DiButton(m_bar, title, pm);
	}
	else
	{
		button = new Ideal::DiButton(m_bar, title);
	}

	m_bar->addButton(button);

	QDesktopWidget *desktop = new QDesktopWidget();
	
	if (widget->height() > desktop->screen(desktop->primaryScreen ())->height()-230)
	{
		QScrollArea *area = new QScrollArea(m_widgetStack);
		widget->adjustSize();
		area->setWidget(widget);
		area->setWidgetResizable(true);
		m_widgetStack->addWidget(area);
		m_widgets[button] = area;
		m_buttons[area] = button;
// 		area->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
	}
	else
	{
		m_widgets[button] = widget;
		widget->setParent(m_widgetStack);
		m_widgetStack->addWidget(widget);
		m_buttons[widget] = button;
	}
	
	delete desktop;
    
	connect(button, SIGNAL(clicked()), this, SLOT(selectWidget()));
    
    //if the widget was selected last time the dock is deleted 
    //we need to show it

	QSettings config;
	config.beginGroup("DLSLib-"+objectName());
    
	if ( config.value("ViewLastWidget").toString() == title)
	{
		button->setChecked(true);
		selectWidget(button);
	}

	widget->show();
}

void DiDockInternalWidget::raiseWidget(QWidget *widget)
{
	Ideal::DiButton *button = m_buttons[widget];
	if ((button != 0) && (!button->isChecked()))
	{
		button->setChecked(true);
		selectWidget(button);
	}
}

void DiDockInternalWidget::removeWidget(QWidget *widget)
{
	if (m_widgetStack->indexOf(widget) == -1)
	{
		return; //not in dock
	}   
	bool changeVisibility = false;
	if (m_widgetStack->currentWidget() == widget)
	{
		changeVisibility = true;
	}
	Ideal::DiButton *button = m_buttons[widget];
	if (button)
	{
		m_bar->removeButton(button);
	}
	m_widgets.remove(button);
	m_buttons.remove(widget);
	m_widgetStack->removeWidget(widget);
    
	if (changeVisibility)
	{
		delete m_toggledButton;
		m_toggledButton = 0;
		setExpanded(false);
	}
}

void DiDockInternalWidget::selectWidget(Ideal::DiButton *button)
{
	if ( m_visible )
	{
		QWidget *parent = qobject_cast<QWidget *>(m_widgets[button]->parentWidget());
		if ( parent == 0 )
		{
			m_widgets[button]->setParent(m_widgetStack, Qt::Widget);
			m_widgetStack->addWidget(m_widgets[button]);
			m_widgetStack->setCurrentWidget(m_widgets[button]);
		}
		m_widgets[button]->show();
	}
	
	if (m_toggledButton == button)
	{
		setExpanded(!m_visible);
		return;
	}

	if (m_toggledButton)
	{
		m_toggledButton->setChecked(false);
	}
	
	m_toggledButton = button;
	setExpanded(true);

	m_widgetStack->setCurrentWidget(m_widgets[button]);
	m_widgets[button]->show();
	
}

void DiDockInternalWidget::selectWidget()
{
	Ideal::DiButton* bTmp = qobject_cast<Ideal::DiButton *>(sender());
	if  (bTmp )
	{
		selectWidget(bTmp);
	}
}

void DiDockInternalWidget::hideWidget(QWidget *widget)
{
	Ideal::DiButton *button = m_buttons[widget];
	if (button != 0)
	{
		button->setChecked(false);
		button->hide();
	}
	widget->hide();
	if (button == m_toggledButton)
	{
		setExpanded(false);
	}
}

void DiDockInternalWidget::showWidget(QWidget *widget)
{
	Ideal::DiButton *button = m_buttons[widget];
	if (button != 0)
	{
		button->show();
	}
	widget->show();
}


void DiDockInternalWidget::shrink()
{
	QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(QApplication::activeWindow());
	if ( !mainWindow )
	{
		D_FUNCINFO << "Fatal error";
		return;
	}
	
	mainWindow->setUpdatesEnabled(false);
	
	bool hmt = mainWindow->hasMouseTracking();
	
	int pm = style()->pixelMetric(QStyle::PM_DockWidgetSeparatorExtent);
	
	mainWindow->setMouseTracking(true);
	
	int wOffset = 0, hOffset= 0;
	
	if ( m_position == DiDockWidget::Bottom )
	{
		wOffset = 20;
		hOffset = -(y() * 2 + pm - 1); // FIXME FIXME FIXME
	}
	else if ( m_position == DiDockWidget::Left )
	{
		wOffset = width()+(pm/2)+1;
		hOffset = height() / 2;
	}
	else if (m_position == DiDockWidget::Right )
	{
		wOffset = -(pm/2)+1;
		hOffset = height() / 2;
	}
	
	QMouseEvent press(QEvent::MouseButtonPress,
			  mapTo(mainWindow, QPoint(this->x(), this->y())) + QPoint(wOffset, hOffset),
			  Qt::LeftButton, 0, 0);
	
	SHOW_VAR( mapTo(mainWindow, QPoint(this->x(), this->y())) + QPoint(wOffset, hOffset) );
	
	if ( ! QApplication::sendEvent(mainWindow, &press) )
	{
		qWarning("Fail pressing");
	}
	
	qApp->processEvents();
	
	int df = 0;
	int x1 = 0, x2= 0, y1= 0, y2= 0, xRelease= 0, yRelease= 0;
	
	if ( m_position == DiDockWidget::Bottom )
	{
		df = m_widgetStack->height();
		x1 = press.pos().x();
		y1 = press.pos().y() + df;
		
		x2 = press.globalPos().x();
		y2 = press.globalPos().y() + df;
		
		xRelease = this->x();
		yRelease = 10;
	}
	else if ( m_position == DiDockWidget::Left )
	{
		df = m_widgetStack->width();
		x1 = press.pos().x() - df;
		y1 = press.pos().y();
		
		x2 = press.globalPos().x() - df;
		y2 = press.globalPos().y();
		
		xRelease = 10;
		yRelease = this->y();
	}
	else if (m_position == DiDockWidget::Right )
	{
		df = m_widgetStack->width();
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
	
	mainWindow->setUpdatesEnabled(true);
}


void DiDockInternalWidget::dialoged(int index)
{
	setExpanded(false);
}

