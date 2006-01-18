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


KTColorPalette::KTColorPalette(QWidget *parent) : KTModuleWidgetBase(parent), m_currentOutlineColor(Qt::black), m_currentFillColor(Qt::transparent), m_flagGradient(true)
{
	KTINIT;
	setCaption( tr( "Color Palette" ) );

	m_splitter = new QSplitter(Qt::Vertical, this);
	addChild( m_splitter);
	
	m_centralWidget = new KTToolBox(m_splitter);

	setupChooserTypeColor();
	setupGradienManager();
	setupDisplayColor();
	m_splitter->addWidget(m_centralWidget);
	m_centralWidget->setPalette(palette());
	
	KTCONFIG->beginGroup("ColorPalette");
	QColor foreground = QColor(KTCONFIG->value("LastForegroundColor", Qt::black).toString());
	QColor background = QColor(KTCONFIG->value("LastBackgroundColor", Qt::transparent).toString());
	
	setColor( foreground );
}


KTColorPalette::~KTColorPalette()
{
	KTEND;
	
	KTCONFIG->beginGroup("ColorPalette");
	KTCONFIG->setValue("LastForegroundColor", color().first);
	KTCONFIG->setValue("LastBackgroundColor", color().second);
}

void KTColorPalette::setupChooserTypeColor()
{
	QFrame *colorMixer = new QFrame(m_centralWidget);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	colorMixer->setLayout(layout);

	layout->setSizeConstraint(QLayout::SetFixedSize);
	
	m_displayValueColor = new KTValueColor(colorMixer);
	
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::LeftToRight);
	m_colorPicker = new KTColorPicker(colorMixer);
	connect( m_colorPicker, SIGNAL(newCol(int, int)), this, SLOT(setHS(int, int)));
	connect(m_displayValueColor, SIGNAL(hueChanged(int)), m_colorPicker, SLOT(setH(int)));
	connect(m_displayValueColor, SIGNAL(saturationChanged(int)), m_colorPicker, SLOT(setS(int)));
	
	layoutContainer->addWidget(m_colorPicker, 0, Qt::AlignLeft);
	
	m_luminancePicker = new KTLuminancePicker( colorMixer );
	connect( m_luminancePicker, SIGNAL( newHsv(int, int, int )), this, SLOT (syncHsv(int, int, int)));

	m_luminancePicker->setMaximumWidth(15);
	m_luminancePicker->setMinimumWidth(15);
	connect(m_displayValueColor, SIGNAL(valueChanged(int)), m_luminancePicker, SLOT(setVal( int )));
	layoutContainer->addWidget(m_luminancePicker, 0, Qt::AlignLeft);
	layoutContainer->setSpacing(3);
	
	layout->addLayout(layoutContainer);
	layout->addWidget(m_displayValueColor);
	this->layout()->setAlignment( colorMixer, Qt::AlignTop);
	connect(m_displayValueColor, SIGNAL(colorChanged(const QColor&)), this, SLOT(setColor(const QColor &)));
	
	m_centralWidget->addPage(colorMixer, tr("Color Mixer"));
}


void KTColorPalette::setupGradienManager()
{
	m_gradientManager = new KTGradientManager(this);
	connect(m_gradientManager, SIGNAL(gradientChanged( const QGradient& )), this, SLOT(changeGradient(const QGradient &) ));
	m_centralWidget->addPage(m_gradientManager,tr("Gradients"));
	
}

void KTColorPalette::setupDisplayColor()
{
	
	//palettes
	m_containerPalette = new KTViewColorCells(m_splitter);
	
	connect (m_containerPalette, SIGNAL(selectColor( const QColor& )), this, SLOT(setColor( const QColor& )));
	
	connect (m_containerPalette, SIGNAL(selectGradient(const QGradient&)), this, SLOT(changeGradient(const QGradient&)));
	
	connect (m_containerPalette, SIGNAL(selectGradient(const QGradient&)), m_gradientManager, SLOT(setGradient(const QGradient&)));
	
	m_splitter->addWidget(m_containerPalette);
	
	
	//////////
	QFrame *viewColor = new QFrame(this);
	QBoxLayout *vlayout = new QBoxLayout(QBoxLayout::LeftToRight);
	viewColor->setLayout(vlayout);
	
	m_outlineAndFillColors = new KTDualColorButton(m_currentOutlineColor,m_currentFillColor, viewColor);
	m_outlineAndFillColors->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
	connect( m_outlineAndFillColors, SIGNAL(currentChanged(KTDualColorButton::DualColor)),this, SLOT(changeTypeColor(KTDualColorButton::DualColor)));
	vlayout->addWidget( m_outlineAndFillColors);
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::TopToBottom);
	layoutName->addWidget(new QLabel( tr("<b>HTML</b>"), viewColor));
	m_nameColor = new QLineEdit(viewColor);
	QFontMetrics fm( font() );
	m_nameColor->setMaximumWidth( fm.width ( " #000000 " ) );
	
	connect( m_nameColor, SIGNAL(editingFinished()), this, SLOT(updateColor()));
	layoutName->addWidget(m_nameColor);
	vlayout->addLayout(layoutName);
	
	addChild( viewColor);

}



void KTColorPalette::setColor(const QColor& color)
{
	if(m_displayValueColor && m_outlineAndFillColors && m_colorPicker && m_nameColor && m_luminancePicker)
	{
		m_displayValueColor->setColor(color);
		m_outlineAndFillColors->setCurrentColor(color);
		m_colorPicker->setCol(color.hue(), color.saturation ());
	
		m_nameColor->setText(color.name ());
	
		m_luminancePicker->setCol(color.hue(), color.saturation(), color.value());
		m_containerPalette->setColor( QBrush(color) );
		
		if(m_flagGradient)
		{
			m_gradientManager->setColor(color);
			m_gradientManager->repaint();
		}
		
		if(m_gradientManager->gradientApply() == KTGradientManager::None)
		{
			emit colorChanged( m_outlineAndFillColors->foreground(),m_outlineAndFillColors->background() );
		}
	}
}



void KTColorPalette::changeTypeColor(KTDualColorButton::DualColor s)
{
	if(s == KTDualColorButton::Background)
	{
		m_outlineAndFillColors->setCurrent( s);
		m_flagGradient = false;
		setColor( m_outlineAndFillColors->background());
		m_flagGradient = true;
	}
	else
	{
		m_outlineAndFillColors->setCurrent( s);
		m_flagGradient = false;
		setColor( m_outlineAndFillColors->foreground());
		m_flagGradient = true;
	}
}

void KTColorPalette::syncHsv(int h, int s, int v)
{
// 	int th, ts, tv;
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
	colors.second = m_outlineAndFillColors->background();
	
	return colors;
}

void KTColorPalette::changeGradient(const QGradient & gradient)
{
	switch(m_gradientManager->gradientApply())
	{
		case KTGradientManager::FillAndOutLine:
		{
			ktDebug() << m_gradientManager->gradientApply();
			emit colorChanged(QBrush(gradient), QBrush(gradient));
			break;
		}
		case KTGradientManager::Fill:
		{
			ktDebug() << m_gradientManager->gradientApply();
			emit colorChanged(m_outlineAndFillColors->foreground(), QBrush(gradient));
			break;
		}
		case KTGradientManager::OutLine:
		{
			ktDebug() << m_gradientManager->gradientApply();
			emit colorChanged(QBrush(gradient) ,m_outlineAndFillColors->background());
			break;
		}
		default:
		{
			return;
			break;
		}
	}
	m_containerPalette->setColor( QBrush(gradient) );
}

void KTColorPalette::parsePaletteFile(const QString &file)
{
	m_containerPalette->readPaletteFile( file );
}
