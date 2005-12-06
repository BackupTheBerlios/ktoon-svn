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
#include "ktapplication.h"
#include <QBoxLayout>
#include "ktvhbox.h"


KTGradientManager::KTGradientManager(QWidget *parent)
 : QFrame(parent)
{
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->setSpacing(2);
	layout->setMargin(2);
	setLayout(layout);
	
	m_viewer = new KTGradientViewer(this);
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addLayout(subLayout);
	subLayout->addWidget(m_viewer);

	m_selector = new KTGradientSelector(this);
	layout->addWidget(m_selector);
	connect( m_selector, SIGNAL(gradientChanged(  const QGradientStops& )),this, SLOT(changeGradient( const QGradientStops& )));
	m_type = new QComboBox(this); //cambiar por una clase que liste las opciones con radiobuttons
	QStringList list;
	list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" ) << tr("Conical");
	m_type->addItems ( list );
	connect(  m_type, SIGNAL(  activated ( int )),this, SLOT(changeType(int)));
	subLayout->addWidget( m_type,0, Qt::AlignTop);
	
	subLayout->setSpacing(0);
	subLayout->setMargin(0);
	subLayout->setSizeConstraint(QLayout::SetMinimumSize);
	
	
	KTVHBox *box = new KTVHBox(this, Qt::Horizontal);
	box->setFrameStyle(QFrame::StyledPanel );
	box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	box->boxLayout()->setSpacing(0);
	box->boxLayout()->setMargin(2);
	
	m_fill = new KTImageButton(QPixmap(KTOON_THEME_DIR+"icons/fillcolor.png"), 32, box, false);
	connect(m_fill, SIGNAL( clicked()), this, SLOT(emitGradientChanged()));
	m_fill->setCheckable ( true );
	
	m_outLine = new KTImageButton(QPixmap(KTOON_THEME_DIR+"icons/written_pic.png"), 32, box, false);
	m_outLine->setCheckable ( true );
	layout->addWidget(box);

// 	m_focal = new KTXYSpinBox(tr("focal") );
// 	layout->addWidget(m_focal);
// 	connect( m_focal, SIGNAL(valueXYChanged(double, double)), m_viewer,SLOT( changeFocal(double, double)));
	m_viewer->changeFocal( m_viewer->rect().center().x(), m_viewer->rect().center().y());
	setFrameStyle(QFrame::StyledPanel );
// 	ktDebug().resaltWidget(this);
}


KTGradientManager::~KTGradientManager()
{
	KTEND;
}


void KTGradientManager::updateGradient()
{
	
}

int KTGradientManager::gradientApply()
{
	if( m_fill->isChecked() && m_outLine->isChecked())
	{
		return FillAndOutLine;
	}
	else if(m_fill->isChecked())
	{
		return Fill;
	}
	else if(m_outLine->isChecked())
	{
		return OutLine;
	}
	else
	{
		return None;
	}
}

void KTGradientManager::setColor(const QColor &color)
{
	
	m_selector->setColor(color);
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
	emitGradientChanged();
	
}

void KTGradientManager::changeGradient( const QGradientStops& stops )
{
	m_viewer->changeGradient(stops);
	emit gradientChanged(m_viewer->gradient());
}

void KTGradientManager::emitGradientChanged()
{
	m_viewer->changeGradient(m_selector->gradient().stops());
	emit gradientChanged(m_viewer->gradient());
}

QBrush KTGradientManager::currentGradient()
{
	return QBrush(m_viewer->gradient());
}

