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

#include <QPixmap>
#include <QLabel>
#include <QScrollArea>

#include <QTextBrowser>

#include "ktabout.h"
// #include "images.h"
#include "ktapplication.h"
#include "kimageeffect.h"

#include "ktdebug.h"

#define DEBUG_ABOUT 1

KTAbout::KTAbout( QWidget *parent ) : KTTabDialog( Cancel, parent )
{
	KTINIT;
	setWindowTitle( tr( "About" ) + QString( " KToon..." ) );

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
	
	QImage credits = QImage(KTOON_THEME_DIR+"/images/credits-image.png" );
	
	KImageEffect::fade(credits, 0.25, palette().background().color());
    
	m_credits = new KTAnimWidget( QPixmap::fromImage(credits), creditsText );
	addTab( m_credits, tr( "Credits" ) );

    	// 2: Ack
	ListOfPixmaps lop;

	for(uint i = 1; i < 11; i++)
	{
		QImage tmp(KTOON_THEME_DIR+QString("/sequences/ack-image%1.png").arg(i));
		KImageEffect::fade(tmp, 0.2, palette().background().color());
		lop << QPixmap::fromImage(tmp);
	}
	
	m_ack = new KTAnimWidget( lop );

	addTab( m_ack, tr( "Acknowledgements" ) );

    	// 3: Changelog

	QTextBrowser *logText = new QTextBrowser/*(scroll)*/;
	
// 	QString readText;
	QFile clFile( KTOON_HOME+"/data/Changelog" );
	
	if ( clFile.open( QIODevice::ReadOnly | QIODevice::Text) )
	{
		QTextStream stream( &clFile );
		while ( ! stream.atEnd() )
		{
			QString text = stream.readLine();
// 			readText += text+"\n";
			logText->append(text);
		}
		clFile.close();
	}
// 	logText -> setText( readText );
	
	addTab( logText, tr( "Changelog" ) );

    	// 4: Toonka Films

	QLabel *toonka = new QLabel/*( this )*/;
	
	QImage toonkaImg( KTOON_THEME_DIR+"/images/toonka.png" );
	KImageEffect::fade( toonkaImg,0.2,palette().background().color()); 
	
	toonka->setPixmap(QPixmap::fromImage(toonkaImg));
	addTab( toonka, "Toonka Films" );

    	// 5: Laboratoon

	QLabel *laboratoon = new QLabel/*( this )*/;
	
	QImage laboratoonImg( KTOON_THEME_DIR+"/images/laboratoon.png" );
	KImageEffect::fade( laboratoonImg,0.2,palette().background().color()); 
	
	laboratoon->setPixmap( QPixmap::fromImage(laboratoonImg  ) );
	addTab( laboratoon, "Laboratoon" );

    	// 6: Licence

	QTextBrowser *licenceText = new QTextBrowser/*( scrollLicence )*/;
// 	scrollLicence->setWidget( licenceText );
// 	QString licence = "";
	QFile licenceFile( KTOON_HOME+"/data/COPYING" );
	if ( licenceFile.open( QIODevice::ReadOnly  | QIODevice::Text) )
	{
		QTextStream stream( &licenceFile );
		while ( ! stream.atEnd() )
		{
			QString line = stream.readLine();
// 			licence += line+"\n";
			licenceText->append(line);
		}
		licenceFile.close();
	}
	
// 	licenceText -> setText( licence );

	addTab( licenceText, tr( "License Agreement" ) );

	setMaximumSize( m_credits->size() );
	
	setButtonText( Cancel, tr("Close"));
}

KTAbout::~KTAbout()
{
	KTEND;
}


