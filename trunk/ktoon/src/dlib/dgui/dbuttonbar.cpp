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

DButtonBar::DButtonBar(Qt::ToolBarArea area, QWidget *parent) : QToolBar(parent)
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
	
// #if 1
// 	if ( area == Qt::BottomToolBarArea || area == Qt::TopToolBarArea )
// 	{
	
	m_separator = addAction("");
	m_separator->setEnabled(false); // Separator
	m_separator->setVisible( false );
	
// 	}
// #endif

	setupMenu();
}


DButtonBar::~DButtonBar()
{
}

void DButtonBar::setupMenu()
{
	m_menu = new QMenu(windowTitle(), this);
	
	QAction *a = 0;
	
	a = m_menu->addAction(tr("Only icons"));
	connect(a, SIGNAL(triggered()), this, SLOT(setShowOnlyIcons()));
	
	a = m_menu->addAction(tr("Only texts"));
	connect(a, SIGNAL(triggered()), this, SLOT(setShowOnlyTexts()));
	
	m_menu->addSeparator();
	
	m_exclusive = m_menu->addAction(tr("Exclusive"));
	m_exclusive->setCheckable(true);
	m_exclusive->setChecked( m_buttons.exclusive() );
	
	connect(m_exclusive, SIGNAL(triggered(bool)), this, SLOT(setExclusive( bool )));
}

void DButtonBar::addButton(DViewButton *viewButton)
{
	if ( m_buttons.checkedButton() )
	{
		m_buttons.checkedButton()->setChecked(false);
// 		static_cast<DViewButton *>(m_buttons.checkedButton())->defaultAction()->toggle();
	}
	
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
	return m_buttons.buttons().isEmpty();
}


void DButtonBar::setExclusive(bool excl)
{
	if ( sender() != m_exclusive ) // Called directly!
	{
		m_exclusive->setChecked( excl );
	}
	
	m_buttons.setExclusive( excl );
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
		if ( m_menu->exec(e->globalPos()) )
		{
			e->accept();
		}
	}
}

void DButtonBar::showSeparator(bool e)
{
	m_separator->setVisible(e);
}


