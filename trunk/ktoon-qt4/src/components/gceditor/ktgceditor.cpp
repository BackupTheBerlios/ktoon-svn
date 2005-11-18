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
#include "ktgceditor.h"
#include <QVBoxLayout>

KTGCEditor::KTGCEditor(QWidget *parent) : KTModuleWidgetBase(parent)
{
	setCaption( tr("Graphic Component Editor") );

	m_angle = new KTEditSpinBox(0, 0, 360, 1, tr("Rotate"));
	connect(m_angle, SIGNAL(valueChanged(int)), this, SIGNAL(requestRotate(int)));
	addChild(m_angle, Qt::AlignTop);
	
	QHBoxLayout *propLayout = new QHBoxLayout;
	
	m_scale = new KTXYSpinBox(tr("Scale"));
	connect(m_scale, SIGNAL(valueXYChanged(double,double)), this, SIGNAL(requestScale( double,double)));
	
	m_scale->setSingleStep(0.1);
	m_scale->setMaximum(100);
	propLayout->addWidget(m_scale);
	
	m_shear = new KTXYSpinBox(tr("Shear"));
	connect(m_shear, SIGNAL(valueXYChanged(double,double)), this, SIGNAL(requestShear( double,double)));
	propLayout->addWidget(m_shear);
	
	m_shear->setSingleStep(0.1);
	m_shear->setMaximum(100);
	m_shear->setMinimum(-100);
	
	
	m_translate = new KTXYSpinBox(tr("Translate"));
	connect(m_translate, SIGNAL(valueXYChanged(double,double)), this, SIGNAL(requestTranslate( double,double)));
	propLayout->addWidget(m_translate);
	
	m_translate->setMaximum(1000);
	
	static_cast<QVBoxLayout *>(layout())->addLayout(propLayout);
}


KTGCEditor::~KTGCEditor()
{
}


