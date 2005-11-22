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

KTColorPalette::KTColorPalette(QWidget *parent) : KTModuleWidgetBase(parent), m_currentOutlineColor(Qt::black), m_currentFillColor(Qt::transparent), m_lastIndex(0)
{
	KTINIT;
	setCaption( tr( "Color Palette" ) );
	
	createIcon();

	m_centralWidget = new QToolBox(this);
	connect(m_centralWidget, SIGNAL(currentChanged(int)), this, SLOT(changeIcon(int)));
	
	m_centralWidget->setFrameStyle(QFrame::StyledPanel );
	
	addChild(m_centralWidget);
	
	m_containerPalette = new KTViewColorCells(m_centralWidget);
	m_centralWidget->addItem( m_containerPalette, m_icon, tr("Palettes") );
	connect (m_containerPalette, SIGNAL(selectColor( const QColor& )), this, SLOT(setColor( const QColor& )));
	
	QFrame *viewColor = new QFrame(this);;
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	viewColor->setLayout(layout);
	m_outlineAndFillColors = new KTDualColorButton(m_currentOutlineColor,m_currentFillColor, viewColor);
	m_outlineAndFillColors->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
	connect( m_outlineAndFillColors, SIGNAL(currentChanged(KTDualColorButton::DualColor)),this, SLOT(changeTypeColor(KTDualColorButton::DualColor)));
	layout->addWidget( m_outlineAndFillColors);
	
	m_centralWidget->setPalette(palette());
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::TopToBottom);
	layoutName->addWidget(new QLabel( tr("<b>HTML</b>"), viewColor));
	m_nameColor = new QLineEdit(viewColor);
	m_nameColor->setMaxLength ( 7 );
// 	m_nameColor->
	QFontMetrics fm( font() );
	m_nameColor->setMaximumWidth( fm.width ( " #000000 " ) );
	
	connect( m_nameColor, SIGNAL(editingFinished()), this, SLOT(updateColor()));
	layoutName->addWidget(m_nameColor);
	
	layout->addLayout(layoutName);
	setupChooserTypeColor();
	setupChooserGradient();
	

	addChild(viewColor);
	setColor(m_currentOutlineColor);
	setupButtons();
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
	KTImageButton *m_addColor = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/plussign.png" ) , 22, containerButtons);
	
	connect( m_addColor, SIGNAL( clicked() ), SLOT( addColor() ) );
	QToolTip::add( m_addColor, tr( "Add Color" ) );
	layout->addWidget(m_addColor, Qt::AlignCenter);
	
	KTImageButton *m_removeColor = new KTImageButton( QPixmap( KTOON_HOME+"/themes/default/icons/minussign.png"), 22, containerButtons);
	
	connect( m_removeColor, SIGNAL( clicked() ), SLOT( removeColor() ) );
	QToolTip::add( m_removeColor, tr( "Remove Color" ) );
	
	layout->addWidget(m_removeColor, Qt::AlignCenter);
	
	KTImageButton *m_addPalette = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/plussign.png" ), 22, containerButtons);
	connect( m_addPalette, SIGNAL( clicked() ), SLOT( addPalette() ) );
	QToolTip::add( m_addPalette, tr( "Add Custom Palette" ) );
	
	layout->addWidget(m_addPalette, Qt::AlignCenter);
	
	KTImageButton *m_removePalette = new KTImageButton(QPixmap( KTOON_HOME+"/themes/default/icons/minussign.png"), 22, containerButtons);
	
	connect( m_removePalette, SIGNAL( clicked() ), SLOT( removeColor() ) );
	QToolTip::add( m_removePalette, tr( "Remove Custom Palette" ) );
	containerButtons->setMaximumHeight(m_removePalette->height());
	layout->addWidget(m_removePalette, Qt::AlignCenter);
	addChild(containerButtons);
}

void KTColorPalette::setupChooserTypeColor()
{
	
	QFrame *colorMixer = new QFrame(this);
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	colorMixer->setLayout(layout);

	layout->setSizeConstraint(QLayout::SetFixedSize);
	
	m_displayValueColor = new KTValueColor(colorMixer);
	
	layout->addWidget(m_displayValueColor);
	
	
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::LeftToRight);
	
	m_colorPicker = new KTColorPicker(colorMixer);
	connect( m_colorPicker, SIGNAL(newCol(int, int)), this, SLOT(setHS(int, int)));
	connect(m_displayValueColor, SIGNAL(hueChanged(int)), m_colorPicker, SLOT(setH(int)));
	connect(m_displayValueColor, SIGNAL(saturationChanged(int)), m_colorPicker, SLOT(setS(int)));
	
	layoutContainer->addWidget(m_colorPicker);
	
	m_luminancePicker = new KTLuminancePicker( colorMixer );
	connect( m_luminancePicker, SIGNAL( newHsv(int, int, int )), this, SLOT (syncHsv(int, int, int)));

	m_luminancePicker->setMaximumWidth(15);
	m_luminancePicker->setMinimumWidth(15);
	connect(m_displayValueColor, SIGNAL(valueChanged(int)), m_luminancePicker, SLOT(setVal( int )));
	layoutContainer->addWidget(m_luminancePicker);
	layoutContainer->setSpacing(3);
// 
	layout->addLayout(layoutContainer);
	addChild( colorMixer );
	this->layout()->setAlignment( colorMixer, Qt::AlignTop);
	connect(m_displayValueColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(setColor(const QColor &)));
// 	setColor(m_currentOutlineColor);
// 	
	m_centralWidget->addItem(colorMixer, m_icon, tr("Color Mixer"));
	
}

void KTColorPalette::setupChooserGradient()
{
	KTVHBox *blockGradient = new KTVHBox(this);
	blockGradient->layout()->setSizeConstraint(QLayout::SetFixedSize);
	
	QStringList list;
	list << tr( "None" ) << tr( "Linear" ) << tr( "Radial" );
	m_gradientTypes = new QComboBox( blockGradient );
	m_gradientTypes -> insertStringList( list );
	QBoxLayout *displayGradient = new QBoxLayout( QBoxLayout::LeftToRight);
	
	m_gradientViewer = new GradientViewer( this );
	displayGradient->addWidget(m_gradientViewer);
	
	m_gradient = new KTGradientSelector( this );
	displayGradient->addWidget(m_gradient);
	blockGradient->boxLayout()->addLayout(displayGradient);
	addChild(blockGradient);

	m_centralWidget->addItem(blockGradient, m_icon, tr("Gradients"));
}

void KTColorPalette::setColor(const QColor& color)
{
	ktDebug() << "KTColorPalette::setColor " << color; 
	if(m_displayValueColor && m_outlineAndFillColors && m_colorPicker && m_nameColor && m_luminancePicker)
	{
		m_displayValueColor->setColor(color);
		m_outlineAndFillColors->setCurrentColor(color);
		m_colorPicker->setCol(color.hue(), color.saturation ());
	
		m_nameColor->setText(color.name ());
	
		m_luminancePicker->setCol(color.hue(), color.saturation(), color.value());
		
		
		emit colorChanged( m_outlineAndFillColors->foreground(),m_outlineAndFillColors->background() );
	}
}

void KTColorPalette::changeTypeColor(KTDualColorButton::DualColor s)
{
	ktDebug() << "KTColorPalette::changeTypeColor";
	if(s == KTDualColorButton::Background)
	{
		m_outlineAndFillColors->setCurrent( s);
		setColor( m_outlineAndFillColors->background());
	}
	else
	{
		m_outlineAndFillColors->setCurrent( s);
		setColor( m_outlineAndFillColors->foreground());
	}
}

void KTColorPalette::syncHsv(int h, int s, int v)
{
	int th, ts, tv;
	QColor tmpColor = m_outlineAndFillColors->currentColor();
	tmpColor.setHsv( h, s, v, tmpColor.alpha() );
	if(m_luminancePicker == sender())
		setColor(tmpColor);
}


void KTColorPalette::setHS(int h , int s)
{
	int th, ts, tv;
	QColor tmpColor = m_outlineAndFillColors->currentColor();
	
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

QPair<QColor, QColor> KTColorPalette::color()
{
	QPair<QColor, QColor> colors;
	colors.first = m_outlineAndFillColors->foreground();
	colors.first = m_outlineAndFillColors->background();
	
	return colors;
}


