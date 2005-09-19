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
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>

#include "about.h"
#include "images.h"
#include "ktapplication.h"
#include "kimageeffect.h"

#define DEBUG_ABOUT 1

About::About( QWidget *parent ) : Q3TabDialog( parent )
{ 
	setCaption( tr( "About" ) + QString( " KToon..." ) );

    	//1: Credits

	QFile creditsFile( KTOON_HOME+"/data/credits.txt" );
	QString creditsText;
	if ( creditsFile.open( QIODevice::ReadOnly ) )
	{
		QTextStream stream( &creditsFile );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine();
			creditsText += line + "\n";
		}
		creditsFile.close();
	}
	
	QImage credits = QImage(KTOON_HOME+"/images/credits-image.png" );
	
	KImageEffect::fade(credits, 0.25, paletteBackgroundColor());
    
	m_credits = new KTAnimWidget( QPixmap(credits), creditsText, this );
	addTab( m_credits, tr( "Credits" ) );

    	// 2: Ack
	ListOfPixmaps lop;

	for(uint i = 1; i < 11; i++)
	{
		QImage tmp(KTOON_HOME+QString("/images/sequences/ack-image%1.png").arg(i));
		KImageEffect::fade(tmp, 0.2, paletteBackgroundColor());
		lop << QPixmap(tmp);
	}
	
	m_ack = new KTAnimWidget( lop,this );

	addTab( m_ack, tr( "Acknowledgements" ) );

    	// 3: Changelog

 	Q3ScrollView *scroll = new Q3ScrollView( this );
	
	QLabel *logText = new QLabel(scroll->viewport());
	scroll->addChild( logText );
	
	QString readText;
	QFile clFile( KTOON_HOME+"/data/Changelog" );
	
	if ( clFile.open( QIODevice::ReadOnly ) )
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
	
	QImage toonkaImg( KTOON_HOME+"/images/toonka.jpg" );
	KImageEffect::fade( toonkaImg,0.2,paletteBackgroundColor()); 
	
	toonka->setPixmap(QPixmap(toonkaImg));
	addTab( toonka, "Toonka Films" );

    	// 5: Laboratoon

	QLabel *laboratoon = new QLabel( this );
	
	QImage laboratoonImg( KTOON_HOME+"/images/laboratoon.jpg" );
	KImageEffect::fade( laboratoonImg,0.2,paletteBackgroundColor()); 
	
	laboratoon->setPixmap( QPixmap(laboratoonImg  ) );
	addTab( laboratoon, "Laboratoon" );

    	// 6: Licence

	Q3ScrollView *scrollLicence = new Q3ScrollView( this );

	QLabel *licenceText = new QLabel( scrollLicence );
	scrollLicence->addChild( licenceText );
	QString licence;
	QFile licenceFile( KTOON_HOME+"/data/COPYING" );
	if ( licenceFile.open( QIODevice::ReadOnly ) )
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
	
	setMaximumSize( m_credits->size() );
}

About::~About()
{
}


