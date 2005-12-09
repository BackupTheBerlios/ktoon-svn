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
#include "ktdebug.h"
#include "ktapplication.h"
#include <QScrollArea>


KTViewColorCells::KTViewColorCells(QWidget *parent)
	: QFrame(parent), m_numColorRecent(0)
{
	
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	setFrameStyle ( QFrame::Box | QFrame::Raised);
	setupForm();
}


KTViewColorCells::~KTViewColorCells()
{
	KTEND;
}

void KTViewColorCells::setupForm()
{
	m_chooserPalette = new QComboBox(this);
	
	
	m_containerPalette = new QStackedWidget(this);
	layout()->addWidget(m_chooserPalette);
	layout()->addWidget(m_containerPalette);
	
	m_chooserPalette->addItem(tr("Default Palette"));
	m_defaultPalette = new  KTCellsColor( m_containerPalette );
	m_defaultPalette->setReadOnly( true);
	fillDefaultColors();
	connect(m_defaultPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_containerPalette->addWidget(m_defaultPalette);
	
	
	m_chooserPalette->addItem(tr("Named Colors"));
	m_qtColorPalette = new  KTCellsColor( m_containerPalette );
	m_qtColorPalette->setReadOnly( true );
	connect(m_qtColorPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_containerPalette->addWidget(m_qtColorPalette);
	fillNamedColor();
	
// 	readPalettes(KTOON_HOME+"/data/palettes"); // Pre-installed
	readPalettes(ktapp->configDir()+"/palettes"); // Locals
	
	m_chooserPalette->addItem(tr("Custom Color Palette"));
	m_customColorPalette = new  KTCellsColor(m_containerPalette);
	connect(m_customColorPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_containerPalette->addWidget(m_customColorPalette);

	
	m_chooserPalette->addItem(tr("Custom Gradient Palette"));
	m_customGradientPalette = new  KTCellsColor(m_containerPalette);
	connect(m_customGradientPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_containerPalette->addWidget(m_customGradientPalette);
	
	connect(m_chooserPalette, SIGNAL(activated ( int  )), m_containerPalette, SLOT(setCurrentIndex ( int )));
}

void KTViewColorCells::readPalettes(const QString &paletteDir)
{
	QDir dir(paletteDir);
	
	if(dir.exists ())
	{
		QStringList files = dir.entryList ( QStringList() << "*.ktpl" );
		QStringList::ConstIterator it = files.begin();
		
		ktDebug() << files;
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
		ktError() << "Error while parse palette file: " << file;
	}
	delete parser;
}

void KTViewColorCells::addPalette(const QString & name, const QList<QBrush> & brushes, bool editable )
{
	KTCellsColor *palette = new  KTCellsColor( m_containerPalette);
	QList<QBrush>::ConstIterator it = brushes.begin();
	
	while(it != brushes.end())
	{
		palette->addColor( *it);
		++it;
	}
	
	connect(palette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	palette->setReadOnly( !editable );
	m_chooserPalette->addItem(name);
	m_containerPalette->addWidget(palette);
}


void KTViewColorCells::changeColor(KTCellViewItem* item)
{
	if(item)
	{
		if(item->background().gradient ())
		{
			emit selectGradient( *item->background().gradient ());
		}
		else
		{
			emit selectColor(item->background());
		}
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



void KTViewColorCells::addCustomColor(const QBrush& c)
{
	
	KTCellsColor *palette =  qobject_cast<KTCellsColor*>(m_containerPalette->currentWidget());
	
	if(palette->isReadOnly() || (c.gradient() && palette->type() == KTCellsColor::Color ) || !c.color().isValid() && palette->type() == KTCellsColor::Gradient )
	{
		if(c.gradient())
		{
			palette = m_customGradientPalette;
			m_chooserPalette->setCurrentIndex( m_chooserPalette->count()-1 );
			m_containerPalette->setCurrentWidget ( m_customGradientPalette );
		}
		else
		{
			palette = m_customColorPalette;
			m_chooserPalette->setCurrentIndex( m_chooserPalette->count()-2 );
			m_containerPalette->setCurrentWidget ( m_customColorPalette );
		}
	}
	palette->addColor(c);
}

void KTViewColorCells::removeCurrentColor()
{
	KTCellView *palette = qobject_cast<KTCellView *>(m_containerPalette->currentWidget());
	if(palette)
	{
		if( m_defaultPalette != palette)
		{
			//TODO: implementar en KTCellView removeItem
		}
	}
}
