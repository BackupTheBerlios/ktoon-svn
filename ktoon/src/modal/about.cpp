/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#include "about.h"

//------------------ CONSTRUCTOR -----------------

About::About( QWidget *parent ) : QTabDialog( parent )
{
    Q_CHECK_PTR( parent );
    
    setCaption( tr( "About" ) + QString( " Ktoon..." ) );
    setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    resize( 580, 400 );
    setMaximumSize( 580, 400 );
    setMinimumSize( 580, 400 );
    setFont( QFont( "helvetica", 10 ) );
    parent_widget = parent;

    //-- 1: CREDITS --

    credits = new QLabel( this );
    file_id = 1;
    credits_painter = new QPainter();
    credits_pixmap = QPixmap();
    credits_pixmap.load( "src/images/ani/credits/credits-image.png" );
    credits -> setPixmap( credits_pixmap );

    QFile credits_file( "src/images/ani/credits/credits-text.txt" );
    if ( credits_file.open( IO_ReadOnly ) )
    {
        QTextStream stream( &credits_file );
        while ( !stream.atEnd() )
            credits_text = credits_text + stream.readLine() + QString( "\n" );
        credits_file.close();
    }

    credits_timer = new QTimer( this );
    connect( credits_timer, SIGNAL( timeout() ), SLOT( slotDisplayCreditsPixmap() ) );
    addTab( credits, tr( "Credits" ) );

    //-- 2: ACKNOWLEDGEMENTS --

    ack = new QLabel( this );
    file_id2 = 1;
    ack_pixmap = QPixmap();
    ack_pixmap.load( "src/images/ani/ack/ack-image1.png", "PNG" );
    ack -> setPixmap( ack_pixmap );
    ack_timer = new QTimer( this );
    connect( ack_timer, SIGNAL( timeout() ), SLOT( slotDisplayAcknowledgementsPixmap() ) );
    addTab( ack, tr( "Acknowledgements" ) );

    //-- 3: SVN LOG FILE --

    scroll_container_log = new QScrollView( this );
    scroll_log = new QFrame( scroll_container_log -> viewport() );
    scroll_container_log -> addChild( scroll_log );

    text_log = new QLabel( scroll_log );
    QString lines;
    QFile log_file( "svnlog.txt" );
    int new_height = 12000;
    if ( log_file.open( IO_ReadOnly ) )
    {
        int line_count = 0;
        QTextStream stream( &log_file );
        while ( !stream.atEnd() )
	{
	    QString new_line = stream.readLine();
	    QString copy = new_line;
	    if ( !copy.replace( " ", "" ).isEmpty() )
            {
	        lines = lines + new_line + QString( "\n" );
	        line_count++;
	    }
	}
        log_file.close();
	new_height = line_count * 17;
    }
    text_log -> setText( lines );
    scroll_log -> resize( 600, new_height );
    text_log -> resize( 600, new_height );
    addTab( scroll_container_log, tr( "SVN Log" ) );

    //-- 4: ABOUT TOONKA FILMS --

    toonka = new QLabel( this );
    toonka_pixmap = QPixmap( "src/images/ani/toonka.jpg" );
    toonka -> setPixmap( toonka_pixmap );
    addTab( toonka, "Toonka Films" );

    //-- 4: ABOUT LABORATOON --

    laboratoon = new QLabel( this );
    laboratoon_pixmap = QPixmap( "src/images/ani/laboratoon.jpg" );
    laboratoon -> setPixmap( laboratoon_pixmap );
    addTab( laboratoon, "Laboratoon" );

    //-- 6: GNU PUBLIC LICENSE --

    scroll_container_gnu = new QScrollView( this );
    scroll_gnu = new QFrame( scroll_container_gnu -> viewport() );
    scroll_gnu -> resize( 600, 6000 );
    scroll_container_gnu -> addChild( scroll_gnu );

    text_gnu = new QLabel( scroll_gnu );
    text_gnu -> resize( 600, 6000 );
    QString lines2;
    QFile gnu_file( "COPYING" );
    if ( gnu_file.open( IO_ReadOnly ) )
    {
        QTextStream stream( &gnu_file );
        while ( !stream.atEnd() )
            lines2 = lines2 + stream.readLine() + QString( "\n" );
        gnu_file.close();
    }
    text_gnu -> setText( lines2 );
    addTab( scroll_container_gnu, tr( "License Agreement" ) );

    connect( this, SIGNAL( currentChanged( QWidget * ) ), SLOT( slotPerformPageChangeActions( QWidget * ) ) );
}

//---------------- DESTRUCTOR -------------------

About::~About()
{
    delete credits_timer;
    delete credits_painter;
    delete credits;
    delete ack;
    delete ack_timer;
    delete text_log;
    delete scroll_log;
    delete scroll_container_log;
    delete toonka;
    delete laboratoon;
    delete text_gnu;
    delete scroll_gnu;
    delete scroll_container_gnu;
}

//-------------- PUBLIC MEMBERS ----------------

//-------------- SLOTS ------------------

void About::slotDisplayCreditsPixmap()
{
    credits_painter -> begin( credits );
    credits_painter -> setClipping( true );
    credits_painter -> setClipRect( QRect( 0, 0, credits -> width() / 2, credits -> height() ) );
    credits_painter -> drawPixmap( 0, 0, credits_pixmap );
    credits_painter -> setClipping( false );
    credits_painter -> setPen( QPen( QColor( 0, 0, 0 ), 1 ) );
    credits_painter -> setFont( QFont( "helvetica", 12 ) );
    credits_painter -> drawText( 30, credits -> height() - file_id, credits -> width() / 2, 2000, Qt::AlignLeft, credits_text );
    file_id++;
    if ( file_id == 2100 )
        file_id = 1;
    credits_painter -> end();
}

void About::slotDisplayAcknowledgementsPixmap()
{
    QString png_file_name;
    png_file_name = QString( "src/images/ani/ack/ack-image" + QString::number( file_id2 ) + ".png" );
    if ( !ack_pixmap.load( png_file_name, "PNG" ) )
	qDebug( tr( "Couldn't load the acknowledgements animation png - " ) + png_file_name );
    ack -> setPixmap( ack_pixmap );
    if ( file_id2 == 10 )
        file_id2 = 1;
    else
        file_id2++;
}

void About::slotPerformPageChangeActions( QWidget *page_widget )
{
    Q_CHECK_PTR( page_widget );
    if ( ( QLabel * )page_widget == credits )
    {
        ack_timer -> stop();
        file_id = 1;
        credits_timer -> start( 25, false );
    }
    else if ( ( QLabel * )page_widget == ack )
    {
        credits_timer -> stop();
	file_id2 = 1;
        ack_timer -> start( 5000, false );
    }
    else
    {
	credits_timer -> stop();
	ack_timer -> stop();
    }
}

//----------- EVENTS AND PROTECTED MEMBERS ---------------
