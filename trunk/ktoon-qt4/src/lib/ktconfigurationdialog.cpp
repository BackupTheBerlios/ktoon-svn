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

#include "ktconfigurationdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QTreeWidget>
#include <QHeaderView>


#include "kseparator.h"

KTConfigurationDialog::KTConfigurationDialog(QWidget *parent) : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *controlLayout = new QHBoxLayout;
// 	controlLayout->addStretch(0);
	
	m_list = new QTreeWidget;
	connect(m_list, SIGNAL(itemClicked (QTreeWidgetItem *, int )), this, SLOT(showPageForItem(QTreeWidgetItem *, int )));
	m_list->setHeaderLabels(QStringList() << "");
	m_list->header()->hide();
	m_list->setAlternatingRowColors(true);
	m_list->setUniformRowHeights (true);
	m_list->setRootIsDecorated (true);
	
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
}


KTConfigurationDialog::~KTConfigurationDialog()
{
}

void KTConfigurationDialog::ok()
{
	accept();
}

void KTConfigurationDialog::cancel()
{
	reject();
}

void KTConfigurationDialog::apply()
{
}

void KTConfigurationDialog::addSection(const QString &title)
{
	QTreeWidgetItem *newItem = new QTreeWidgetItem(m_list);
	newItem->setText(0, title);
	
	m_sections.insert(title, newItem);
}

void KTConfigurationDialog::addSection(QWidget *info, const QString &title)
{
	QTreeWidgetItem *newItem = new QTreeWidgetItem(m_list);
	newItem->setText(0, title);
	
	m_container->addWidget(info);
	m_pages.insert(newItem, info);
	m_sections.insert(title, newItem);
}

void KTConfigurationDialog::addPage(QWidget *page, const QString &title, const QString &section)
{
	QTreeWidgetItem *sectionItem = m_sections[section];
	
	if ( sectionItem )
	{
		QTreeWidgetItem *newItem = new QTreeWidgetItem();
		newItem->setText(0, title);
		
		sectionItem->addChild(newItem);
		
		m_pages.insert(newItem, page);
	
		m_container->addWidget(page);
	}
}

void KTConfigurationDialog::addPage(QWidget *page, const QString &title, const QIcon &icon, const QString &section)
{
	QTreeWidgetItem *sectionItem = m_sections[section];
	
	if ( sectionItem )
	{
		QTreeWidgetItem *newItem = new QTreeWidgetItem();
		newItem->setText(0, title);
		newItem->setIcon(0, icon );
		
		sectionItem->addChild(newItem);
		
		m_pages.insert(newItem, page);
	
		m_container->addWidget(page);
	}
}

QWidget *KTConfigurationDialog::currentPage()
{
	return m_pages[m_list->currentItem()];
}


void KTConfigurationDialog::showPageForItem(QTreeWidgetItem *item, int )
{
	if ( item )
	{
		m_container->setCurrentWidget(m_pages[item]);
	}
}


