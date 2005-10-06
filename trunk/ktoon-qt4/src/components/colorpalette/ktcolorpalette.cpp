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
 
#include "ktcolorpalette.h"
#include "ktdebug.h"



#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QToolTip>

#include "ktimagebutton.h"
#include "ktapplication.h"
#include "colordisplay.h"

KTColorPalette::KTColorPalette(QWidget *parent)
	: KTModuleWidgetBase(parent), m_currentOutlineColor(Qt::black), m_currentFillColor(Qt::white)
{
	KTINIT;
	setCaption( tr( "Color Palette" ) );
	layout()->setSpacing(0);
	m_containerPalette = new KTViewColorCells(this);
	addChild( m_containerPalette); //, Qt::AlignTop );
	layout()->setAlignment(m_containerPalette, Qt::AlignTop);
	setupButtons();

	setupChooserTypeColor();
	setupChooserGradient();

// 	setColor(QColor(10,10,10));
	
}


KTColorPalette::~KTColorPalette()
{
	KTEND;
}

void KTColorPalette::setupButtons()
{
	QGroupBox *containerButtons = new QGroupBox(this);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, containerButtons);
	layout->setMargin(0);
	layout->setSpacing(0);
	KTImageButton *m_addColor = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/plussign.xpm" ) , 22, containerButtons);
	
	connect( m_addColor, SIGNAL( clicked() ), SLOT( addColor() ) );
	QToolTip::add( m_addColor, tr( "Add Color" ) );
	layout->addWidget(m_addColor, Qt::AlignCenter);
	
	KTImageButton *m_removeColor = new KTImageButton( QPixmap( KTOON_HOME+"/images/icons/minussign.xpm"), 22, containerButtons);
	
	connect( m_removeColor, SIGNAL( clicked() ), SLOT( removeColor() ) );
	QToolTip::add( m_removeColor, tr( "Remove Color" ) );
	
	layout->addWidget(m_removeColor, Qt::AlignCenter);
	
	KTImageButton *m_addPalette = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/plussign.xpm" ), 22, containerButtons);
	connect( m_addPalette, SIGNAL( clicked() ), SLOT( addPalette() ) );
	QToolTip::add( m_addPalette, tr( "Add Custom Palette" ) );
	
	layout->addWidget(m_addPalette, Qt::AlignCenter);
	
	KTImageButton *m_removePalette = new KTImageButton(QPixmap( KTOON_HOME+"/images/icons/minussign.xpm"), 22, containerButtons);
	
	connect( m_removePalette, SIGNAL( clicked() ), SLOT( removeColor() ) );
	QToolTip::add( m_removePalette, tr( "Remove Custom Palette" ) );
	containerButtons->setMaximumHeight(m_removePalette->height());
	layout->addWidget(m_removePalette, Qt::AlignCenter);
	addChild(containerButtons);
}

void KTColorPalette::setupChooserTypeColor()
{
	QGroupBox *chooserTypeColor = new QGroupBox(this);
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	chooserTypeColor->setLayout(layout);
	
	m_displayValueColor = new KTValueColor(chooserTypeColor);
	layout->addWidget(m_displayValueColor);

	
	QFrame *container = new QFrame(chooserTypeColor);
	
	QBoxLayout *layoutContiner = new QBoxLayout(QBoxLayout::TopToBottom);
	container->setLayout(layoutContiner);
	
	QBoxLayout *layoutFillOutLine = new  QBoxLayout(QBoxLayout::LeftToRight);
	
	m_fillColor = new FillColor(QPixmap( KTOON_HOME+"/images/icons/fillcolor.xpm" ), container);
	connect(m_fillColor, SIGNAL( activated()) , this, SLOT(changeTypeColor()));
	
	layoutFillOutLine->addWidget(m_fillColor);
	m_fillColor->slotSetColor(m_currentFillColor);
	
	layoutContiner->addLayout(layoutFillOutLine);
	
	m_outlineColor = new FillColor(QPixmap( KTOON_HOME+"/images/icons/written_pic.xpm" ), container);
	connect(m_outlineColor, SIGNAL( activated()) , this, SLOT(changeTypeColor()));
	
	m_outlineColor->setActive( true );
	layoutFillOutLine->addWidget(m_outlineColor);
	m_outlineColor->slotSetColor(m_currentOutlineColor);
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::LeftToRight);
	layoutName->addWidget(new QLabel("HTML", container));
	nameColor = new QLineEdit(container);
	
	layoutName->addWidget(nameColor);
	layoutContiner->addLayout(layoutName);
	
	layout->addWidget(container);
	
	QBoxLayout * layoutColorPicker= new QBoxLayout(QBoxLayout::LeftToRight);

	m_colorPicker = new KTColorPicker(chooserTypeColor);
	
	connect(m_displayValueColor, SIGNAL(hueChanged(int)), m_colorPicker, SLOT(setH(int)));
	
	connect(m_displayValueColor, SIGNAL(saturationChanged(int)), m_colorPicker, SLOT(setS(int)));
	
	layoutColorPicker->addWidget(m_colorPicker);
	ValueSelector *selector = new ValueSelector( chooserTypeColor );
	connect(m_displayValueColor, SIGNAL(valueChanged(int)), selector, SLOT(slotSetValue( int )));
	
	layoutColorPicker->addWidget(selector);
	layout->addLayout(layoutColorPicker);
	addChild( chooserTypeColor );
	this->layout()->setAlignment( chooserTypeColor, Qt::AlignTop);
	connect(m_displayValueColor, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
	setColor(m_currentOutlineColor);
	
}

void KTColorPalette::setupChooserGradient()
{
	KTVHBox *blockGradient = new KTVHBox(this);
	
	QStringList list;
	list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" );
	m_gradientTypes = new QComboBox( blockGradient );
	m_gradientTypes -> insertStringList( list );
	QBoxLayout *displayGradient = new QBoxLayout(QBoxLayout::LeftToRight);
	
	m_gradientViewer = new GradientViewer( blockGradient );
	displayGradient->addWidget(m_gradientViewer);
	
	m_gradient = new ColorGradientSelector( blockGradient );
	displayGradient->addWidget(m_gradient);
	blockGradient->boxLayout()->addLayout(displayGradient);
	addChild(blockGradient);

}

void KTColorPalette::setColor(QColor color)
{
// 	ktDebug() << "aki" << endl;
	if(m_displayValueColor != sender())
	{
		m_displayValueColor->setColor(color);
	}
// 	if(
// 		m_colorPicker->setCol(color.hue(), color.saturation ());
	
	nameColor->setText(color.name ());
	if(m_outlineColor->isActive())
	{
		m_outlineColor->slotSetColor(color);
		m_currentOutlineColor = color;
	}
	else
	{
		m_currentFillColor = color;
		m_fillColor->slotSetColor(color);
	}
}

void KTColorPalette::changeTypeColor()
{
	if(m_outlineColor == sender())
	{
		m_fillColor->setActive(false);
		m_outlineColor->setActive(true);
		setColor(m_outlineColor->currentColor());
	}
	else
	{
		m_outlineColor->setActive(false);
		m_fillColor ->setActive(true);
		setColor(m_fillColor->currentColor());
	}
	
	
	
}
