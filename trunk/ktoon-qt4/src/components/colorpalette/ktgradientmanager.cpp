/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "ktgradientmanager.h"
#include "ktdebug.h"
#include <QBoxLayout>


KTGradientManager::KTGradientManager(QWidget *parent)
 : QFrame(parent)
{
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->setSpacing(2);
	layout->setMargin(2);
	setLayout(layout);
	
	m_viewer = new KTGradientViewer(this);
	m_viewer->setMinimumSize(100,100);
	layout->addWidget(m_viewer);
	m_selector = new KTGradientSelector(this);
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	subLayout->addWidget(m_selector);
	connect( m_selector, SIGNAL(gradientChanged(  const QGradientStops& )),this, SLOT(changeGradient( const QGradientStops& )));


	m_type = new QComboBox(this);
	QStringList list;
	list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" ) << tr("Conical");
	m_type->addItems ( list );
	connect(  m_type, SIGNAL(  activated ( int )),this, SLOT(changeType(int)));
	subLayout->addWidget( m_type);
	layout->addLayout(subLayout);
// 	m_focal = new KTXYSpinBox(tr("focal") );
// 	layout->addWidget(m_focal);
// 	connect( m_focal, SIGNAL(valueXYChanged(double, double)), m_viewer,SLOT( changeFocal(double, double)));
	m_viewer->changeFocal( m_viewer->rect().center().x(), m_viewer->rect().center().y());
}


KTGradientManager::~KTGradientManager()
{
}


void KTGradientManager::updateGradient()
{
	
}


void KTGradientManager::setColor(const QColor &color)
{
	
	m_selector->setColor(color);
// 	SHOW_VAR(isVisible());
	if(!isVisible())
	{
		m_viewer->createGradient();
	}
	emit gradientChanged(m_viewer->gradient());
}

int KTGradientManager::gradientType()
{
	return m_type->currentIndex();
}


void KTGradientManager::changeType(int type)
{
	SHOW_VAR(type);
	switch(type)
	{
		case 0:
		{
			m_selector->setMaxRow( 1);
			break;
		}
		case 1:
		case 2:
		case 3:
		{
			m_selector->setMaxRow( 10);
			m_viewer->changeType( type-1);
			break;
		}
	}
	
}

void KTGradientManager::changeGradient( const QGradientStops& stops )
{
	m_viewer->changeGradient(stops);
	emit gradientChanged(m_viewer->gradient());
}

QBrush KTGradientManager::currentGradient()
{
	return QBrush(m_viewer->gradient());
}

