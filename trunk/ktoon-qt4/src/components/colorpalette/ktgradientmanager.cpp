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
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	QBoxLayout *selectorAndViewer = new QBoxLayout(QBoxLayout::TopToBottom);
	
	m_selector = new KTGradientSelector(this);
	m_viewer = new KTGradientViewer(this);
	
	layout->addLayout(subLayout);
	
	selectorAndViewer->addWidget(m_viewer);
	selectorAndViewer->addWidget(m_selector);
	subLayout->addLayout(selectorAndViewer);
	
	connect( m_selector, SIGNAL(gradientChanged(  const QGradientStops& )),this, SLOT(changeGradient( const QGradientStops& )));
	
	m_controlerSpins = new ControlerSpins(this);
	
	connect(m_controlerSpins, SIGNAL(angleChanged(int)), m_viewer, SLOT(changeAngle(int)));
	connect(m_controlerSpins, SIGNAL(radiusChanged(int)), m_viewer, SLOT(changeRadius(int)));
	layout->addWidget(m_controlerSpins);
	
	
	
	m_type = new KTRadioButtonGroup(tr("Gradient type"), Qt::Vertical, this);
	QStringList list;
	list << tr( "Linear" ) << tr( "Radial" ) << tr("Conical");
	m_type->addItems ( list );
	connect(  m_type, SIGNAL(  clicked ( int )),this, SLOT(changeType(int)));
	subLayout->addWidget( m_type);
	
	m_spread = new KTRadioButtonGroup(tr("Gradient spread"), Qt::Vertical, this);
	list.clear();
	list << tr( "Pad" ) << tr( "Reflect" ) << tr("Repeat");
	m_spread->addItems ( list );
	connect(  m_spread, SIGNAL(  clicked ( int )),this, SLOT(changeSpread(int)));
	subLayout->addWidget( m_spread);
	
	
	subLayout->setSpacing(2);
	subLayout->setMargin(2);
	
	
	KTVHBox *box = new KTVHBox(this, Qt::Horizontal);
	box->setFrameStyle(QFrame::StyledPanel );
	box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	box->boxLayout()->setSpacing(2);
	box->boxLayout()->setMargin(2);
	
	m_fill = new KTImageButton(QPixmap(KTOON_THEME_DIR+"icons/fillcolor.png"), 32, box, false);
	connect(m_fill, SIGNAL( clicked()), this, SLOT(emitGradientChanged()));
	m_fill->setCheckable ( true );
	
	m_outLine = new KTImageButton(QPixmap(KTOON_THEME_DIR+"icons/written_pic.png"), 32, box, false);
	m_outLine->setCheckable ( true );
	layout->addWidget(box);

	m_viewer->changeFocal( QPointF(m_viewer->rect().center().x(), m_viewer->rect().center().y()));
	setFrameStyle(QFrame::StyledPanel );
	
	m_controlerSpins->setSpin( QGradient::Type(0 ));
	m_controlerSpins->setRadius(50);
}


KTGradientManager::~KTGradientManager()
{
	KTEND;
}


void KTGradientManager::updateGradient()
{
	
}

KTGradientManager::KTGradientApply KTGradientManager::gradientApply()
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
	return None;
	
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
	m_viewer->changeType( type);
	
	m_controlerSpins->setSpin( QGradient::Type(type));
	adjustSize();
	emitGradientChanged();
}

void KTGradientManager::changeSpread(int spread)
{
	m_viewer->setSpread( spread);
	emitGradientChanged();
}


void KTGradientManager::changeGradient( const QGradientStops& stops )
{
	m_viewer->changeGradient(stops);
	emit gradientChanged(m_viewer->gradient());
}

void KTGradientManager::setGradient(const QGradient & gradient)
{
	blockSignals ( true);
	m_type->setCurrentIndex(gradient.type());
	m_spread->setCurrentIndex(gradient.spread());
	m_selector->setStops(gradient.stops());
	m_viewer->changeGradient( gradient.stops());
	blockSignals ( false);
// 	emit gradientChanged(m_viewer->gradient());
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
