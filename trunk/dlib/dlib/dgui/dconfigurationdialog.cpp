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

#include "dconfigurationdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QHeaderView>


#include "dseparator.h"

#include "ddebug.h"

DConfigurationDialog::DConfigurationDialog(QWidget *parent) : DPageDialog(parent)
{
	setFaceType( DPageDialog::Tree );

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
	
	static_cast<QBoxLayout *>(layout())->addLayout(buttonLayout);
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
