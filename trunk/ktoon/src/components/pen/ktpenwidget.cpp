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

#include "ktpenwidget.h"
#include <dglobal.h>
#include <ddebug.h>

#include "ktpaintareaevent.h"

KTPenWidget::KTPenWidget(QWidget *parent) : KTModuleWidgetBase(parent)
{
	setWindowTitle( tr("Pen"));
	
	m_thickness = new DEditSpinBox( 3, 1, 100, 1, tr("Thickness") );
	
	connect(m_thickness, SIGNAL(valueChanged( int )), this, SLOT(setThickness(int)));
	
	addChild( m_thickness );
	
	m_style = new QComboBox();
	
	m_style->addItem(tr("No pen"), Qt::NoPen);
	m_style->addItem( tr("Solid"), Qt::SolidLine);
	m_style->addItem( tr("Dash"), Qt::DashLine );
	m_style->addItem( tr("Dot"), Qt::DotLine );
	m_style->addItem( tr("Dash dot"), Qt::DashDotLine);
	m_style->addItem( tr("Dash dot dot"), Qt::DashDotDotLine);
	
	addChild(m_style);
	
	connect(m_style, SIGNAL(currentIndexChanged( int )), this, SLOT(setStyle(int)) );
	
	m_capStyle = new QComboBox();
	
	m_capStyle->addItem( tr("Flat"), Qt::FlatCap);
	m_capStyle->addItem( tr("Square"), Qt::SquareCap);
	m_capStyle->addItem( tr("Round"), Qt::RoundCap);
	
	addChild(m_capStyle);
	connect(m_capStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setCapStyle(int)) );
	
	m_joinStyle = new QComboBox();
	
	m_joinStyle->addItem( tr("Miter"), Qt::MiterJoin );
	m_joinStyle->addItem( tr("Bevel"), Qt::BevelJoin);
	m_joinStyle->addItem( tr("Round"), Qt::RoundJoin);
	
	addChild( m_joinStyle );
	connect(m_joinStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setJoinStyle(int)) );
	
	boxLayout()->addStretch(2);
	
	setWindowIcon(QIcon(THEME_DIR+"/icons/brushes.png"));
	
	reset();
}


KTPenWidget::~KTPenWidget()
{
}


void KTPenWidget::setThickness(int value)
{
	m_pen.setWidth(value);
	emitPenChanged();
}

void KTPenWidget::setStyle(int s)
{
	m_pen.setStyle( Qt::PenStyle(m_style->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setJoinStyle(int s)
{
	m_pen.setJoinStyle(Qt::PenJoinStyle(m_joinStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setCapStyle(int s )
{
	m_pen.setCapStyle(Qt::PenCapStyle(m_capStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::reset()
{
	blockSignals(true);
	m_capStyle->setCurrentIndex( 2 );
	m_joinStyle->setCurrentIndex( 2 );
	setThickness( 3 );
	blockSignals(false);
	m_style->setCurrentIndex( 1 );
	
	m_pen.setColor(QColor() ); // invalid color
}

QPen KTPenWidget::pen() const
{
	return m_pen;
}

void KTPenWidget::emitPenChanged()
{
	emit penChanged( m_pen );
	
	KTPaintAreaEvent event(KTPaintAreaEvent::ChangePen, m_pen);
	emit paintAreaEventTriggered( &event );
}

