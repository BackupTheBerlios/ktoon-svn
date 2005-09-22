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

DDockWindow::DDockWindow(QWidget *parent, Position position)
	:Q3DockWindow(parent), m_position(position), m_visible(false), m_toggledButton(0)
{
    setMovingEnabled(false);
    setResizeEnabled(true);
	
// 	setFeatures();

    Ideal::Place place = Ideal::Left;
    switch (position) {
        case DDockWindow::Bottom:
            m_name = "BottomToolWindow";
            place = Ideal::Bottom;
	    m_internalLayout = new QVBoxLayout(layout(), 0);
            m_internalLayout->setDirection(QBoxLayout::BottomToTop);
            break;
        case DDockWindow::Left:
            m_name = "LeftToolWindow";
            place = Ideal::Left;
	    m_internalLayout = new QHBoxLayout(layout(), 0);
            m_internalLayout->setDirection(QBoxLayout::LeftToRight);
            break;
        case DDockWindow::Right:
            m_name = "RightToolWindow";
            place = Ideal::Right;
	    m_internalLayout = new QHBoxLayout(layout(), 0);
            m_internalLayout->setDirection(QBoxLayout::RightToLeft);
            break;
    }
    
    m_internalLayout->setAutoAdd(true);
    
    QSettings config;
    config.setPath("NewMDI", qApp->name(), QSettings::User );
    int mode = config.readNumEntry( SETTINGSPATH+"/UI/MDIStyle", 3 );


    Ideal::ButtonMode buttonMode = Ideal::Text;
    if (mode == 0)
        buttonMode = Ideal::Icons;
    else if (mode == 1)
        buttonMode = Ideal::Text;
    else if (mode == 3)
        buttonMode = Ideal::IconsAndText;

    m_bar = new Ideal::ButtonBar(place, buttonMode, this);
//     m_internalLayout->addWidget(m_bar);
    
    QPalette pal = QApplication::palette();
    pal.setColor(QPalette::Background, Qt::green);
    m_bar->setPalette(pal);
    m_bar->show();
    
    m_widgetStack = new QStackedWidget(this);
    setWidget(m_widgetStack);
//     m_internalLayout->addWidget(m_widgetStack);
    
    m_widgetStack->setVisible(true);
    setVisible(m_visible);
    
    loadSettings();
}

DDockWindow::~DDockWindow()
{
    saveSettings();
}

void DDockWindow::setVisible(bool v)
{
    QSettings config;
    config.setPath("NewMDI", qApp->name(), QSettings::User );
    
    if (m_visible)
    {
        config.writeEntry( SETTINGSPATH+"/"+m_name+"/ViewWidth", m_position == DDockWindow::Bottom ? height() : width() );
    }

    v ? m_widgetStack->show() : m_widgetStack->hide();
    m_visible = v;
    
    m_internalLayout->invalidate();
    if (!m_visible)
    {
        if (m_position == DDockWindow::Bottom)
        {
            setFixedExtentHeight(m_internalLayout->sizeHint().height());
        }
        else
        {
            setFixedExtentWidth(m_internalLayout->sizeHint().width());
        }
    }
    else
    {
        //restore widget size from the config
        int size = 0;
        if (m_position == DDockWindow::Bottom)
        {
            size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().height());

            setFixedExtentHeight(size);
        }
        else
        {

            size = config.readNumEntry(SETTINGSPATH+"/"+m_name+"/ViewWidth", m_internalLayout->sizeHint().width());

            setFixedExtentWidth(size);
        }
    }
}

void DDockWindow::loadSettings()
{
}

void DDockWindow::saveSettings()
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

QWidget *DDockWindow::currentWidget() const
{
    return m_widgetStack->currentWidget();
}

void DDockWindow::addWidget(const QString &title, QWidget *widget)
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

void DDockWindow::raiseWidget(QWidget *widget)
{
    Ideal::Button *button = m_buttons[widget];
    if ((button != 0) && (!button->isOn()))
    {
        button->setOn(true);
        selectWidget(button);
    }
}

void DDockWindow::removeWidget(QWidget *widget)
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
        setVisible(false);
    }
}

void DDockWindow::selectWidget(Ideal::Button *button)
{
    if (m_toggledButton == button)
    {
        setVisible(!m_visible);
        return;
    }

    if (m_toggledButton)
    {
        m_toggledButton->setOn(false);
    }
    m_toggledButton = button;
    setVisible(true);
    m_widgetStack->setCurrentWidget(m_widgets[button]);
}

void DDockWindow::selectWidget()
{
    selectWidget((Ideal::Button*)sender());
}

void DDockWindow::hideWidget(QWidget *widget)
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
        setVisible(false);
    }
}

void DDockWindow::showWidget(QWidget *widget)
{
    Ideal::Button *button = m_buttons[widget];
    if (button != 0)
    {
        button->show();
    }
    widget->show();
}

void DDockWindow::setMovingEnabled(bool)
{
    //some operations on KMainWindow cause moving to be enabled
    //but we always don't want DDockWindow instances to be movable
//     QDockWidget::setMovingEnabled(false);
}

