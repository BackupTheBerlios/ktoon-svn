/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktdrawingareaproperties.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <dcolorbutton.h>
#include <dconfig.h>

KTDrawingAreaProperties::KTDrawingAreaProperties(QWidget *parent) : QDialog(parent)
{
	setModal(true);
	
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	
	setupPage();
	
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	
	QPushButton *okButton = new QPushButton(tr("Accept"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	buttonLayout->addWidget(okButton);
	
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject ()));
	buttonLayout->addWidget(cancelButton);
	
	mainLayout->addLayout(buttonLayout);
	
}

KTDrawingAreaProperties::~KTDrawingAreaProperties()
{
}


void KTDrawingAreaProperties::setupPage()
{
	
	QFrame *page = new QFrame;
	QGridLayout *pageLayout = new QGridLayout(page);
	page->setFrameStyle(QFrame::Box | QFrame::Sunken );
	
	DCONFIG->beginGroup("PaintArea");
	
	pageLayout->addWidget(new QLabel(tr("Grid color")), 0, 0);
	m_gridColor = new DColorButton;
	pageLayout->addWidget(m_gridColor, 0, 1);
	
	m_gridColor->setColor( qvariant_cast<QColor>(DCONFIG->value("GridColor", QColor(Qt::gray))));
	
	
	pageLayout->addWidget(new QLabel(tr("Grid separation")), 0, 0);
	m_gridSeparation = new QSpinBox(this);
	
	pageLayout->addWidget(m_gridSeparation, 0, 1);
	
	m_gridSeparation->setMaximum(100);
	m_gridSeparation->setMinimum(10);
	m_gridSeparation->setValue(10);
	
	
	pageLayout->addWidget(new QLabel(tr("Background color")), 1, 0);
	m_backgroundColor = new DColorButton;
	pageLayout->addWidget(m_backgroundColor, 1, 1);
	
	m_backgroundColor->setColor(qvariant_cast<QColor>(DCONFIG->value("BackgroundColor", QColor(Qt::white))));
	
	pageLayout->addWidget(new QLabel(tr("Onion skin color")), 2, 0);
	m_onionSkinColor = new DColorButton;
	pageLayout->addWidget(m_onionSkinColor, 2, 1);
	
	m_onionSkinColor->setColor(qvariant_cast<QColor>(DCONFIG->value("OnionSkinColor", QColor(Qt::lightGray))));
	
	pageLayout->addWidget(new QLabel(tr("Onion skin background ")), 3, 0);
	m_onionSkinBackground = new DColorButton;
	pageLayout->addWidget(m_onionSkinBackground, 3, 1);
	
	m_onionSkinBackground->setColor(qvariant_cast<QColor>(DCONFIG->value("OnionSkinBackground", QColor(Qt::lightGray))));
	
	layout()->addWidget(page);
}

QColor KTDrawingAreaProperties::gridColor() const
{
	return m_gridColor->color();
}

QColor KTDrawingAreaProperties::backgroundColor() const
{
	return m_backgroundColor->color();
}

QColor KTDrawingAreaProperties::onionSkinColor() const
{
	return m_onionSkinColor->color();
}

QColor KTDrawingAreaProperties::onionSkinBackground() const
{
	return m_onionSkinBackground->color();
}

int KTDrawingAreaProperties::gridSeparation() const
{
	return m_gridSeparation->value();
}
