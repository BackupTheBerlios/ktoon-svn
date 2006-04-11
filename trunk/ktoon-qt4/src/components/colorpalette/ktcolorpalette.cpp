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
#include "ddebug.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QToolTip>
#include "dimagebutton.h"
#include "dglobal.h"


KTColorPalette::KTColorPalette(QWidget *parent) : KTModuleWidgetBase(parent), m_currentOutlineColor(Qt::black), m_currentFillColor(Qt::transparent), m_flagGradient(true), m_type(Solid)
{
	DINIT;
	setCaption( tr( "Color Palette" ) );

	m_splitter = new QSplitter(Qt::Vertical, this);
	addChild( m_splitter);
	
	m_centralWidget = new DToolBox(m_splitter);

	setupChooserTypeColor();
	setupGradienManager();
	setupDisplayColor();
	m_splitter->addWidget(m_centralWidget);
	m_centralWidget->setPalette(palette());
	
	DCONFIG->beginGroup("ColorPalette");
	QColor foreground = QColor(DCONFIG->value("LastForegroundColor", Qt::black).toString());
	QColor background = QColor(DCONFIG->value("LastBackgroundColor", Qt::transparent).toString());
	
}


KTColorPalette::~KTColorPalette()
{
	DEND;
	
	DCONFIG->beginGroup("ColorPalette");
	DCONFIG->setValue("LastForegroundColor", color().first);
	DCONFIG->setValue("LastBackgroundColor", color().second);
}

void KTColorPalette::setupChooserTypeColor()
{
	QFrame *colorMixer = new QFrame(m_centralWidget);
	
	colorMixer->setFrameStyle(QFrame::Box | QFrame::Sunken );
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	colorMixer->setLayout(layout);

// 	layout->setSizeConstraint(QLayout::SetFixedSize);
	
	m_displayValueColor = new KTValueColor(colorMixer);
	
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::LeftToRight);
	layoutContainer->addStretch(2);
	
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
	
	layoutContainer->addStretch(2);
	
	layout->addLayout(layoutContainer);
	layout->addWidget(m_displayValueColor, 0, Qt::AlignCenter);
	this->layout()->setAlignment( colorMixer, Qt::AlignTop);
	connect(m_displayValueColor, SIGNAL(brushChanged(const QBrush&)), this, SLOT(setColor(const QBrush &)));
	
	m_centralWidget->addPage(colorMixer, tr("Color Mixer"));
}

void KTColorPalette::setupGradienManager()
{
	m_gradientManager = new DGradientCreator(this);
	connect(m_gradientManager, SIGNAL(gradientChanged( const QBrush& )), this, SLOT(setColor(const QBrush &) ));
	m_centralWidget->addPage(m_gradientManager,tr("Gradients"));
	
}

void KTColorPalette::setupDisplayColor()
{
	
	//palettes
	m_containerPalette = new KTViewColorCells(m_splitter);
	
	connect (m_containerPalette, SIGNAL(selectColor( const QBrush& )), this, SLOT(setColor( const QBrush& )));
	
// 	connect (m_containerPalette, SIGNAL(selectGradient(const QBrush&)), this, SLOT(changeGradient(const QBrush&)));
	
// 	connect (m_containerPalette, SIGNAL(selectGradient(const QGradient&)), m_gradientManager, SLOT(setGradient(const QGradient&)));
	
	m_splitter->addWidget(m_containerPalette);
	
	
	//////////
	QFrame *viewColor = new QFrame(this);
	QBoxLayout *vlayout = new QBoxLayout(QBoxLayout::LeftToRight);
	viewColor->setLayout(vlayout);
	
	m_labelType = new QComboBox( viewColor);
	m_labelType->addItem(tr("Solid"));
	m_labelType->addItem(tr("Gradient"));
	
	connect(m_labelType, SIGNAL(activated ( const QString & )), this, SLOT(changeBrushType(const QString &)));
	
	
	
	vlayout->addWidget(m_labelType);
	
	m_outlineAndFillColors = new KTDualColorButton(m_currentOutlineColor, m_currentFillColor, viewColor);
	m_outlineAndFillColors->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
	connect( m_outlineAndFillColors,  SIGNAL(currentChanged(KTDualColorButton::DualColor)),this, SLOT(changeTypeColor(KTDualColorButton::DualColor)));

connect( m_outlineAndFillColors,  SIGNAL(fgChanged(const QBrush &)),this, SLOT(
		setFG(const QBrush &)));
connect( m_outlineAndFillColors,  SIGNAL(bgChanged(const QBrush &)),this, SLOT(
		setBG( (const QBrush &))));
	
	
	vlayout->addWidget( m_outlineAndFillColors);
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::TopToBottom);
	layoutName->addWidget(new QLabel( "<b>HTML</b>", viewColor));
	m_nameColor = new QLineEdit(viewColor);
	QFontMetrics fm( font() );
	m_nameColor->setMaximumWidth( fm.width ( " #000000 " ) );
	
	connect( m_nameColor, SIGNAL(editingFinished()), this, SLOT(updateColor()));
	layoutName->addWidget(m_nameColor);
	vlayout->addLayout(layoutName);
	
	addChild( viewColor);

}



void KTColorPalette::setColor(const QBrush& brush)
{
	QColor color = brush.color();
	
	if(color.isValid())
	{
		if(m_type == Gradient)
		{
			m_gradientManager->setCurrentColor(color);
		}
		if(m_displayValueColor && m_outlineAndFillColors && m_colorPicker && m_nameColor && m_luminancePicker)
		{
			
			
			m_colorPicker->setCol(color.hue(), color.saturation ());
			if(m_type == Solid)
			{
				m_outlineAndFillColors->setCurrentColor(color);
			}
			m_nameColor->setText(color.name ());
			m_luminancePicker->setCol(color.hue(), color.saturation(), color.value());
			m_containerPalette->setColor( brush );
			m_displayValueColor->setColor(color);
		}
		
	}
	else if(brush.gradient())
	{
		
		QGradient gradient(*brush.gradient());
		changeBrushType(tr("Gradient"));

		m_containerPalette->setColor(gradient);
		m_outlineAndFillColors->setCurrentColor(gradient);
		if( sender () != m_gradientManager )
		{
			m_gradientManager->setGradient(gradient);
		}
		
	}
	emit brushChanged( m_outlineAndFillColors->foreground(),m_outlineAndFillColors->background() );
	
}

void KTColorPalette::setFG(const QBrush &brush)
{
	if(brush.color().isValid())
	{
		changeBrushType(tr("Solid"));
	}
	else
	{
		changeBrushType(tr("Gradient"));
	}
	m_outlineAndFillColors->setCurrent( KTDualColorButton::Foreground );
	setColor(brush);
}

void KTColorPalette::setBG(const QBrush &brush)
{
	if(brush.color().isValid())
	{
		changeBrushType(tr("Solid"));
	}
	else
	{
		changeBrushType(tr("Gradient"));
	}
	m_outlineAndFillColors->setCurrent( KTDualColorButton::Background);
	setColor(brush);
}


void KTColorPalette::changeTypeColor(KTDualColorButton::DualColor s)
{
	if(s == KTDualColorButton::Background)
	{
		m_outlineAndFillColors->setCurrent( s);
		setColor( m_outlineAndFillColors->background());
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
	QColor tmpColor = m_outlineAndFillColors->currentColor().color();
	tmpColor.setHsv( h, s, v, tmpColor.alpha() );
	if(m_luminancePicker == sender())
		setColor(tmpColor);
}


void KTColorPalette::setHS(int h , int s)
{
	QColor tmpColor = m_outlineAndFillColors->currentColor().color();
	
	tmpColor.setHsv( h, s, m_luminancePicker->value(), tmpColor.alpha() );
	setColor(QBrush(tmpColor));
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
	colors.first = m_outlineAndFillColors->foreground().color();
	colors.second = m_outlineAndFillColors->background().color();
	return colors;
}


void KTColorPalette::parsePaletteFile(const QString &file)
{
	m_containerPalette->readPaletteFile( file );
}

void KTColorPalette::mousePressEvent ( QMouseEvent * e )
{
	if(e->button () == Qt::RightButton)
	{
		QMenu *menu = new QMenu(tr("type brush"), this);
// 		menu->addAction(tr("solid"), this, SLOT(setSolidType()));
// 		menu->addAction(tr("gradient"), this, SLOT(setGradientType()));
		menu->exec(e->globalPos ());
		delete menu;
	}
}


void KTColorPalette::changeBrushType(const QString& type)
{
	
	if(type == tr("Solid"))
	{
		m_type = Solid;
	}
	else if(type == tr("Gradient"))
	{
		m_type = Gradient;
	}
	if(type != m_labelType->currentText ())
	{
		int index =  m_labelType->findText(type);
		if(index >= 0)
		{
			m_labelType->setCurrentIndex(index);
		}
	}
	
}



