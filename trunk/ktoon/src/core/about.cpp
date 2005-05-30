/***************************************************************************
 *   Copyright (C) 2004 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include <qfile.h>
#include <qtextstream.h>
#include <qimage.h>

#include "about.h"
#include "images.h"
#include "ktapplication.h"

#define DEBUG_ABOUT 1

About::About( QWidget *parent ) : QTabDialog( parent )
{ 
	setCaption( tr( "About" ) + QString( " KToon..." ) );
// 	setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
	resize( 580, 400 );
	setMaximumSize( 580, 400 );
	setMinimumSize( 580, 400 );
// 	setFont( QFont( "helvetica", 10 ) );

    	//1: Credits

	QFile creditsFile( KTOON_HOME+"/data/credits.txt" );
	QString creditsText;
	if ( creditsFile.open( IO_ReadOnly ) )
	{
		QTextStream stream( &creditsFile );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine();
			creditsText += line + "\n";
		}
		creditsFile.close();
	}
    
	m_credits = new KTAnimWidget( QPixmap(KTOON_HOME+"/images/credits-image.png" ), creditsText, this );
	addTab( m_credits, tr( "Credits" ) );

    	// 2: Ack
	ListOfPixmaps lop;

	for(uint i = 1; i < 11; i++)
	{
		lop << QPixmap(KTOON_HOME+QString("/images/sequences/ack-image%1.png").arg(i));
	}
	
	m_ack = new KTAnimWidget( lop,this );

	addTab( m_ack, tr( "Acknowledgements" ) );

    	// 3: Changelog

 	QScrollView *scroll = new QScrollView( this );
	
	QLabel *logText = new QLabel(scroll->viewport());
	scroll->addChild( logText );
	
	QString readText;
	QFile clFile( KTOON_HOME+"/data/Changelog" );
	
	if ( clFile.open( IO_ReadOnly ) )
	{
		QTextStream stream( &clFile );
		while ( ! stream.atEnd() )
		{
			QString text;
			text = stream.readLine();
			readText += text+"\n";
		}
		clFile.close();
	}
	logText -> setText( readText );
	
	addTab( scroll, tr( "Changelog" ) );

    	// 4: Toonka Films

	QLabel *toonka = new QLabel( this );
	toonka->setPixmap(QPixmap( KTOON_HOME+"/images/toonka.jpg" ));
	addTab( toonka, "Toonka Films" );

    	// 5: Laboratoon

	QLabel *laboratoon = new QLabel( this );
	laboratoon->setPixmap( QPixmap( KTOON_HOME+"/images/laboratoon.jpg" ) );
	addTab( laboratoon, "Laboratoon" );

    	// 6: Licence

	QScrollView *scrollLicence = new QScrollView( this );

	QLabel *licenceText = new QLabel( scrollLicence );
	scrollLicence->addChild( licenceText );
	QString licence;
	QFile licenceFile( KTOON_HOME+"/data/COPYING" );
	if ( licenceFile.open( IO_ReadOnly ) )
	{
		QTextStream stream( &licenceFile );
		while ( ! stream.atEnd() )
		{
			QString line = stream.readLine();
			licence += line+"\n";
		}
		licenceFile.close();
	}
	licenceText -> setText( licence );
	addTab( scrollLicence, tr( "License Agreement" ) );
}

About::~About()
{
}


