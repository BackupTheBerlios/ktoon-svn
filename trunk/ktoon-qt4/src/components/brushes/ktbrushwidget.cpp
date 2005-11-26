/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
 
#include "ktbrushwidget.h"
#include "ktapplication.h"
#include "ktdebug.h"

#include <QToolTip>
#include <QFrame>
#include <QPainterPath>
#include <QSizePolicy>

#include <cmath>

KTBrushWidget::KTBrushWidget(QWidget *parent) : KTModuleWidgetBase( parent ), m_currentFormIndex(0)
{
	setCaption( tr( "Brushes" ) );
	
	m_layout = new QGridLayout;
	
	setupDisplay();
	setupBrushManager();
	setupButtons();
	
	static_cast<QVBoxLayout *>(layout())->addLayout(m_layout);
}


KTBrushWidget::~KTBrushWidget()
{
} 

void KTBrushWidget::setupDisplay()
{
	m_displayBrush = new KTDisplayBrush();

	m_layout->addWidget(m_displayBrush, 0,0);
}

void KTBrushWidget::setupBrushManager()
{
	m_brushManager = new KTTabWidget();
	
	QFrame *container = new QFrame(m_brushManager);
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::TopToBottom, container );
	
	m_displayThickness = new KTEditSpinBox(5, 1, 99,1, tr("Thickness"), container,"Display Thickness");
	layoutContainer->addWidget(m_displayThickness);
	connect( m_displayThickness, SIGNAL( valueChanged( int ) ), this , SLOT( changeValueMinThickness(int) ) );
	
	m_displaySmoothness = new KTEditSpinBox(2,  1, 99,1, tr("Smoothness"), container, "Display Smoothness");
	layoutContainer->addWidget(m_displaySmoothness);
	
	
	QGroupBox *box = new QGroupBox(this);
	QBoxLayout *layoutBox = new QBoxLayout(QBoxLayout::TopToBottom, box);
	layoutBox->setSpacing(1);
	layoutBox->setMargin(2);
	
	
	box->setTitle(tr( "Brush Name" ));
	m_nameBrush = new QLineEdit( tr( "Brush" ),  box);
	layoutBox->addWidget(m_nameBrush);
	
	QPushButton *editForm = new QPushButton(tr("Edit Form"),container );
	layoutContainer->addWidget(box);
	layoutContainer->addWidget(editForm);
	
	createDefaultBrushes();
	
	m_customBrushesList = new KTBrushesList(m_brushManager);
	m_brushManager->addTab (m_defaultBrushesList, tr("Default Brushes") );
	m_brushManager->addTab (m_customBrushesList, tr("Custom Brushes") );
	
	m_brushManager->addTab ( container, tr("Edit Brush") );
// 	addChild(m_brushManager, Qt::AlignTop);
	
	m_layout->addWidget(m_brushManager, 1,0);
}

void KTBrushWidget::setupButtons()
{
	QGroupBox *containerButtons = new QGroupBox();
	QHBoxLayout *layout = new QHBoxLayout(containerButtons);
	layout->setMargin(0);
	m_addBrush = new KTImageButton( QPixmap( KTOON_HOME+"/themes/default/icons/plussign.png" ), 22, containerButtons);
	connect( m_addBrush, SIGNAL( clicked() ), SLOT( addBrush() ) );
	QToolTip::add( m_addBrush, tr( "Add Brush" ) );
	layout->addWidget(m_addBrush, Qt::AlignCenter);
	
	m_removeBrush = new KTImageButton( QPixmap( KTOON_HOME+"/themes/default/icons/minussign.png" ), 22, containerButtons);
	connect( m_removeBrush, SIGNAL( clicked() ), SLOT( removeBrush() ) );
	QToolTip::add( m_removeBrush, tr( "Remove Brush" ) );
	layout->addWidget(m_removeBrush, Qt::AlignCenter);
	
	m_layout->addWidget(containerButtons, 2,0);
}


void KTBrushWidget::changeValueMinThickness(int value)
{
	m_displayBrush->setThickness( value );
	
	emit brushSelected( m_defaultBrushesList->path( m_currentFormIndex), m_displayThickness->value()  );
}


void KTBrushWidget::changeValueSmoothness(int value)
{
// 	emit smoothnessChanged(m_tableBrushes->indexCurrentBrush(), value);
}

void KTBrushWidget::selectBrush(KTCellViewItem *item)
{
	// 	m_displayBrush->setForm( item->icon().pixmap ( 100, 100)); // Escalar el path

	if ( item )
	{
		int currentRow = m_defaultBrushesList->row(item);
		int currentColumn = m_defaultBrushesList->column(item);

		if ( currentRow >= 0 && currentColumn >= 0 )
		{
			// FIXME: no funciona para columnas!! // doesn't work for columns
			if ( currentRow < m_defaultBrushesList->count() )
			{
				m_currentFormIndex = currentRow;
				m_displayBrush->setForm( m_defaultBrushesList->path( currentRow  ));
				emit brushSelected( m_defaultBrushesList->path( m_currentFormIndex), m_displayThickness->value()  );
			}
		}
	}
}

void KTBrushWidget::createDefaultBrushes()
{
	m_defaultBrushesList = new KTBrushesList(m_brushManager);
	QPainterPath form;
	connect(m_defaultBrushesList, SIGNAL(itemClicked( KTCellViewItem * )), this,SLOT(selectBrush( KTCellViewItem * )));
	
	int thickness = 40; // FIXME
	QRect boundingRect = QRect( 0, 0, thickness , thickness);
	
	form.moveTo( boundingRect.center() );
	form.addEllipse(boundingRect);
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("normal"));
	
	form = QPainterPath();
	
	form.addRect(boundingRect);
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("square"));
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(boundingRect.center());
	
	form = QPainterPath();
	form.moveTo(0,0);
	form.lineTo(thickness , thickness);
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("line"));
	
// 	form = QPainterPath();
// 	QFont f( "Cronyx-Helvetica", 10, thickness);
// 	f.setPixelSize ( 50);
// 	form.addText( 0, 50, f, "text" );
// 	m_defaultBrushesList->addBrush( thickness, thickness, form, tr("text"));

	form = QPainterPath();
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(boundingRect.center());
	form.arcTo(boundingRect,0,180);
	form.closeSubpath();
	
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("arc"));
	
	form = QPainterPath();
	boundingRect = QRect( 0, 0, thickness , thickness);
	form.moveTo(0,0);
	form.addText(0, 0,QFont("Times", 70),"KTooN");
	
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("Text"));
	
	form = QPainterPath();
// 	boundingRect = QRect( 0, 0, thickness , thickness);
	
	form.moveTo(0,0);
	form.cubicTo(thickness*2, 0, thickness+10, thickness+10, thickness*2, thickness*2);
	
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("bezier"));
	
	form = QPainterPath();
// 	boundingRect = QRect( 0, 0, thickness*2 , thickness*2);
	form.moveTo(thickness/2, 0);
	for (int i = 1; i < 5; ++i)
	{
		form.lineTo(thickness/2 * cos(0.8 * i * 3.14159f), thickness/2 * sin(0.8 * i * 3.14159f));
	}
	form.closeSubpath();
	
	m_defaultBrushesList->addBrush( thickness, m_displaySmoothness->value(), form, tr("star"));
}


