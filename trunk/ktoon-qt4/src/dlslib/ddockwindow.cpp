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

DDockWindow::DDockWindow(QWidget *parent, Position position) : QDockWidget( parent), m_fixedSize(-1,-1)
{
	setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_centralWidget = new DDockInternalWidget(this, position);
	m_centralWidget->show();
	
	connect(m_centralWidget, SIGNAL(fixedExtentHeight(int)), this, SLOT(setFixedExtendHeight(int)));
	connect(m_centralWidget, SIGNAL(fixedExtentWidth(int)), this, SLOT(setFixedExtentWidth(int)));
	
	setWidget(m_centralWidget);
	
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

void DDockWindow::setFixedExtentHeight(int h)
{
// 	qDebug(QString("seFixedExtentHeigth: %1").arg(h));
	m_fixedSize.setHeight(h);
}

void DDockWindow::setFixedExtentWidth(int w)
{
// 	qDebug(QString("seFixedExtentWidth: %1").arg(w));
	m_fixedSize.setWidth(w);
}

void DDockWindow::addWidget(const QString &title, QWidget *widget)
{
	m_centralWidget->addWidget(title, widget);
}

void DDockWindow::resizeEvent(QResizeEvent *e)
{
// 	qDebug(QString("DDockWindow resized to w: %1 h: %2 de %3").arg(e->size().width()).arg(e->size().height()).arg(m_centralWidget->name()));
	QDockWidget::resizeEvent(e);
}

QSize DDockWindow::sizeHint() const
{
	QSize sh(QDockWidget::sizeHint());
	
	sh = sh.expandedTo(fixedExtent());
	
// 	qDebug(QString(">>>\nWidth: %1 Height: %2\n<<<").arg(sh.width()).arg(sh.height()));

	if (m_centralWidget)
	{
		if (m_centralWidget->position() == Bottom )
		{
			sh.setWidth(sh.width() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
		}
		else if (m_centralWidget->position() == Right || m_centralWidget->position() == Left)
		{
			sh.setHeight(sh.height() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
		}
	}
	
// 	qDebug(QString("retornando sh: w: %1 h: %2").arg(sh.width()).arg(sh.height()));
	
	return sh;
}

// QSize DDockWindow::minimumSize() const
// {
// 	QSize ms(QDockWidget::minimumSize());
// 
// 	ms = ms.expandedTo(fixedExtent());
// 
// 	if (m_centralWidget)
// 	{
// 		if (m_centralWidget->position() == Bottom)
// 		{
// 			ms.setWidth(ms.width() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
// 		}
// 		else if (m_centralWidget->position() == Right || m_centralWidget->position() == Left)
// 		{
// 			ms.setHeight(ms.height() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
// 		}
// 	}
// 	return ms;
// }
// 
// QSize DDockWindow::minimumSizeHint() const
// {
// 	QSize msh(QDockWidget::minimumSize());
// 
// 	msh = msh.expandedTo(fixedExtent());
// 
// 	if (m_centralWidget)
// 	{
// 		if (m_centralWidget->position() == Bottom)
// 		{
// 			msh.setWidth(msh.width() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
// 		}
// 		else if (m_centralWidget->position() == Right || m_centralWidget->position() == Left)
// 		{
// 			msh.setHeight(msh.height() + 2 * style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this) / 3);
// 		}
// 	}
// 	
// 	qDebug(QString("retornando minsh: w: %1 h: %2").arg(msh.width()).arg(msh.height()));
// 	
// 	return msh;
// }

QSize DDockWindow::fixedExtent() const
{
// 	qDebug(QString("retornando fe: w: %1 h: %2 de %3").arg(m_fixedSize.width()).arg(m_fixedSize.height()).arg(m_centralWidget->name()));
	return m_fixedSize;
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
			m_internalLayout->setAlignment( m_bar,Qt::AlignBottom );
			break;
		}
		case DDockWindow::Left:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignLeft );
			break;
		}
		case DDockWindow::Right:
		{
			m_internalLayout->setAlignment( m_bar,Qt::AlignRight );
			break;
		}
	}

	loadSettings();

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
	m_visible = v;

	m_internalLayout->invalidate();
	parentWidget()->layout()->invalidate();
	
	if (!m_visible)
	{
		if (m_position == DDockWindow::Bottom)
		{
			emit fixedExtentHeight(m_internalLayout->sizeHint().height());
		}
		else
		{
// 			qDebug("HEREEEEEEEE");
			emit fixedExtentWidth(m_internalLayout->sizeHint().width());
		}
	}
	else
	{
        //restore widget size from the config
		int size = 0;
		if (m_position == DDockWindow::Bottom)
		{
			size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().height());
			emit fixedExtentHeight(size);
		}
		else
		{
			size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().width());
			emit fixedExtentWidth(size);
		}
	}
	
	layout()->invalidate();
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
    
	QSettings config;
	config.setPath("NewMDI", qApp->name(), QSettings::User );
    
	if ( config.readEntry(SETTINGSPATH+"/"+m_name+"/ViewLastWidget") == title)
	{
		button->setOn(true);
		selectWidget(button);
	}
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

