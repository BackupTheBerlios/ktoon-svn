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
#include "dglobal.h"
#include "dimageeffect.h"

#include "ddebug.h"

#define DEBUG_ABOUT 1

KTAbout::KTAbout( QWidget *parent ) : DTabDialog( Cancel, parent )
{
//	Q_INIT_RESOURCE(ktui_images);
	
// 	tabWidget()->setTabPosition(QTabWidget::West);
	
	setWindowTitle( tr( "About" ) + QString( " KToon..." ) );

    	//1: Credits

	QFile creditsFile( DATA_DIR+"/credits.txt" );
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
	else
	{
		dError() << "Error while trying to read " << creditsFile.fileName();
	}
	
	QImage credits = QImage(DATA_DIR+"/images/credits.png" );
	
	DImageEffect::fade(credits, 0.25, palette().background().color());
    
	m_credits = new DAnimWidget( QPixmap::fromImage(credits), creditsText );
	addTab( m_credits, tr( "Credits" ) );
	
	QPalette pal = m_credits->palette();
	pal.setColor(QPalette::Foreground, 0x686868);
	
	m_credits->setPalette(pal);
	
	m_credits->setFont(QFont("verdana", 24));

    	// 2: Ack
	
	QLabel *ack = new QLabel;
	
	QImage ackImg( DATA_DIR+"/images/sponsors.png" );
	DImageEffect::fade( ackImg,0.2,palette().background().color()); 
	
	ack->setPixmap(QPixmap::fromImage(ackImg));
	
	addTab( ack, tr("Sponsors") );

	// 4: Toonka Films

	QLabel *toonka = new QLabel;
	
	QImage toonkaImg( DATA_DIR+"/images/toonka.png" );
	DImageEffect::fade( toonkaImg,0.2,palette().background().color()); 
	
	toonka->setPixmap(QPixmap::fromImage(toonkaImg));
	addTab( toonka, "Toonka Films" );

    	// 5: Laboratoon

	QLabel *laboratoon = new QLabel;
	
	QImage laboratoonImg( DATA_DIR+"/images/laboratoon.png" );
	DImageEffect::fade( laboratoonImg,0.2,palette().background().color()); 
	
	laboratoon->setPixmap( QPixmap::fromImage(laboratoonImg  ) );
	addTab( laboratoon, "Laboratoon" );

    	// 6: Licence

	QTextBrowser *licenceText = new QTextBrowser;
// 	scrollLicence->setWidget( licenceText );
// 	QString licence = "";
	QFile licenceFile( DATA_DIR+"/COPYING" );
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
	
	setButtonText( Cancel, tr("Close"));
}

KTAbout::~KTAbout()
{
}


