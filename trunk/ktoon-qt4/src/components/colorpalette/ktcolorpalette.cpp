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

#include "ktgradientselector.h"

KTColorPalette::KTColorPalette(QWidget *parent) : KTModuleWidgetBase(parent), m_currentOutlineColor(Qt::black), m_currentFillColor(Qt::white), m_lastIndex(0)
{
	KTINIT;
	setCaption( tr( "Color Palette" ) );
	
	createIcon();
	
	m_centralWidget = new QToolBox(this);
	connect(m_centralWidget, SIGNAL(currentChanged(int)), this, SLOT(changeIcon(int)));
	
	m_centralWidget->setFrameStyle(QFrame::StyledPanel );
	
	addChild(m_centralWidget);
	
// 	layout()->setSpacing(0);
	
	m_containerPalette = new KTViewColorCells(m_centralWidget);
	m_centralWidget->addItem( m_containerPalette, m_icon, tr("Palettes") );
// 	layout()->setAlignment(m_containerPalette, Qt::AlignTop);
	setupButtons();
	
	setupChooserTypeColor();
	setupChooserGradient();
	
	m_centralWidget->setPalette(palette());
	
}


KTColorPalette::~KTColorPalette()
{
	KTEND;
}

void KTColorPalette::createIcon()
{
	QPolygon m_pArrow(3);

	QPixmap pixmap (22,22);
	pixmap.fill( palette().background() );
	
	m_pArrow.setPoint ( 0, 0, 0);
	m_pArrow.setPoint ( 1, 10, 5);
	m_pArrow.setPoint ( 2, 0, 10);
	
	QPainter p(&pixmap);
	
	p.setBrush( palette().foreground() );
	p.translate(pixmap.width()/3, 5);
	p.drawConvexPolygon(m_pArrow);

	p.end();
	
	m_icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
	
	///////////
	
	QPixmap pixmap2 (22,22);
	pixmap2.fill( palette().background() );
	
	m_pArrow.setPoint ( 0, 0, 0);
	m_pArrow.setPoint ( 1, 5, 10);
	m_pArrow.setPoint ( 2, 10, 0);
	
	QPainter p2(&pixmap2);
	
	p2.setBrush( palette().foreground() );
	p2.translate(pixmap2.width()/3, 5);
	p2.drawConvexPolygon(m_pArrow);

	p2.end();
	
	m_icon.addPixmap(pixmap2, QIcon::Normal, QIcon::On);
	
}

void KTColorPalette::changeIcon(int index)
{
	m_centralWidget->setItemIcon(m_lastIndex, m_icon.pixmap (QSize(22,22), QIcon::Normal, QIcon::Off));
	
	m_centralWidget->setItemIcon(index, m_icon.pixmap (QSize(22,22), QIcon::Normal, QIcon::On));
	
	m_lastIndex = index;
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
	QFrame *chooserTypeColor = new QFrame(this);
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	chooserTypeColor->setLayout(layout);
	
	layout->setSizeConstraint(QLayout::SetFixedSize);
	
	m_displayValueColor = new KTValueColor(chooserTypeColor);
	layout->addWidget(m_displayValueColor);
	
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::LeftToRight);
	
	QFrame *left = new QFrame(chooserTypeColor);
	QBoxLayout *layoutLeft = new QBoxLayout(QBoxLayout::TopToBottom);
	left->setLayout(layoutLeft);
	
	m_outlineAndFillColors = new KTDualColorButton(m_currentOutlineColor,m_currentFillColor, left);
	layoutLeft->addWidget(m_outlineAndFillColors);
	
// 	m_fillColor = new FillColor(QPixmap( KTOON_HOME+"/images/icons/fillcolor.xpm" ), left);
// 	connect(m_fillColor, SIGNAL( activated()) , this, SLOT(changeTypeColor()));
// 	
// 	layoutFillOutLine->addWidget(m_fillColor);
// 	m_fillColor->slotSetColor(m_currentFillColor);
	
// 	layoutLeft->addLayout(layoutFillOutLine);
// 	
// 	m_outlineColor = new FillColor(QPixmap( KTOON_HOME+"/images/icons/written_pic.xpm" ), left);
// 	connect(m_outlineColor, SIGNAL( activated()) , this, SLOT(changeTypeColor()));
	
// 	m_outlineColor->setActive( true );
// 	layoutFillOutLine->addWidget(m_outlineColor);
// 	m_outlineColor->slotSetColor(m_currentOutlineColor);
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::TopToBottom);
	layoutName->addWidget(new QLabel( tr("<b>HTML</b>"), left));
	m_nameColor = new QLineEdit(left);
	connect( m_nameColor, SIGNAL(editingFinished()), this, SLOT(updateColor()));
	
	layoutName->addWidget(m_nameColor);
	layoutLeft->addLayout(layoutName);
	
// 	layout->addWidget(left);
	layoutContainer->addWidget(left);
	
// 	QBoxLayout * layoutColorPicker= new QBoxLayout(QBoxLayout::LeftToRight);
	m_colorPicker = new KTColorPicker(chooserTypeColor);
	connect( m_colorPicker, SIGNAL(newCol(int, int)), this, SLOT(setHS(int, int)));
	connect(m_displayValueColor, SIGNAL(hueChanged(int)), m_colorPicker, SLOT(setH(int)));
	connect(m_displayValueColor, SIGNAL(saturationChanged(int)), m_colorPicker, SLOT(setS(int)));
	layoutContainer->addWidget(m_colorPicker);

// 	layoutColorPicker->addWidget(m_colorPicker);
	m_luminancePicker = new KTLuminancePicker( chooserTypeColor );
	connect( m_luminancePicker, SIGNAL( newHsv(int, int, int )), this, SLOT (syncHsv(int, int, int)));

	m_luminancePicker->setMaximumWidth(15);

	m_luminancePicker->setMinimumWidth(15);
	
	connect(m_displayValueColor, SIGNAL(valueChanged(int)), m_luminancePicker, SLOT(setVal( int )));
	layoutContainer->addWidget(m_luminancePicker);
// 	layoutColorPicker->addWidget(m_luminancePicker);
	layoutContainer->setSpacing(3);
	
	layout->addLayout(layoutContainer);
	addChild( chooserTypeColor );
	this->layout()->setAlignment( chooserTypeColor, Qt::AlignTop);
	connect(m_displayValueColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(setColor(const QColor &)));
	setColor(m_currentOutlineColor);
	
	m_centralWidget->addItem(chooserTypeColor, m_icon, tr("Color Mixer"));
	
}

void KTColorPalette::setupChooserGradient()
{
	KTVHBox *blockGradient = new KTVHBox(this);
	blockGradient->layout()->setSizeConstraint(QLayout::SetFixedSize);
	
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

	m_centralWidget->addItem(blockGradient, m_icon, tr("Gradients"));
}

void KTColorPalette::setColor(const QColor& color)
{
	m_outlineAndFillColors->setCurrentColor(color);
// 	if( m_outlineColor->isActive() )
// 	{
// 		m_outlineColor->slotSetColor(color);
// 		m_currentOutlineColor = color;
// 	}
// 	else
// 	{
// 		m_fillColor->slotSetColor(color);
// 		m_currentFillColor = color;
// 	}
	
	if(m_displayValueColor != sender())
	{
		m_displayValueColor->setColor(color);
	}
	
	m_colorPicker->setCol(color.hue(), color.saturation ());
	m_nameColor->setText(color.name ());
	m_luminancePicker->setCol(color.hue(), color.saturation(), color.value());
	
	emit colorChanged(color);
}

void KTColorPalette::changeTypeColor()
{
// 	if(m_outlineColor == sender())
// 	{
// 		m_fillColor->setActive(false);
// 		m_outlineColor->setActive(true);
// 		setColor(m_outlineColor->currentColor());
// 	}
// 	else
// 	{
// 		m_outlineColor->setActive(false);
// 		m_fillColor ->setActive(true);
// 		setColor(m_fillColor->currentColor());
// 	}
}

void KTColorPalette::syncHsv(int h, int s, int v)
{
	int th, ts, tv;
	QColor tmpColor = m_outlineAndFillColors->currentColor();
// 	if(m_outlineColor->isActive())
// 	{
// 		tmpColor = m_currentOutlineColor;
// 	}
// 	else
// 	{
// 		tmpColor = m_currentFillColor;
// 	}
	tmpColor.setHsv( h, s, v, tmpColor.alpha() );
	setColor(tmpColor);
}


void KTColorPalette::setHS(int h , int s)
{
	int th, ts, tv;
	QColor tmpColor = m_outlineAndFillColors->currentColor();

// 	if(m_outlineColor->isActive())
// 	{
// 		tmpColor = m_currentOutlineColor;
// 	}
// 	else
// 	{
// 		tmpColor = m_currentFillColor;
// 	}
	
	tmpColor.getHsv( &th, &ts, &tv );
	tmpColor.setHsv( h, s, tv, tmpColor.alpha() );
	setColor(tmpColor);
}

void KTColorPalette::updateColor()
{
	QColor tmpColor(m_nameColor->text());
	tmpColor.setAlpha(m_displayValueColor->alpha());
	setColor(tmpColor);
}