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

#include <QCursor>
//Added by qt3to4:
#include <QMouseEvent>
#include <QResizeEvent>
#include <QEvent>
#include "eslayer.h"

#include <QVBoxLayout>

#include "ktapplication.h"

//--------------- CONSTRUCTOR --------------------

ESLayer::ESLayer( const QString &initial_text, QWidget *parent  )
    : QPushButton( initial_text, parent )
{
    Q_CHECK_PTR( parent );
    
    QVBoxLayout *m_layout = new QVBoxLayout;
    
    m_layout->setMargin(5);
    m_layout->setSpacing(0);
    
    m_visibilityButton = new QPushButton;
    m_visibilityButton->setCheckable(true);
    m_visibilityButton->setChecked(true);
    
    connect(m_visibilityButton, SIGNAL(clicked()), this, SLOT(visibilityClick()));
    m_visibilityButton->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/show_hide_all_layers.png" ));
//     m_visibilityButton->setIconSize(m_visibilityButton->size());
    
    QPalette pal = palette();
    pal.setColor(QPalette::Button, Qt::green );
    m_visibilityButton->setPalette(pal);
    
    m_visibilityButton->setMaximumWidth(15);
    
    
    m_layout->addWidget(m_visibilityButton, 0, Qt::AlignLeft);
    
    setAutoDefault( false );
    setFocusPolicy( Qt::NoFocus );
    default_color = paletteBackgroundColor();
    selection_color = QColor( 210, 210, 255 );

    description = new QLineEdit( initial_text,this );
//     description -> resize( 66, 21 );
//     description -> move( 2, 2 );
    description -> hide();
    connect( description, SIGNAL( lostFocus() ), SLOT( slotSetDescription() ) );
    connect( description, SIGNAL( returnPressed() ), SLOT( slotSetDescription() ) );

//     right_click_menu = new QPopupMenu( this );
//     right_click_menu -> setFont( QFont( "helvetica", 10 ) );
    /*right_click_menu -> insertItem( tr( "Rename Layer" ), this, SLOT( slotSendDoubleClickEvent() ) );
    right_click_menu -> insertItem( tr( "Remove this Layer" ), grandparent, SLOT( slotRemoveLayer() ) );
    right_click_menu -> insertSeparator();
    right_click_menu -> insertItem( tr( "Insert Frames" ), grandparent, SLOT( slotInsertFrame() ) );
    right_click_menu -> insertItem( tr( "Remove Frames" ), grandparent, SLOT( slotRemoveFrame() ) );*/
    
    setLayout(m_layout);
}

//--------------- DESTRUCTOR --------------------

ESLayer::~ESLayer()
{
//     delete right_click_menu;
    delete description;
}

//-------------- PUBLIC MEMBERS ----------------


bool ESLayer::isSelected()
{
    return is_selected;
}

void ESLayer::setSelected( bool in_is_selected )
{
	QPalette pal = palette();
	is_selected = in_is_selected;
	if ( is_selected )
	{
		pal.setColor(QPalette::Button, selection_color);
// 		pal.setColor(QPalette::ButtonText, pal.highlightedText ());
		
// 		setPaletteBackgroundColor( colorGroup().dark() );
// 		setPaletteBackgroundColor( colorGroup ().highlight().light ( 100 ) );
// 		setPaletteForegroundColor( colorGroup ().highlightedText ());
	}
	else
	{
		pal.setColor(QPalette::Button, /*pal.dark()*/QApplication::palette().button() );
// 		pal.setColor(QPalette::ButtonText, colorGroup ().highlightedText ());
// 		setPaletteBackgroundColor( colorGroup ().background() );
// 		setPaletteForegroundColor( colorGroup ().text ());
	}
	
	setPalette(pal);
}

void ESLayer::clearTextfieldFocus()
{
    description -> clearFocus();
}

//-------------- SLOTS --------------------

void ESLayer::slotSetDescription()
{
	if(description -> text() != text())
	{
		setText( description -> text() );
		emit renamed(text());
	}
	m_visibilityButton->show();
	description -> hide();
}

void ESLayer::slotSendDoubleClickEvent()
{
    QMouseEvent mouse_event( QEvent::MouseButtonDblClick, QPoint( x(), y() ), Qt::LeftButton, 0 );
    QApplication::sendEvent( this, &mouse_event );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void ESLayer::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    setSelected( true );
    /*is_selected = true;
    setPaletteBackgroundColor( selection_color );*/
    
    QPalette pal = palette();
    pal.setColor(QPalette::Button, selection_color);
    setPalette(pal);
    
    description -> setText( text() );

    emit clicked(true, mouse_event );

//     emit clicked();
//     if ( mouse_event -> button() == Qt::RightButton )
//         right_click_menu -> exec( QCursor::pos() );

    mouse_event -> accept();
}

void ESLayer::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    if ( mouse_event -> button() == Qt::LeftButton )
    {
        description -> show();
	m_visibilityButton->hide();
        description -> setText( text() );
        description -> setFocus();
	mouse_event -> accept();
    }
    else
        mouse_event -> ignore();
}

void ESLayer::resizeEvent ( QResizeEvent *  )
{
	description -> resize( width(), height() );
}

void ESLayer::visibilityClick()
{
	QPalette pal = palette();
	
	bool isVisible = m_visibilityButton->isChecked();
	
	if ( isVisible )
	{
		pal.setColor(QPalette::Button, Qt::green);
	}
	else
	{
		pal.setColor(QPalette::Button, Qt::red );
	}
	m_visibilityButton->setPalette(pal);
	
	
	emit visibilityChanged( isVisible );
	
}

