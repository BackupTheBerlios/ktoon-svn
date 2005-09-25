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

#include "ddockwindow.h"

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
#include <QLabel>

#include "buttonbar.h"
#include "button.h"

DDockWindow::DDockWindow(QWidget *parent, Position position) : QDockWidget( parent)
{
	setFeatures(QDockWidget::NoDockWidgetFeatures);
	
	layout()->setSizeConstraint( QLayout::SetMinimumSize );
	m_centralWidget = new DDockInternalWidget(this, position);

	updateGeometry();
	update();
	
	setWidget(m_centralWidget);
	m_centralWidget->show();
	
	resize(minimumSize());
	
// 	QPalette pal = QApplication::palette();
// 	pal.setColor(QPalette::Background, Qt::blue);
// 	setPalette(pal);
// 	
// 	QPalette pal2 = QApplication::palette();
// 	pal2.setColor(QPalette::Background, Qt::red);
// 	m_centralWidget->setPalette(pal2);
	
	layout()->setSpacing(0);
	layout()->setMargin(0);
}

DDockWindow::~DDockWindow() {}

void DDockWindow::addWidget(const QString &title, QWidget *widget)
{
	m_centralWidget->addWidget(title, widget);
}

// DDockInternalWidget

DDockInternalWidget::DDockInternalWidget(QWidget *parent, DDockWindow::Position position) : QWidget(parent), m_position(position), m_visible(false), m_toggledButton(0)
{
	qDebug("[Initializing DDockInternalWidget]");
	
	Ideal::Place place = Ideal::Left;
	switch (position)
	{
		case DDockWindow::Bottom:
		{
			m_name = "BottomToolWindow";
			place = Ideal::Bottom;
			m_internalLayout = new QVBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::BottomToTop);
			
			break;
		}
		case DDockWindow::Left:
		{
			m_name = "LeftToolWindow";
			place = Ideal::Left;
			m_internalLayout = new QHBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::LeftToRight);

			break;
		}
		case DDockWindow::Right:
		{
			m_name = "RightToolWindow";
			place = Ideal::Right;
			m_internalLayout = new QHBoxLayout(this);
			m_internalLayout->setDirection(QBoxLayout::RightToLeft);

			break;
		}
	}
	
	setObjectName(m_name);

	m_internalLayout->setSizeConstraint(QLayout::SetMaximumSize);
	m_internalLayout->setMargin(0);
	m_internalLayout->setSpacing(0);
	
	QSettings config;
// 	config.setPath("NewMDI", qApp->objectName());
	int mode = config.readNumEntry( SETTINGSPATH+"/UI/MDIStyle", 3 );

	Ideal::ButtonMode buttonMode = Ideal::Text;
	if (mode == 0)
		buttonMode = Ideal::Icons;
	else if (mode == 1)
		buttonMode = Ideal::Text;
	else if (mode == 3)
		buttonMode = Ideal::IconsAndText;

	m_bar = new Ideal::ButtonBar(place, buttonMode, this);
	m_internalLayout->addWidget(m_bar);
	
	m_bar->setMaximumWidth(m_bar->width());
			
	m_bar->setMinimumHeight(width());

	m_bar->show();
    
	m_widgetStack = new QStackedWidget(this);
	m_internalLayout->addWidget(m_widgetStack);
	
// 	QPalette pal2 = QApplication::palette();
// 	pal2.setColor(QPalette::Background, Qt::magenta);
// 	m_widgetStack->setPalette(pal2);
	
	switch (position)
	{
		case DDockWindow::Bottom:
		{
			m_internalLayout->setAlignment( m_bar, Qt::AlignBottom | Qt::AlignLeft );
			break;
		}
		case DDockWindow::Left:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignTop | Qt::AlignLeft );
			break;
		}
		case DDockWindow::Right:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignTop | Qt::AlignRight );
			break;
		}
	}

	setExpanded(false);
	//loadSettings(); // FIXME

	setBaseSize(m_bar->size());
	show();	
}

DDockInternalWidget::~DDockInternalWidget()
{
	qDebug("[Destroying DDockInternalWidget]");
	saveSettings();
}

void DDockInternalWidget::setExpanded(bool v)
{
	QSettings config;
	config.setPath("NewMDI", qApp->name(), QSettings::User );
	
	if (m_visible)
	{
		config.writeEntry( SETTINGSPATH+"/"+m_name+"/ViewWidth", m_position == DDockWindow::Bottom ? height() : width() );
	}
	
	m_widgetStack->setVisible(v);

#if 0
	m_internalLayout->invalidate();
	if (!m_visible)
	{
		if (m_position == DDockWindow::Bottom)
		{
			parentWidget()->setFixedHeight(m_internalLayout->sizeHint().height());
		}
		else
		{
			parentWidget()->setFixedWidth(m_internalLayout->sizeHint().width());
		}
	}
	else
	{
        	//restore widget size from the config
		int size = 0;
		if (m_position == DDockWindow::Bottom)
		{
// 			size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().height());
			size = m_internalLayout->sizeHint().height();

			parentWidget()->setFixedHeight(size);
		}
		else
		{
// 			size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().width());
			size = m_internalLayout->sizeHint().width();
			parentWidget()->setFixedWidth(size);
		}
	}
#endif
	
	m_visible = v;
}

void DDockInternalWidget::loadSettings()
{
}

void DDockInternalWidget::saveSettings()
{
	QSettings config;
	config.setPath("NewMDI", qApp->name(), QSettings::User );
    
	int invisibleWidth = 0;
   
	invisibleWidth = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth");
    
	config.removeEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth");
	config.removeEntry(SETTINGSPATH+"/"+m_name+"/ViewLastWidget");
	if (m_toggledButton && m_visible)
	{
		config.writeEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_position == DDockWindow::Bottom ? height() : width());
		config.writeEntry(SETTINGSPATH+"/"+m_name+"/ViewLastWidget", m_toggledButton->realText());
	}
	else if (invisibleWidth != 0)
	{
		config.writeEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", invisibleWidth);
	}
}

QWidget *DDockInternalWidget::currentWidget() const
{
	return m_widgetStack->currentWidget();
}

void DDockInternalWidget::addWidget(const QString &title, QWidget *widget)
{
	qDebug(QString("######Adding widget with name %1").arg(widget->name()));

	QPixmap *pm = const_cast<QPixmap*>(widget->icon());
	Ideal::Button *button;
	if (pm != 0)
	{
        //force 16pt for now
		if (pm->height() > 16)
		{
			QImage img = pm->convertToImage();
			img = img.smoothScale(16, 16);
			pm->convertFromImage(img);
		}
		button = new Ideal::Button(m_bar, title, *pm);
	}
	else
	{
		button = new Ideal::Button(m_bar, title);
	}
    
	m_widgets[button] = widget;
	m_buttons[widget] = button;
	m_bar->addButton(button);
    
	widget->setParent(m_widgetStack);
    
	m_widgetStack->addWidget(widget);
	connect(button, SIGNAL(clicked()), this, SLOT(selectWidget()));
    
    //if the widget was selected last time the dock is deleted 
    //we need to show it
    
#if 0
	QSettings config;
	config.setPath("NewMDI", qApp->name(), QSettings::User );
    
	if ( config.readEntry(SETTINGSPATH+"/"+m_name+"/ViewLastWidget") == title)
	{
		button->setOn(true);
		selectWidget(button);
	}
#endif

	widget->show();
}

void DDockInternalWidget::raiseWidget(QWidget *widget)
{
	Ideal::Button *button = m_buttons[widget];
	if ((button != 0) && (!button->isOn()))
	{
		button->setOn(true);
		selectWidget(button);
	}
}

void DDockInternalWidget::removeWidget(QWidget *widget)
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
	Ideal::Button *button = m_buttons[widget];
	if (button)
	{
		m_bar->removeButton(button);
	}
	m_widgets.remove(button);
	m_buttons.remove(widget);
	m_widgetStack->removeWidget(widget);
    
	if (changeVisibility)
	{
		m_toggledButton = 0;
		setExpanded(false);
	}
}

void DDockInternalWidget::selectWidget(Ideal::Button *button)
{
	if (m_toggledButton == button)
	{
		setExpanded(!m_visible);
		return;
	}

	if (m_toggledButton)
	{
		m_toggledButton->setOn(false);
	}
	m_toggledButton = button;
	setExpanded(true);
	m_widgetStack->setCurrentWidget(m_widgets[button]);
}

void DDockInternalWidget::selectWidget()
{
	selectWidget((Ideal::Button*)sender());
}

void DDockInternalWidget::hideWidget(QWidget *widget)
{
	Ideal::Button *button = m_buttons[widget];
	if (button != 0)
	{
		button->setOn(false);
		button->hide();
	}
	widget->hide();
	if (button == m_toggledButton)
	{
		setExpanded(false);
	}
}

void DDockInternalWidget::showWidget(QWidget *widget)
{
	Ideal::Button *button = m_buttons[widget];
	if (button != 0)
	{
		button->show();
	}
	widget->show();
}

