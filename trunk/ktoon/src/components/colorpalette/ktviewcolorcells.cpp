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

#include "ktviewcolorcells.h"
#include <kcore/kdebug.h>
#include <kcore/kglobal.h>
#include <QScrollArea>
#include <QGroupBox>
#include <kgui/kimagebutton.h>

#include <kcore/kconfig.h>

struct KTViewColorCells::Private
{
	QComboBox *chooserPalette;
	QStackedWidget *containerPalette;
	KTCellsColor *defaultPalette;
	KTCellsColor *qtColorPalette;
	KTCellsColor *customColorPalette;
	KTCellsColor *customGradientPalette;
	int numColorRecent;
	QBrush currentColor;
};

KTViewColorCells::KTViewColorCells(QWidget *parent) : QFrame(parent), d(new Private)
{
	d->numColorRecent = 0;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	
	setLayout(layout);
	setFrameStyle ( QFrame::Box | QFrame::Raised);
	setupForm();
	setupButtons();
}


KTViewColorCells::~KTViewColorCells()
{
	DCONFIG->beginGroup("ColorPalette");
	DCONFIG->setValue("LastPalette", d->chooserPalette->currentIndex());;
	
	QDir brushesDir(CONFIG_DIR+"/palettes");
	
	if ( ! brushesDir.exists() )
	{
		brushesDir.mkdir(brushesDir.path() );
	}
	
	dDebug("palette") << brushesDir.path();
	
	for(int i = 0; i < d->containerPalette->count(); i++)
	{
		KTCellsColor *palette = qobject_cast<KTCellsColor *>(d->containerPalette->widget(i) );
		if(palette)
		{
			if(!palette->isReadOnly())
			{
				palette->save( CONFIG_DIR+"/palettes/"+ palette->name() + ".ktpl" );
			}
		}
	}
	
	delete d;
	
	DEND;
}

void KTViewColorCells::setupForm()
{
	d->chooserPalette = new QComboBox(this);
	
	
	d->containerPalette = new QStackedWidget(this);
	layout()->addWidget(d->chooserPalette);
	layout()->addWidget(d->containerPalette);
	
	//Default Palette
	d->defaultPalette = new  KTCellsColor(d->containerPalette);
	d->defaultPalette->setName( tr("Default Palette") );
	d->defaultPalette->setReadOnly( true);
	
	fillDefaultColors();
	addPalette(d->defaultPalette);
	
	
	//Named Colors
	d->qtColorPalette = new  KTCellsColor(d->containerPalette);
	d->qtColorPalette->setReadOnly( true );
	d->qtColorPalette->setName( tr("Named Colors") );
	addPalette(d->qtColorPalette);
	
	fillNamedColor();
		
	//Custom Color Palette
	d->customColorPalette = new  KTCellsColor(d->containerPalette);
	d->customColorPalette->setName( tr("Custom Color Palette"));
	addPalette( d->customColorPalette );
	
	//Custom Gradient Palette
	d->customGradientPalette = new  KTCellsColor(d->containerPalette);
	d->customGradientPalette->setName( tr("Custom Gradient Palette"));
	d->customGradientPalette->setType( KTCellsColor::Gradient);
	addPalette( d->customGradientPalette );
	
	connect(d->chooserPalette, SIGNAL(activated ( int  )), d->containerPalette, SLOT(setCurrentIndex ( int )));
	
	DCONFIG->beginGroup("ColorPalette");
	int lastIndex = DCONFIG->value("LastPalette").toInt();
	
	if ( lastIndex > 0 )
	{
		d->chooserPalette->setCurrentIndex(lastIndex);
		d->containerPalette->setCurrentIndex(lastIndex);
	}
	
	readPalettes(HOME_DIR+"/data/palettes"); // Pre-installed
	readPalettes(CONFIG_DIR+"/palettes"); // Locals
}

void KTViewColorCells::readPalettes(const QString &paletteDir)
{
	dDebug("palette") << "Reading palettes from: " << paletteDir;
	QDir dir(paletteDir);
	
	if(dir.exists ())
	{
		QStringList files = dir.entryList ( QStringList() << "*.ktpl" );
		QStringList::ConstIterator it = files.begin();
		
		while(it != files.end())
		{
			readPaletteFile(dir.path()+"/"+*it);
			++it;
		}
	}
}

void KTViewColorCells::readPaletteFile(const QString &file)
{
	KTPaletteParser parser;
	QFile f(file);
	if(parser.parse(&f))
	{
		QList<QBrush> brushes = parser.brushes();
		QString name = parser.paletteName();
		bool editable = parser.paletteIsEditable();
		addPalette(name,brushes,editable );
	}
	else
	{
		dError() << "Error while parse palette file: " << file;
	}
}

void KTViewColorCells::addPalette(const QString & name, const QList<QBrush> & brushes, bool editable )
{
	if( name == d->customColorPalette->name())
	{
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			d->customColorPalette->addItem( *it);
			++it;
		}
	}
	else if(name == d->customGradientPalette->name())
	{
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			d->customGradientPalette->addItem( *it);
			++it;
		}
	}
	else
	{
		KTCellsColor *palette = new  KTCellsColor(d->containerPalette);
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			palette->addItem( *it);
			++it;
		}
		palette->setName(name);
		addPalette(palette);
	
		palette->setReadOnly( !editable );
	}
}

void KTViewColorCells::addPalette(KTCellsColor *palette)
{
	connect(palette, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(changeColor(QTableWidgetItem *)));
	connect(palette, SIGNAL(itemPressed(QTableWidgetItem *)), this, SLOT(changeColor(QTableWidgetItem *)));
	d->chooserPalette->addItem(palette->name());
	d->containerPalette->addWidget(palette);
}


void KTViewColorCells::changeColor(QTableWidgetItem* item)
{
	D_FUNCINFO;
	if(item)
	{
		emit selectColor(item->background());
	}
}

void KTViewColorCells::fillDefaultColors()
{
	int i, j;
	j =0;
    //First column, first 6 rows, a gray scale
	for ( i = 0; i <= 5; i++ )
	{
		d->defaultPalette->addItem(QColor( i * 51, i * 51, i * 51 ));
	}
    //First column, last 6 rows, basic colors
	d->defaultPalette->addItem(QColor( 255, 0, 0 ));
	d->defaultPalette->addItem(QColor( 0, 255, 0 ));
	d->defaultPalette->addItem(QColor( 0, 0, 255 ));
	d->defaultPalette->addItem(QColor( 255, 255, 0 ));
	d->defaultPalette->addItem(QColor( 0, 255, 255 ));
	d->defaultPalette->addItem(QColor( 255, 0, 255 ));

    //Segment from column 1 to 6 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			d->defaultPalette->addItem(QColor( 0, ( j - 1 ) * 51, i * 51 ));
		}
	}

    //Segment from column 1 to 6 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			d->defaultPalette->addItem(QColor( 153, ( j - 1 ) * 51, ( i - 6 ) * 51 ));
		}
	}

    //Segment from column 7 to 12 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 7; j <= 12; j++ )
		{
			d->defaultPalette->addItem(QColor( 51, ( j - 7 ) * 51, i * 51 ));
		}
	}

    //Segment from column 7 to 12 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 7; j <= 12; j++ )
		{
			d->defaultPalette->addItem(QColor( 204, ( j - 7 ) * 51, ( i - 6 ) * 51 ));
		}
	}

    //Segment from column 13 to 18 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 13; j <= 18; j++ )
		{
			d->defaultPalette->addItem(QColor( 102, ( j - 13 ) * 51, i * 51 ));
		}
	}

    //Segment from column 13 to 18 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 13; j <= 18; j++ )
		{
			d->defaultPalette->addItem(QColor( 255, ( j - 13 ) * 51, ( i - 6 ) * 51 ));
		}
	}
	
}

void KTViewColorCells::fillNamedColor()
{
	QStringList strColors = QColor::colorNames ();
	QStringList::ConstIterator it = strColors.begin();
	
	while(it != strColors.end() )
	{
		d->qtColorPalette->addItem( QColor(*it) );
		++it;
	}
	
	d->qtColorPalette->addItem(QColor(0,0,0,0));
	d->qtColorPalette->addItem(QColor(0,0,0,50));
}

void KTViewColorCells::addCurrentColor()
{
	KTCellsColor *palette = qobject_cast<KTCellsColor*>(d->containerPalette->currentWidget());
	
	if(palette)
	{
		if( palette->isReadOnly()
		|| (d->currentColor.gradient()  && palette->type() == KTCellsColor::Color )
		|| d->currentColor.color().isValid() && palette->type() == KTCellsColor::Gradient )
		{
			if( 15 <= d->currentColor.style() && d->currentColor.style() < 18  )
			{
				palette = d->customGradientPalette;
				d->chooserPalette->setCurrentIndex( d->chooserPalette->findText ( d->customGradientPalette->name()));
				d->containerPalette->setCurrentWidget ( d->customGradientPalette );
			}
			else
			{
				palette = d->customColorPalette;
				d->chooserPalette->setCurrentIndex( d->chooserPalette->findText ( d->customColorPalette->name()));
				d->containerPalette->setCurrentWidget ( d->customColorPalette );
			}
		}
		palette->addItem(d->currentColor);
	}
}

void KTViewColorCells::removeCurrentColor()
{
	DCellView *palette = qobject_cast<DCellView *>(d->containerPalette->currentWidget());
	if(palette)
	{
		if( d->defaultPalette != palette)
		{
			//TODO: implementar en DCellView removeItem
		}
	}
}

void KTViewColorCells::setupButtons()
{
	QGroupBox *containerButtons = new QGroupBox(this);
	QBoxLayout *bLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	bLayout->setMargin(0);
	bLayout->setSpacing(0);
	
	containerButtons->setLayout(bLayout);
	
	DImageButton *addItem = new DImageButton( QPixmap(THEME_DIR  + "/icons/plussign.png" ) , 22);
	connect( addItem, SIGNAL( clicked() ), SLOT( addCurrentColor() ));
	addItem->setToolTip(tr( "Add Color" ));
	bLayout->addWidget(addItem);
	
	DImageButton *removeColor = new DImageButton( QPixmap( THEME_DIR + "/icons/minussign.png"), 22);
	
	connect( removeColor, SIGNAL( clicked() ), SLOT( removeCurrentColor() ) );
	removeColor->setToolTip(tr( "Remove Color" ));
	bLayout->addWidget(removeColor);
	
	layout()->addWidget(containerButtons);
}

void KTViewColorCells::setColor(const QBrush& b)
{
	d->currentColor = b;
}



