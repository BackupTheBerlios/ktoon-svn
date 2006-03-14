/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "dconfigurationdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QHeaderView>


#include "kseparator.h"

#include "ddebug.h"

DConfigurationDialog::DConfigurationDialog(QWidget *parent) : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *controlLayout = new QHBoxLayout;
// 	controlLayout->addStretch(0);
	
	m_list = new DWidgetListView;
	connect(m_list, SIGNAL(itemClicked (QTableWidgetItem *)), this, SLOT(showPageForItem(QTableWidgetItem * )));
	
	controlLayout->addWidget(m_list);
	
	m_container = new QStackedWidget;
	controlLayout->addWidget(m_container);
	
	mainLayout->addLayout(controlLayout);
	
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	
	QPushButton *applyButton = new QPushButton(tr("Apply"));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	buttonLayout->addWidget(applyButton);
	
	QPushButton *okButton = new QPushButton(tr("OK"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));
	buttonLayout->addWidget(okButton);
	
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	buttonLayout->addWidget(cancelButton);
	
	mainLayout->addWidget(new KSeparator);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setModal(true);
	
	m_buttonGroup = new QButtonGroup(this);
	connect(m_buttonGroup, SIGNAL(buttonClicked (QAbstractButton *)), this, SLOT(showPageForButton(QAbstractButton *)));
	
	m_buttonGroup->setExclusive(true);
}


DConfigurationDialog::~DConfigurationDialog()
{
}

void DConfigurationDialog::ok()
{
	accept();
}

void DConfigurationDialog::cancel()
{
	reject();
}

void DConfigurationDialog::apply()
{
}

void DConfigurationDialog::addSection(const QString &title)
{
	QTableWidgetItem *newItem = new QTableWidgetItem();
	newItem->setText(title);
	
	m_sections.insert(title, newItem);
}

void DConfigurationDialog::addSection(QWidget *info, const QString &title)
{
	QTableWidgetItem *newItem = new QTableWidgetItem();
	newItem->setText(title);
	
	m_container->addWidget(info);
	m_pages.insert(newItem, info);
	m_sections.insert(title, newItem);
}

void DConfigurationDialog::addPageToSection(QWidget *page, const QString &title, const QString &section)
{
	QTableWidgetItem *sectionItem = m_sections[section];
	
	if ( sectionItem )
	{
		QTableWidgetItem *newItem = new QTableWidgetItem();
		newItem->setText(title);
		
// 		sectionItem->addChild(newItem);
		
		m_pages.insert(newItem, page);
	
		m_container->addWidget(page);
	}
}

void DConfigurationDialog::addPageToSection(QWidget *page, const QString &title, const QIcon &icon, const QString &section)
{
	QTableWidgetItem *sectionItem = m_sections[section];
	
	if ( sectionItem )
	{
		QTableWidgetItem *newItem = new QTableWidgetItem;
		newItem->setText(title);
		newItem->setIcon(icon);
		
// 		sectionItem->addChild(newItem);
		
		m_pages.insert(newItem, page);
	
		m_container->addWidget(page);
	}
}

void DConfigurationDialog::addPage(QWidget *page, const QString &title, const QIcon &icon)
{
	DFlatButton *button = new DFlatButton(title);
	button->setIcon(icon);
	button->setMinimumHeight(90);
	QTableWidgetItem *newItem = m_list->addWidget( button );
	
	m_buttonGroup->addButton(button);
	
	m_pages.insert(newItem, page);
	m_container->addWidget(page);
	
	if ( m_list->rowCount() == 1 )
	{
		button->click();
	}
}

QWidget *DConfigurationDialog::currentPage()
{
	dDebug() << m_list->currentItem();
	return m_pages[m_list->currentItem()];
}

void DConfigurationDialog::showPageForItem(QTableWidgetItem *item )
{
	if ( item )
	{
		m_container->setCurrentWidget(m_pages[item]);
	}
}

void DConfigurationDialog::showPageForButton(QAbstractButton *button)
{
	m_list->setCurrentItem(m_list->item(button));
	showPageForItem( m_list->item(button) );
}


