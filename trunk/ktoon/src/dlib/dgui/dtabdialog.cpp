/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dtabdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

DTabDialog::DTabDialog(QWidget *parent, bool modal)
 : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;

	m_tabWidget = new DTabWidget(this);
	
	mainLayout->addWidget(m_tabWidget);
	setLayout(mainLayout);
	
	setupButtons(Ok|Cancel);
	
	setModal(modal);
}

DTabDialog::DTabDialog(int buttons, QWidget *parent, bool modal)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	
	m_tabWidget = new DTabWidget(this);
	
	mainLayout->addWidget(m_tabWidget);
	
	setupButtons(buttons);
	
	setModal(modal);
}

void DTabDialog::setupButtons(int buttons)
{
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	
	if ( buttons & Custom1)
	{
		QPushButton *customButton = new QPushButton(tr(""));
		connect(customButton, SIGNAL(clicked()), this, SLOT(custom1()));
		buttonLayout->addWidget(customButton);
		m_buttons.insert( Custom1, customButton);
	}
	
	if ( buttons & Custom2)
	{
		QPushButton *customButton = new QPushButton(tr(""));
		connect(customButton, SIGNAL(clicked()), this, SLOT(custom2()));
		buttonLayout->addWidget(customButton);
		m_buttons.insert( Custom2, customButton);
	}
	
	if ( buttons & Custom3)
	{
		QPushButton *customButton = new QPushButton(tr(""));
		connect(customButton, SIGNAL(clicked()), this, SLOT(custom3()));
		buttonLayout->addWidget(customButton);
		m_buttons.insert( Custom3, customButton);
	}
	
	if ( buttons & Help )
	{
		QPushButton *helpButton = new QPushButton(tr("Help"));
		connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
		buttonLayout->addWidget(helpButton);
		m_buttons.insert( Help, helpButton);
	}
	
	if ( buttons & Apply )
	{
		QPushButton *applyButton = new QPushButton(tr("Apply"));
		connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
		buttonLayout->addWidget(applyButton);
		m_buttons.insert( Apply, applyButton);
	}
	
	if ( buttons & Ok )
	{
		QPushButton *okButton = new QPushButton(tr("OK"));
		connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));
		buttonLayout->addWidget(okButton);
		m_buttons.insert( Ok, okButton);
	}
	
	if ( buttons & Cancel)
	{
		QPushButton *cancelButton = new QPushButton(tr("Cancel"));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
		buttonLayout->addWidget(cancelButton);
		m_buttons.insert( Cancel, cancelButton);
	}
	
	static_cast<QVBoxLayout *>(layout())->addLayout(buttonLayout);
}


DTabDialog::~DTabDialog()
{
}

void DTabDialog::addTab ( QWidget * child, const QString & label )
{
	m_tabWidget->addTab(child, label);
}

void DTabDialog::addTab ( QWidget * child, const QIcon & iconset, const QString & label )
{
	m_tabWidget->addTab(child, iconset, label);
}

QWidget *DTabDialog::currentTab()
{
	return m_tabWidget->currentWidget();
}

void DTabDialog::ok()
{
	accept();
}

void DTabDialog::cancel()
{
	reject();
}

void DTabDialog::apply()
{
}

void DTabDialog::setButtonText(Button b, const QString &text)
{
	QPushButton *button = m_buttons[b];
	if ( button )
	{
		button->setText(text);
	}
}

QPushButton *DTabDialog::button(Button b)
{
	return m_buttons[b];
}


DTabWidget *DTabDialog::tabWidget() const
{
	return m_tabWidget;
}


