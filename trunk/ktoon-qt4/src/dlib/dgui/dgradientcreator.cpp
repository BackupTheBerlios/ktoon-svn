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
 
#include "dgradientcreator.h"
#include "ddebug.h"
#include "dapplication.h"
#include <QBoxLayout>
#include "dvhbox.h"


DGradientCreator::DGradientCreator(QWidget *parent)
 : QFrame(parent)
{
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->setSpacing(2);
	layout->setMargin(2);
	setLayout(layout);
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	QBoxLayout *selectorAndViewer = new QBoxLayout(QBoxLayout::TopToBottom);
	
	m_selector = new DGradientSelector(this);
	m_viewer = new DGradientViewer(this);
	connect(m_viewer, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	layout->addLayout(subLayout);
	
	selectorAndViewer->addWidget(m_viewer);
	selectorAndViewer->addWidget(m_selector);
	subLayout->addLayout(selectorAndViewer);
	
	connect( m_selector, SIGNAL(gradientChanged(  const QGradientStops& )),this, SLOT(changeGradientStops( const QGradientStops& )));
	connect(m_selector, SIGNAL(arrowAdded()), this, SIGNAL(controlArrowAdded()));
	
	m_spinControl = new SpinControl(this);
	
	connect(m_spinControl, SIGNAL(angleChanged(int)), m_viewer, SLOT(changeAngle(int)));
	connect(m_spinControl, SIGNAL(radiusChanged(int)), m_viewer, SLOT(changeRadius(int)));
	layout->addWidget(m_spinControl);
	
	
	
	m_type = new DRadioButtonGroup(tr("Gradient type"), Qt::Vertical, this);
	QStringList list;
	list << tr( "Linear" ) << tr( "Radial" ) << tr("Conical");
	m_type->addItems ( list );
	connect(  m_type, SIGNAL(  clicked ( int )),this, SLOT(changeType(int)));
	subLayout->addWidget( m_type);
	
	m_spread = new DRadioButtonGroup(tr("Gradient spread"), Qt::Vertical, this);
	list.clear();
	list << tr( "Pad" ) << tr( "Reflect" ) << tr("Repeat");
	m_spread->addItems ( list );
	connect(  m_spread, SIGNAL(  clicked ( int )),this, SLOT(changeSpread(int)));
	subLayout->addWidget( m_spread);
	
	
	subLayout->setSpacing(2);
	subLayout->setMargin(2);
	
	
	DVHBox *box = new DVHBox(this, Qt::Horizontal);
	box->setFrameStyle(QFrame::StyledPanel );
	box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	box->boxLayout()->setSpacing(2);
	box->boxLayout()->setMargin(2);
	
	m_fill = new DImageButton(QPixmap(THEME_DIR+"icons/fillcolor.png"), 32, box, false);
	connect(m_fill, SIGNAL( clicked()), this, SLOT(emitGradientChanged()));
	m_fill->setCheckable ( true );
	
	m_outLine = new DImageButton(QPixmap(THEME_DIR+"icons/written_pic.png"), 32, box, false);
	m_outLine->setCheckable ( true );
	layout->addWidget(box);

	setFrameStyle(QFrame::StyledPanel );
	
	m_spinControl->setSpin( QGradient::Type(0 ));
	m_spinControl->setRadius(50);
}


DGradientCreator::~DGradientCreator()
{
	DEND;
}




DGradientCreator::DGradientApply DGradientCreator::gradientApply()
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

void DGradientCreator::setCurrentColor(const QColor &color)
{
	
	m_selector->setCurrentColor(color);
	if(!isVisible())
	{
		m_viewer->createGradient();
	}
	emit gradientChanged(m_viewer->gradient());
}

int DGradientCreator::gradientType()
{
	return m_type->currentIndex();
}


void DGradientCreator::changeType(int type)
{
	m_viewer->changeType( type);
	
	m_spinControl->setSpin( QGradient::Type(type));
	adjustSize();
	emitGradientChanged();
}

void DGradientCreator::changeSpread(int spread)
{
	m_viewer->setSpread( spread);
	emitGradientChanged();
}


void DGradientCreator::changeGradientStops( const QGradientStops& stops )
{
	m_viewer->changeGradientStops( stops);
	emit gradientChanged(m_viewer->gradient());
}

void DGradientCreator::setGradient(const QGradient & gradient)
{
	blockSignals ( true);
	m_type->setCurrentIndex(gradient.type());
	m_spread->setCurrentIndex(gradient.spread());
	m_selector->setStops(gradient.stops());
	m_viewer->setGradient( &gradient);
	m_spinControl->setSpin( gradient.type() );
	if(gradient.type() == QGradient::RadialGradient)
	{
		m_spinControl->setRadius(  static_cast<const QRadialGradient*>(&gradient)->radius());
	}else if (gradient.type() == QGradient::ConicalGradient)
	{
		m_spinControl->setAngle(  static_cast<const QConicalGradient*>(&gradient)->angle());
	}
	blockSignals ( false);
}

void DGradientCreator::emitGradientChanged()
{
	m_viewer->changeGradientStops(m_selector->gradientStops());
	emit gradientChanged(m_viewer->gradient());
}

QBrush DGradientCreator::currentGradient()
{
	return QBrush(m_viewer->gradient());
}


QSize DGradientCreator::sizeHint () const
{
	QSize size = QFrame::sizeHint();
	
	return size.expandedTo(QApplication::globalStrut());
}

