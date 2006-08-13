/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "dbuttonbar.h"
#include "dviewbutton.h"

#include <QToolButton>
#include <QBoxLayout>
#include <QAction>
#include <QCheckBox>
#include <QMenu>
#include <QMouseEvent>

#include <QtDebug>

DButtonBar::DButtonBar(Qt::ToolBarArea area, QWidget *parent) : QToolBar(parent), m_autoHide(false), m_blockHider(false)
{
	setMovable(false);
	
	setIconSize(QSize(16,16));
	
	m_buttons.setExclusive( true );
	
	switch(area)
	{
		case Qt::LeftToolBarArea:
		{
			setWindowTitle(tr("Left button bar"));
		}
		break;
		case Qt::RightToolBarArea:
		{
			setWindowTitle(tr("Right button bar"));
		}
		break;
		case Qt::BottomToolBarArea:
		{
			setWindowTitle(tr("Bottom button bar"));
		}
		break;
		case Qt::TopToolBarArea:
		{
			setWindowTitle(tr("Top button bar"));
		}
		break;
		default: break;
	}
	
	
	setObjectName("DButtonBar-"+windowTitle());
	
	m_separator = addAction("");
	m_separator->setEnabled(false); // Separator
	m_separator->setVisible( false );
	
	connect( &m_hider, SIGNAL(timeout()), this, SLOT(hide()));
}


DButtonBar::~DButtonBar()
{
}

QMenu *DButtonBar::createMenu()
{
	QMenu *menu = new QMenu(windowTitle(), this);
	
	QAction *a = 0;
	
	a = menu->addAction(tr("Only icons"));
	connect(a, SIGNAL(triggered()), this, SLOT(setShowOnlyIcons()));
	
	a = menu->addAction(tr("Only texts"));
	connect(a, SIGNAL(triggered()), this, SLOT(setShowOnlyTexts()));
	
	menu->addSeparator();
	
	a = menu->addAction(tr("Exclusive"));
	a->setCheckable(true);
	a->setChecked( isExclusive() );
	
	connect(a, SIGNAL(triggered(bool)), this, SLOT(setExclusive( bool )));
	
	a = menu->addAction(tr("Auto hide"));
	a->setCheckable(true);
	a->setChecked( autohide() );
	
	connect(a, SIGNAL(triggered(bool)), this, SLOT(setAutoHide( bool )));
	
	
	return menu;
}

void DButtonBar::addButton(DViewButton *viewButton)
{
	bool isExclusive = m_buttons.exclusive();
	m_buttons.setExclusive(false);
	
	if ( m_buttons.checkedButton() )
	{
		m_buttons.checkedButton()->setChecked( false );
	}
	
	m_buttons.setExclusive( isExclusive );
	
	QAction *act = addWidget(viewButton);
	
	m_buttons.addButton(viewButton);
	
	m_actionForWidget[viewButton] = act;
	act->setVisible(true);
	if ( !isVisible() ) show();
	
	connect(viewButton, SIGNAL(clicked()), this, SLOT(hideOthers()));
}

void DButtonBar::removeButton(DViewButton *viewButton)
{
	if ( ! m_buttons.buttons().contains(viewButton) ) return;
	
	m_buttons.removeButton(viewButton);
	removeAction( m_actionForWidget[viewButton] );
	viewButton->setParent(0);
	
	disconnect(viewButton, SIGNAL(clicked()), this, SLOT(hideOthers()));
	
	if ( isEmpty() ) hide();
}

bool DButtonBar::isEmpty() const
{
	// O(n) -> very slow...
	
	bool isEmpty = true;
	
	foreach(QAbstractButton *button, m_buttons.buttons() )
	{
		isEmpty = isEmpty && button->isHidden();
		
		if ( ! isEmpty ) break;
	}
	
	return isEmpty;
}


void DButtonBar::setExclusive(bool excl)
{
	m_buttons.setExclusive( excl );
}

void DButtonBar::setAutoHide(bool autohide)
{
	m_autoHide = autohide;
	if (autohide)
	{
		hide();
	}
}

bool DButtonBar::autohide() const
{
	return m_autoHide;
}

void DButtonBar::setShowOnlyIcons()
{
	foreach(QAbstractButton *b, m_buttons.buttons() )
	{
		DViewButton *viewButton = static_cast<DViewButton *>(b);
		viewButton->setOnlyIcon();
	}
}

void DButtonBar::setShowOnlyTexts()
{
	foreach(QAbstractButton *b, m_buttons.buttons() )
	{
		DViewButton *viewButton = static_cast<DViewButton *>(b);
		viewButton->setOnlyText();
	}
}

void DButtonBar::disable(DViewButton *v)
{
	QAction *a = m_actionForWidget[v];
	if ( a )
	{
		a->setVisible(false);
	}
}

void DButtonBar::enable(DViewButton *v)
{
	QAction *a = m_actionForWidget[v];
	if ( a )
	{
		a->setVisible(true);
	}
}

bool DButtonBar::isExclusive() const
{
	return m_buttons.exclusive();
}

void DButtonBar::hideOthers()
{
	if ( !m_buttons.exclusive() ) return;
	
	QAbstractButton *source = static_cast<QAbstractButton *>(sender());
	
	setUpdatesEnabled( false );
	foreach(QAbstractButton *button, m_buttons.buttons())
	{
		if ( source != button )
		{
			if ( button->isChecked() )
			{
				button->blockSignals(true);
				button->click();
				button->blockSignals(false);
				break;
			}
		}
	}
	
	setUpdatesEnabled( true);
}


void DButtonBar::mousePressEvent(QMouseEvent *e)
{
	QToolBar::mousePressEvent(e);
	
	if ( e->button() == Qt::RightButton )
	{
		m_blockHider = true;
		createMenu()->exec(e->globalPos());
		e->accept();
		
		m_blockHider = false;
	}
}

void DButtonBar::enterEvent(QEvent *e)
{
	QToolBar::enterEvent(e);
	doNotHide();
}

void DButtonBar::leaveEvent(QEvent *e)
{
	QToolBar::leaveEvent(e);
	
	if ( m_autoHide && !m_hider.isActive() && !m_blockHider )
	{
		m_hider.start(800);
	}
}

void DButtonBar::doNotHide()
{
	if ( m_hider.isActive() )
	{
		m_hider.stop();
	}
}

void DButtonBar::showSeparator(bool e)
{
	m_separator->setVisible(e);
}

int DButtonBar::count() const
{
	return m_buttons.buttons().count();
}

void DButtonBar::setEnableButtonBlending(bool enable)
{
	foreach(QAbstractButton *button, m_buttons.buttons() )
	{
		static_cast<DViewButton *>(button)->setBlending(enable);
	}
}
