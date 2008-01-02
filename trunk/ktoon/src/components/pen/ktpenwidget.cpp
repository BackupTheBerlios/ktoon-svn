/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#include "ktpaintareaevent.h"

struct KTPenWidget::Private
{
	KEditSpinBox *thickness;
	QComboBox *capStyle;
	QComboBox *joinStyle;
	QComboBox *style;
	QPen pen;
};

KTPenWidget::KTPenWidget(QWidget *parent) : KTModuleWidgetBase(parent), k(new Private)
{
	setWindowTitle( tr("Pen"));
	
	k->thickness = new KEditSpinBox( 3, 1, 100, 1, tr("Thickness") );
	
	connect(k->thickness, SIGNAL(valueChanged( int )), this, SLOT(setThickness(int)));
	
	addChild( k->thickness );
	
	k->style = new QComboBox();
	
	k->style->addItem(tr("No pen"), Qt::NoPen);
	k->style->addItem( tr("Solid"), Qt::SolidLine);
	k->style->addItem( tr("Dash"), Qt::DashLine );
	k->style->addItem( tr("Dot"), Qt::DotLine );
	k->style->addItem( tr("Dash dot"), Qt::DashDotLine);
	k->style->addItem( tr("Dash dot dot"), Qt::DashDotDotLine);
	
	addChild(k->style);
	
	connect(k->style, SIGNAL(currentIndexChanged( int )), this, SLOT(setStyle(int)) );
	
	k->capStyle = new QComboBox();
	
	k->capStyle->addItem( tr("Flat"), Qt::FlatCap);
	k->capStyle->addItem( tr("Square"), Qt::SquareCap);
	k->capStyle->addItem( tr("Round"), Qt::RoundCap);
	
	addChild(k->capStyle);
	connect(k->capStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setCapStyle(int)) );
	
	k->joinStyle = new QComboBox();
	
	k->joinStyle->addItem( tr("Miter"), Qt::MiterJoin );
	k->joinStyle->addItem( tr("Bevel"), Qt::BevelJoin);
	k->joinStyle->addItem( tr("Round"), Qt::RoundJoin);
	
	addChild( k->joinStyle );
	connect(k->joinStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setJoinStyle(int)) );
	
	boxLayout()->addStretch(2);
	
	setWindowIcon(QIcon(THEME_DIR+"/icons/brushes.png"));
	
	reset();
}


KTPenWidget::~KTPenWidget()
{
	delete k;
}


void KTPenWidget::setThickness(int value)
{
	k->pen.setWidth(value);
	emitPenChanged();
}

void KTPenWidget::setStyle(int s)
{
	k->pen.setStyle( Qt::PenStyle(k->style->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setJoinStyle(int s)
{
	k->pen.setJoinStyle(Qt::PenJoinStyle(k->joinStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setCapStyle(int s )
{
	k->pen.setCapStyle(Qt::PenCapStyle(k->capStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::reset()
{
	blockSignals(true);
	k->capStyle->setCurrentIndex( 2 );
	k->joinStyle->setCurrentIndex( 2 );
	setThickness( 3 );
	blockSignals(false);
	k->style->setCurrentIndex( 1 );
	
	k->pen.setColor(QColor() ); // invalid color
}

QPen KTPenWidget::pen() const
{
	return k->pen;
}

void KTPenWidget::emitPenChanged()
{
	emit penChanged( k->pen );
	
	KTPaintAreaEvent event(KTPaintAreaEvent::ChangePen, k->pen);
	emit paintAreaEventTriggered( &event );
}

