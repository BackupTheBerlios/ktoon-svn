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
#include <kcore/kdebug.h>

#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QToolTip>
#include <kgui/kimagebutton.h>
#include <kcore/kglobal.h>
#include <kcore/kconfig.h>

#include "ktpaintareaevent.h"
#include "ktcolorvalue.h"
#include "ktviewcolorcells.h"

#include "ktcolorpicker.h"
#include "ktluminancepicker.h"
#include "ktgradientcreator.h"

#include <QComboBox>
#include <QGroupBox>
#include <QSplitter>
#include <QMenu>

#include <kgui/ktoolbox.h>
#include <kgui/kvhbox.h>

struct KTColorPalette::Private
{
		DToolBox *centralWidget;
		KTViewColorCells *containerPalette;
		KTColorValue *displayColorValue;
		KTColorPicker *colorPicker;
		KTLuminancePicker *luminancePicker;
		KTGradientCreator *gradientManager;
		QLineEdit *nameColor;
		DDualColorButton *outlineAndFillColors;
		QBrush currentOutlineColor, currentFillColor;
		QComboBox *labelType;
		QSplitter *splitter;
		bool flagGradient;
		TypeBrush type;
		
		~Private()
		{
// 			delete centralWidget;
// 			delete containerPalette;
// 			delete displayColorValue;
// 			delete colorPicker;
// 			delete luminancePicker;
// 			delete gradientManager;
// 			delete nameColor;
// 			delete outlineAndFillColors;
// 			delete labelType;
// 			delete splitter;
		};
		
};


KTColorPalette::KTColorPalette(QWidget *parent) : KTModuleWidgetBase(parent), d(new Private)
{
	DINIT;
	d->currentOutlineColor = Qt::black;
	d->currentFillColor = Qt::transparent;
	d->flagGradient = true;
	d->type = Solid;
	
	setWindowTitle( tr( "Color &Palette" ) );
	setWindowIcon(QPixmap(THEME_DIR+"/icons/color_palette.png") );

	d->splitter = new QSplitter(Qt::Vertical, this);
	addChild( d->splitter);
	
	d->centralWidget = new DToolBox(d->splitter);

	setupChooserTypeColor();
	setupGradienManager();
	setupDisplayColor();
	d->splitter->addWidget(d->centralWidget);
	d->centralWidget->setPalette(palette());
	
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
	delete d;
}

void KTColorPalette::setupChooserTypeColor()
{
	QFrame *colorMixer = new QFrame(d->centralWidget);
	
	colorMixer->setFrameStyle(QFrame::Box | QFrame::Sunken );
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
// 	layout->setMargin(1);
// 	layout->setSpacing(1);
	
	colorMixer->setLayout(layout);

// 	layout->setSizeConstraint(QLayout::SetFixedSize);
	
	d->displayColorValue = new KTColorValue(colorMixer);
	
	QBoxLayout *layoutContainer = new QBoxLayout(QBoxLayout::LeftToRight);
// 	layoutContainer->setMargin(0);
	
	layoutContainer->addStretch(2);
	
	d->colorPicker = new KTColorPicker(colorMixer);
	connect( d->colorPicker, SIGNAL(newCol(int, int)), this, SLOT(setHS(int, int)));
	connect(d->displayColorValue, SIGNAL(hueChanged(int)), d->colorPicker, SLOT(setH(int)));
	connect(d->displayColorValue, SIGNAL(saturationChanged(int)), d->colorPicker, SLOT(setS(int)));
	
	layoutContainer->addWidget(d->colorPicker, 0, Qt::AlignLeft);
	
	d->luminancePicker = new KTLuminancePicker( colorMixer );
	connect( d->luminancePicker, SIGNAL( newHsv(int, int, int )), this, SLOT (syncHsv(int, int, int)));

	d->luminancePicker->setMaximumWidth(15);
	d->luminancePicker->setMinimumWidth(15);
	connect(d->displayColorValue, SIGNAL(valueChanged(int)), d->luminancePicker, SLOT(setVal( int )));
	layoutContainer->addWidget(d->luminancePicker, 0, Qt::AlignLeft);
	layoutContainer->setSpacing(3);
	
	layoutContainer->addStretch(2);
	
	layout->addLayout(layoutContainer);
	
	layout->addWidget(d->displayColorValue/*, 0, Qt::AlignCenter*/);
// 	d->displayColorValue->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	d->displayColorValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
// 	this->layout()->setAlignment( colorMixer, Qt::AlignTop);
	connect(d->displayColorValue, SIGNAL(brushChanged(const QBrush&)), this, SLOT(setColor(const QBrush &)));
	
	d->centralWidget->addPage(colorMixer, tr("Color Mixer"));
}

void KTColorPalette::setupGradienManager()
{
	d->gradientManager = new KTGradientCreator(this);
	connect(d->gradientManager, SIGNAL(gradientChanged( const QBrush& )), this, SLOT(setColor(const QBrush &) ));
	d->centralWidget->addPage(d->gradientManager,tr("Gradients"));
	
}

void KTColorPalette::setupDisplayColor()
{
	
	//palettes
	d->containerPalette = new KTViewColorCells(d->splitter);
	
	connect (d->containerPalette, SIGNAL(selectColor( const QBrush& )), this, SLOT(setColor( const QBrush& )));
	
// 	connect (d->containerPalette, SIGNAL(selectGradient(const QBrush&)), this, SLOT(changeGradient(const QBrush&)));
	
// 	connect (d->containerPalette, SIGNAL(selectGradient(const QGradient&)), d->gradientManager, SLOT(setGradient(const QGradient&)));
	
	d->splitter->addWidget(d->containerPalette);
	
	
	//////////
	QFrame *viewColor = new QFrame(this);
	QBoxLayout *vlayout = new QBoxLayout(QBoxLayout::LeftToRight);
	vlayout->setMargin(0);
	
	viewColor->setLayout(vlayout);
	
	d->labelType = new QComboBox( viewColor);
	d->labelType->addItem(tr("Solid"));
	d->labelType->addItem(tr("Gradient"));
	
	connect(d->labelType, SIGNAL(activated ( const QString & )), this, SLOT(changeBrushType(const QString &)));
	
	
	
	vlayout->addWidget(d->labelType);
	
	d->outlineAndFillColors = new DDualColorButton(d->currentOutlineColor, d->currentFillColor, viewColor);
	d->outlineAndFillColors->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
	connect( d->outlineAndFillColors,  SIGNAL(currentChanged(DDualColorButton::DualColor)),this, SLOT(changeTypeColor(DDualColorButton::DualColor)));

connect( d->outlineAndFillColors,  SIGNAL(fgChanged(const QBrush &)),this, SLOT(
		setFG(const QBrush &)));
connect( d->outlineAndFillColors,  SIGNAL(bgChanged(const QBrush &)),this, SLOT(setBG( const QBrush &)));
	
	
	vlayout->addWidget( d->outlineAndFillColors);
	
	QBoxLayout *layoutName = new  QBoxLayout(QBoxLayout::TopToBottom);
	layoutName->setMargin(0);
	
	layoutName->addWidget(new QLabel( "<b>HTML</b>", viewColor));
	d->nameColor = new QLineEdit(viewColor);
	QFontMetrics fm( font() );
	d->nameColor->setMaximumWidth( fm.width ( " #000000 " ) );
	
	connect( d->nameColor, SIGNAL(editingFinished()), this, SLOT(updateColor()));
	layoutName->addWidget(d->nameColor);
	vlayout->addLayout(layoutName);
	
	addChild( viewColor);

}



void KTColorPalette::setColor(const QBrush& brush)
{
	QColor color = brush.color();
	
	if(color.isValid())
	{
		if(d->type == Gradient)
		{
			d->gradientManager->setCurrentColor(color);
		}
		if(d->displayColorValue && d->outlineAndFillColors && d->colorPicker && d->nameColor && d->luminancePicker)
		{
			
			
			d->colorPicker->setCol(color.hue(), color.saturation ());
			if(d->type == Solid)
			{
				d->outlineAndFillColors->setCurrentColor(color);
			}
			d->nameColor->setText(color.name ());
			d->luminancePicker->setCol(color.hue(), color.saturation(), color.value());
			d->containerPalette->setColor( brush );
			d->displayColorValue->setColor(color);
		}
		
	}
	else if(brush.gradient())
	{
		
		QGradient gradient(*brush.gradient());
		changeBrushType(tr("Gradient"));

		d->containerPalette->setColor(gradient);
		d->outlineAndFillColors->setCurrentColor(gradient);
		if( sender () != d->gradientManager )
		{
			d->gradientManager->setGradient(gradient);
		}
		
	}
// 	emit brushChanged( d->outlineAndFillColors->foreground(),d->outlineAndFillColors->background() );
		
	// FIXME: emitir estos eventos independientemente segun sea escogido.
// 	if(d->outlineAndFillColors->current() == DDualColorButton::Background)
// 	{
		KTPaintAreaEvent event(KTPaintAreaEvent::ChangeBrush, d->outlineAndFillColors->background());
		emit paintAreaEventTriggered( &event );
// 	}
// 	else
// 	{
		KTPaintAreaEvent event2(KTPaintAreaEvent::ChangePenBrush, d->outlineAndFillColors->foreground());
		emit paintAreaEventTriggered( &event2 );
// 	}
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
	d->outlineAndFillColors->setCurrent( DDualColorButton::Foreground );
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
	d->outlineAndFillColors->setCurrent( DDualColorButton::Background);
	setColor(brush);
}


void KTColorPalette::changeTypeColor(DDualColorButton::DualColor s)
{
	if(s == DDualColorButton::Background)
	{
		d->outlineAndFillColors->setCurrent( s);
		setColor( d->outlineAndFillColors->background());
	}
	else
	{
		d->outlineAndFillColors->setCurrent( s);
		d->flagGradient = false;
		setColor( d->outlineAndFillColors->foreground());
		d->flagGradient = true;
	}
}

void KTColorPalette::syncHsv(int h, int s, int v)
{
	QColor tmpColor = d->outlineAndFillColors->currentColor().color();
	tmpColor.setHsv( h, s, v, tmpColor.alpha() );
	if(d->luminancePicker == sender())
		setColor(tmpColor);
}


void KTColorPalette::setHS(int h , int s)
{
	QColor tmpColor = d->outlineAndFillColors->currentColor().color();
	
	tmpColor.setHsv( h, s, d->luminancePicker->value(), tmpColor.alpha() );
	setColor(QBrush(tmpColor));
}

void KTColorPalette::updateColor()
{
	QColor tmpColor(d->nameColor->text());
	tmpColor.setAlpha(d->displayColorValue->alpha());
	setColor(tmpColor);
}

QPair<QColor, QColor> KTColorPalette::color()
{
	QPair<QColor, QColor> colors;
	colors.first = d->outlineAndFillColors->foreground().color();
	colors.second = d->outlineAndFillColors->background().color();
	return colors;
}


void KTColorPalette::parsePaletteFile(const QString &file)
{
	d->containerPalette->readPaletteFile( file );
}

void KTColorPalette::mousePressEvent ( QMouseEvent * e )
{
	if(e->button () == Qt::RightButton)
	{
		QMenu *menu = new QMenu(tr("type brush"), this);
		menu->exec(e->globalPos ());
		delete menu;
	}
}


void KTColorPalette::changeBrushType(const QString& type)
{
	
	if(type == tr("Solid"))
	{
		d->type = Solid;
	}
	else if(type == tr("Gradient"))
	{
		d->type = Gradient;
	}
	if(type != d->labelType->currentText ())
	{
		int index =  d->labelType->findText(type);
		if(index >= 0)
		{
			d->labelType->setCurrentIndex(index);
		}
	}
	
}



