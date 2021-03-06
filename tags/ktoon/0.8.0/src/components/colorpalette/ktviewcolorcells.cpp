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
#include "ddebug.h"
#include "dglobal.h"
#include <QScrollArea>
#include <QGroupBox>
#include "dimagebutton.h"

#include "dconfig.h"

KTViewColorCells::KTViewColorCells(QWidget *parent)
	: QFrame(parent), m_numColorRecent(0)
{
	
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	setFrameStyle ( QFrame::Box | QFrame::Raised);
	setupForm();
	setupButtons();
}


KTViewColorCells::~KTViewColorCells()
{
	DEND;
	
	DCONFIG->beginGroup("ColorPalette");
	DCONFIG->setValue("LastPalette", m_chooserPalette->currentIndex());;
	
	QDir brushesDir(CONFIG_DIR+"/palettes");
	
	if ( ! brushesDir.exists() )
	{
		brushesDir.mkdir(brushesDir.path() );
	}
	
	dDebug() << brushesDir.path();
	
	for(int i = 0; i < m_containerPalette->count(); i++)
	{
		KTCellsColor *palette = qobject_cast<KTCellsColor *>(m_containerPalette->widget(i) );
		if(palette)
		{
			if(!palette->isReadOnly())
			{
				palette->save( CONFIG_DIR+"/palettes/"+ palette->name() + ".ktpl" );
			}
		}
	}
}

void KTViewColorCells::setupForm()
{
	m_chooserPalette = new QComboBox(this);
	
	
	m_containerPalette = new QStackedWidget(this);
	layout()->addWidget(m_chooserPalette);
	layout()->addWidget(m_containerPalette);
	
	//Default Palette
	m_defaultPalette = new  KTCellsColor;
	m_defaultPalette->setName( tr("Default Palette") );
	m_defaultPalette->setReadOnly( true);
	fillDefaultColors();
	addPalette(m_defaultPalette);
	
	
	//Named Colors
	m_qtColorPalette = new  KTCellsColor;
	m_qtColorPalette->setReadOnly( true );
	m_qtColorPalette->setName( tr("Named Colors") );
	addPalette(m_qtColorPalette);
	
	fillNamedColor();
		
	//Custom Color Palette
	m_customColorPalette = new  KTCellsColor;
	m_customColorPalette->setName( tr("Custom Color Palette"));
	addPalette( m_customColorPalette );
	
	//Custom Gradient Palette
	m_customGradientPalette = new  KTCellsColor;
	m_customGradientPalette->setName( tr("Custom Gradient Palette"));
	m_customGradientPalette->setType( KTCellsColor::Gradient);
	addPalette( m_customGradientPalette );
	
	connect(m_chooserPalette, SIGNAL(activated ( int  )), m_containerPalette, SLOT(setCurrentIndex ( int )));
	
	DCONFIG->beginGroup("ColorPalette");
	int lastIndex = DCONFIG->value("LastPalette").toInt();
	
	if ( lastIndex > 0 )
	{
		m_chooserPalette->setCurrentIndex(lastIndex);
		m_containerPalette->setCurrentIndex(lastIndex);
	}
	
	readPalettes(HOME+"/data/palettes"); // Pre-installed
	readPalettes(CONFIG_DIR+"/palettes"); // Locals
}

void KTViewColorCells::readPalettes(const QString &paletteDir)
{
	dDebug() << "Reading palettes from: " << paletteDir;
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
	KTPaletteParser *parser = new KTPaletteParser();
	QXmlSimpleReader reader;
	reader.setContentHandler(parser);
	reader.setErrorHandler(parser);
	
	QFile f(file);
	QXmlInputSource xmlsource(&f);
			
	if ( reader.parse(&xmlsource) )
	{
		QList<QBrush> brushes = parser->brushes();
		QString name = parser->paletteName();
		bool editable = parser->paletteIsEditable();
		addPalette(name,brushes,editable );
	}
	else
	{
		dError() << "Error while parse palette file: " << file;
	}
	delete parser;
}

void KTViewColorCells::addPalette(const QString & name, const QList<QBrush> & brushes, bool editable )
{
	if( name == m_customColorPalette->name())
	{
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			m_customColorPalette->addColor( *it);
			++it;
		}
	}
	else if(name == m_customGradientPalette->name())
	{
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			m_customGradientPalette->addColor( *it);
			++it;
		}
	}
	else
	{
		KTCellsColor *palette = new  KTCellsColor;
		QList<QBrush>::ConstIterator it = brushes.begin();
		
		while(it != brushes.end())
		{
			palette->addColor( *it);
			++it;
		}
		palette->setName(name);
		addPalette(palette);
	
		palette->setReadOnly( !editable );
	}
}

void KTViewColorCells::addPalette(KTCellsColor *palette)
{
	connect(palette, SIGNAL(itemEntered(DCellViewItem *)), this, SLOT(changeColor(DCellViewItem *)));
	connect(palette, SIGNAL(itemPressed(DCellViewItem *)), this, SLOT(changeColor(DCellViewItem *)));
	m_chooserPalette->addItem(palette->name());
	m_containerPalette->addWidget(palette);
}


void KTViewColorCells::changeColor(DCellViewItem* item)
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
		m_defaultPalette->addColor(QColor( i * 51, i * 51, i * 51 ));
	}
    //First column, last 6 rows, basic colors
	m_defaultPalette->addColor(QColor( 255, 0, 0 ));
	m_defaultPalette->addColor(QColor( 0, 255, 0 ));
	m_defaultPalette->addColor(QColor( 0, 0, 255 ));
	m_defaultPalette->addColor(QColor( 255, 255, 0 ));
	m_defaultPalette->addColor(QColor( 0, 255, 255 ));
	m_defaultPalette->addColor(QColor( 255, 0, 255 ));

    //Segment from column 1 to 6 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			m_defaultPalette->addColor(QColor( 0, ( j - 1 ) * 51, i * 51 ));
		}
	}

    //Segment from column 1 to 6 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			m_defaultPalette->addColor(QColor( 153, ( j - 1 ) * 51, ( i - 6 ) * 51 ));
		}
	}

    //Segment from column 7 to 12 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 7; j <= 12; j++ )
		{
			m_defaultPalette->addColor(QColor( 51, ( j - 7 ) * 51, i * 51 ));
		}
	}

    //Segment from column 7 to 12 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 7; j <= 12; j++ )
		{
			m_defaultPalette->addColor(QColor( 204, ( j - 7 ) * 51, ( i - 6 ) * 51 ));
		}
	}

    //Segment from column 13 to 18 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 13; j <= 18; j++ )
		{
			m_defaultPalette->addColor(QColor( 102, ( j - 13 ) * 51, i * 51 ));
		}
	}

    //Segment from column 13 to 18 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 13; j <= 18; j++ )
		{
			m_defaultPalette->addColor(QColor( 255, ( j - 13 ) * 51, ( i - 6 ) * 51 ));
		}
	}
	
}

void KTViewColorCells::fillNamedColor()
{
	QStringList strColors = QColor::colorNames ();
	QStringList::ConstIterator it = strColors.begin();
	
	while(it != strColors.end() )
	{
		m_qtColorPalette->addColor( QColor(*it) );
		++it;
	}
	
	m_qtColorPalette->addColor(QColor(0,0,0,0));
	m_qtColorPalette->addColor(QColor(0,0,0,50));
}

void KTViewColorCells::addCurrentColor()
{
	KTCellsColor *palette = qobject_cast<KTCellsColor*>(m_containerPalette->currentWidget());
	
	if(palette)
	{
		if( palette->isReadOnly()
		|| (m_currentColor.gradient()  && palette->type() == KTCellsColor::Color )
		|| m_currentColor.color().isValid() && palette->type() == KTCellsColor::Gradient )
		{
			if( 15 <= m_currentColor.style() && m_currentColor.style() < 18  )
			{
				palette = m_customGradientPalette;
				m_chooserPalette->setCurrentIndex( m_chooserPalette->findText ( m_customGradientPalette->name()));
				m_containerPalette->setCurrentWidget ( m_customGradientPalette );
			}
			else
			{
				palette = m_customColorPalette;
				m_chooserPalette->setCurrentIndex( m_chooserPalette->findText ( m_customColorPalette->name()));
				m_containerPalette->setCurrentWidget ( m_customColorPalette );
			}
		}
		palette->addColor(m_currentColor);
	}
}

void KTViewColorCells::removeCurrentColor()
{
	DCellView *palette = qobject_cast<DCellView *>(m_containerPalette->currentWidget());
	if(palette)
	{
		if( m_defaultPalette != palette)
		{
			//TODO: implementar en DCellView removeItem
		}
	}
}

void KTViewColorCells::setupButtons()
{
	QGroupBox *containerButtons = new QGroupBox(this);
	QBoxLayout *bLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	containerButtons->setLayout(bLayout);
	bLayout->setMargin(3);
	DImageButton *m_addColor = new DImageButton( QPixmap(THEME_DIR  + "/icons/plussign.png" ) , 22/*, containerButtons*/);
	connect( m_addColor, SIGNAL( clicked() ), SLOT( addCurrentColor() ));
	m_addColor->setToolTip(tr( "Add Color" ));
	bLayout->addWidget(m_addColor);
	
	DImageButton *m_removeColor = new DImageButton( QPixmap( THEME_DIR + "/icons/minussign.png"), 22/*, containerButtons*/);
	
	connect( m_removeColor, SIGNAL( clicked() ), SLOT( removeColor() ) );
	m_removeColor->setToolTip(tr( "Add Color" ));
	bLayout->addWidget(m_removeColor);
	
#if 0
	DImageButton *m_addPalette = new DImageButton( QPixmap(THEME_DIR + "/icons/plussign.png" ), 22, containerButtons);
	connect( m_addPalette, SIGNAL( clicked() ), SLOT( addPalette() ) );
	m_addPalette->setToolTip(tr( "Add Color" ));
	bLayout->addWidget(m_addPalette);
	
	DImageButton *m_removePalette = new DImageButton(QPixmap( THEME_DIR+ "/icons/minussign.png"), 22, containerButtons);
	
	connect( m_removePalette, SIGNAL( clicked() ), SLOT( removeColor() ) );
	m_removePalette->setToolTip(tr( "Add Color" ));
	bLayout->addWidget(m_removePalette);
#endif
	layout()->addWidget(containerButtons);
}

void KTViewColorCells::setColor(const QBrush& b)
{
	m_currentColor = b;
}



