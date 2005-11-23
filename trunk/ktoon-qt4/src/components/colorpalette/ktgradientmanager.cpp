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
#include <QGridLayout>


KTGradientManager::KTGradientManager(QWidget *parent)
 : QFrame(parent)
{
	QGridLayout *layout = new QGridLayout();
	layout->setSpacing(2);
	setLayout(layout);
	
	m_viewer = new KTGradientViewer(this);
	layout->addWidget(m_viewer, 0, 0);
	m_selector = new KTGradientSelector(this);
	layout->addWidget(m_selector, 0, 1);
	connect( m_selector, SIGNAL(gradientChanged(  const QGradientStops& )),m_viewer, SLOT(changeGradient( const QGradientStops& )));
	
	m_selector->addArrow( QPoint(0,0), QColor( 0,0,0 ) );
	m_type = new QComboBox(this);
	QStringList list;
	list /*<< tr( "None" )*/ << tr( "Linear" ) << tr( "Radial" ) << tr("Conical");
	m_type->addItems ( list );
	connect(  m_type, SIGNAL( highlighted (int)),this, SLOT(changeType(int)));
	layout->addWidget( m_type, 1 ,0);
	
	
	m_focal = new KTXYSpinBox(tr("focal") );
	layout->addWidget(m_focal, 2, 0);
	connect( m_focal, SIGNAL(valueXYChanged(double, double)), m_viewer,SLOT( changeFocal(double, double)));
}


KTGradientManager::~KTGradientManager()
{
}

void KTGradientManager::setColor(const QColor &color)
{
	m_selector->setColor(color);
}

void KTGradientManager::changeType(int type)
{
	SHOW_VAR(type);
	m_viewer->changeType( type);
}
