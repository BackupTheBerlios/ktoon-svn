/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#include "kttabdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

KTTabDialog::KTTabDialog(QWidget *parent, bool modal)
 : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;

	m_tabWidget = new QTabWidget(this);
	
	
	QPushButton *applyButton = new QPushButton(tr("Apply"));
	QPushButton *okButton = new QPushButton(tr("OK"));
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(applyButton);
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	mainLayout->addWidget(m_tabWidget);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
	
	setModal(modal);
}


KTTabDialog::~KTTabDialog()
{
}

void KTTabDialog::addTab ( QWidget * child, const QString & label )
{
	m_tabWidget->addTab(child, label);
}

void KTTabDialog::addTab ( QWidget * child, const QIcon & iconset, const QString & label )
{
	m_tabWidget->addTab(child, iconset, label);
}

QWidget *KTTabDialog::currentTab()
{
	return m_tabWidget->currentWidget();
}

void KTTabDialog::ok()
{
	accept();
}

void KTTabDialog::cancel()
{
	reject();
}

void KTTabDialog::apply()
{
}

