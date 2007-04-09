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
#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

#include "ktpaintareaevent.h"

struct KTPenWidget::Private
{
	DEditSpinBox *thickness;
	QComboBox *capStyle;
	QComboBox *joinStyle;
	QComboBox *style;
	QPen pen;
};


KTPenWidget::KTPenWidget(QWidget *parent) : KTModuleWidgetBase(parent), d(new Private)
{
	setWindowTitle( tr("Pen"));
	
	d->thickness = new DEditSpinBox( 3, 1, 100, 1, tr("Thickness") );
	
	connect(d->thickness, SIGNAL(valueChanged( int )), this, SLOT(setThickness(int)));
	
	addChild( d->thickness );
	
	d->style = new QComboBox();
	
	d->style->addItem(tr("No pen"), Qt::NoPen);
	d->style->addItem( tr("Solid"), Qt::SolidLine);
	d->style->addItem( tr("Dash"), Qt::DashLine );
	d->style->addItem( tr("Dot"), Qt::DotLine );
	d->style->addItem( tr("Dash dot"), Qt::DashDotLine);
	d->style->addItem( tr("Dash dot dot"), Qt::DashDotDotLine);
	
	addChild(d->style);
	
	connect(d->style, SIGNAL(currentIndexChanged( int )), this, SLOT(setStyle(int)) );
	
	d->capStyle = new QComboBox();
	
	d->capStyle->addItem( tr("Flat"), Qt::FlatCap);
	d->capStyle->addItem( tr("Square"), Qt::SquareCap);
	d->capStyle->addItem( tr("Round"), Qt::RoundCap);
	
	addChild(d->capStyle);
	connect(d->capStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setCapStyle(int)) );
	
	d->joinStyle = new QComboBox();
	
	d->joinStyle->addItem( tr("Miter"), Qt::MiterJoin );
	d->joinStyle->addItem( tr("Bevel"), Qt::BevelJoin);
	d->joinStyle->addItem( tr("Round"), Qt::RoundJoin);
	
	addChild( d->joinStyle );
	connect(d->joinStyle, SIGNAL(currentIndexChanged( int )), this, SLOT(setJoinStyle(int)) );
	
	boxLayout()->addStretch(2);
	
	setWindowIcon(QIcon(THEME_DIR+"/icons/brushes.png"));
	
	reset();
}


KTPenWidget::~KTPenWidget()
{
	delete d;
}


void KTPenWidget::setThickness(int value)
{
	d->pen.setWidth(value);
	emitPenChanged();
}

void KTPenWidget::setStyle(int s)
{
	d->pen.setStyle( Qt::PenStyle(d->style->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setJoinStyle(int s)
{
	d->pen.setJoinStyle(Qt::PenJoinStyle(d->joinStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::setCapStyle(int s )
{
	d->pen.setCapStyle(Qt::PenCapStyle(d->capStyle->itemData(s).toInt()) );
	emitPenChanged();
}

void KTPenWidget::reset()
{
	blockSignals(true);
	d->capStyle->setCurrentIndex( 2 );
	d->joinStyle->setCurrentIndex( 2 );
	setThickness( 3 );
	blockSignals(false);
	d->style->setCurrentIndex( 1 );
	
	d->pen.setColor(QColor() ); // invalid color
}

QPen KTPenWidget::pen() const
{
	return d->pen;
}

void KTPenWidget::emitPenChanged()
{
	emit penChanged( d->pen );
	
	KTPaintAreaEvent event(KTPaintAreaEvent::ChangePen, d->pen);
	emit paintAreaEventTriggered( &event );
}

