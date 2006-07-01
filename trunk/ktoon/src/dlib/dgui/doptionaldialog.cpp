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

#include "doptionaldialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include "dseparator.h"

DOptionalDialog::DOptionalDialog(const QString &text,const QString &title,QWidget *parent) : QDialog(parent)
{
	setWindowTitle(title);
	m_layout = new QVBoxLayout;
	
	m_layout->addStretch(10);
	
	QLabel *label = new QLabel(text, this);
	
	m_layout->addWidget(label);
	
	m_layout->addStretch(10);
	
	m_layout->addWidget(new DSeparator);
	
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	
	m_checkBox = new QCheckBox(tr("Don't show again"));
	buttonLayout->addWidget(m_checkBox);
	
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	buttonLayout->addWidget(cancelButton);
	
	QPushButton *okButton = new QPushButton(tr("Accept"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	buttonLayout->addWidget(okButton);
	
	m_layout->addLayout(buttonLayout);
	setLayout(m_layout);
}


DOptionalDialog::~DOptionalDialog()
{
}

bool DOptionalDialog::shownAgain()
{
	return m_checkBox->isChecked();
}

