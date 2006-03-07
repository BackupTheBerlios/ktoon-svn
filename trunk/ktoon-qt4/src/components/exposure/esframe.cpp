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
#include <QApplication>
#include <QToolTip>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QPalette>
#include "ddebug.h"
#include "esframe.h"

//--------------- CONSTRUCTOR --------------------

ESFrame::ESFrame( int id, QWidget *parent )
	: DSqueezeLabel( parent )  ,  is_used(false), is_selected(false), is_locked(false),  is_motion(false), has_drawing(false), m_id(id) /*m_initialText(initial_text)*/
{
// 	DINIT;
    //Initializations
	setFrameShadow ( QFrame::Raised  );
	setFrameShape ( QFrame::Panel );
	setLineWidth( 2 );
	description = new QLineEdit( /*initial_text ,*/ this );
	description->hide();
	connect( description, SIGNAL( lostFocus() ), SLOT( slotSetDescription() ) );
	connect( description, SIGNAL( returnPressed() ), SLOT( slotSetDescription() ) );
#if QT_VERSION >= 0x040100
	setAutoFillBackground(true);
#endif
	setMinimumSize(sizeHint());
	
}

//--------------- DESTRUCTOR --------------------

ESFrame::~ESFrame()
{
// 	DEND;
}

//-------------- PUBLIC MEMBERS ----------------

QString ESFrame::name()
{
	return m_initialText;
}

bool ESFrame::isUsed()
{
	return is_used;	
}

bool ESFrame::isSelected()
{
	return is_selected;
}

bool ESFrame::isLocked()
{
	return is_locked;
}

bool ESFrame::isMotion()
{
	return is_motion;
}

bool ESFrame::hasDrawing()
{
	return has_drawing;
}

int ESFrame::id()
{
	return m_id;
}

void ESFrame::setId(int id)
{
	m_id = id;
}

void ESFrame::setUsed( bool in_is_used )
{
	is_used = in_is_used;
	setName(m_initialText);
	QPalette pal = palette();
#if 0
	QColor fg;
	if(parentWidget())
	{
		fg = parentWidget()->palette().color(QPalette::Foreground);
	}
	else
	{
		fg = palette().color(QPalette::Foreground);
	}
	
	if ( is_selected == true && in_is_used == true )
	{
		pal.setColor( QPalette::Background, palette().color( QPalette::Highlight).light(200)) ; 
		setPalette(pal);
		setName(m_initialText);
	}
	else if ( is_selected == true && in_is_used == false )
	{
		pal.setColor( QPalette::Foreground, QColor( 255, 255, 255 ));
		pal.setColor( QPalette::Foreground, pal.color( QPalette::Dark));
		setPalette(pal);
	}
	else if ( is_selected == false && in_is_used == true )
	{
		pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight));
		pal.setColor( QPalette::Foreground, QColor( 255, 255, 255 ));
		setPalette(pal);
	}
	else
	{
		pal.setColor( QPalette::Background, QColor( 239, 237, 223 ));
		pal.setColor( QPalette::Foreground, fg);
		setPalette(pal);
	}
#endif
}

void ESFrame::setSelected( bool in_is_selected )
{
	is_selected = in_is_selected;
	QPalette pal = palette();
	QColor fg;
	if(parentWidget())
	{
		fg = parentWidget()->palette().color(QPalette::Foreground);
	}
	else
	{
		fg = palette().color(QPalette::Foreground);
	}
	
	if ( in_is_selected == true && is_used == true && is_locked == false )
	{
		pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight).light(200));
		pal.setColor( QPalette::Foreground, fg);
	}
	else if ( in_is_selected == true && is_used == false && is_locked == false )
	{
		pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight).dark(200));
	}
	else if ( in_is_selected == false && is_used == true && is_locked == false )
	{
		pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight).light(130));
// 		setPaletteBackgroundColor( palette().color(QPalette::Active , QColorGroup::Highlight).light(130) );
// 		setPaletteForegroundColor( QColor( 0, 0, 0 ) );
		pal.setColor( QPalette::Foreground, fg);
	}
	else if ( in_is_selected == false && is_used == false && is_locked == false )
	{
		if( parentWidget())
		{
			pal =  parentWidget()->palette();
		}
// 		setPaletteForegroundColor( parentWidget()->palette().color(QPalette::Background));
// 		setPaletteBackgroundColor( parentWidget()->paletteBackgroundColor() );
	}
	else if ( is_locked && in_is_selected )
	{
		pal.setColor( QPalette::Background, pal.color(QPalette::Link));
		pal.setColor( QPalette::Foreground, fg);
// 		setPaletteForegroundColor( QColor( 0, 0, 0 ) );
	}
	else if ( is_locked && !in_is_selected )
	{
		pal.setColor( QPalette::Background, pal.color(QPalette::LinkVisited));
		pal.setColor( QPalette::Foreground, fg);
// 		setPaletteForegroundColor( QColor( 0, 0, 0 ) );
	}
	setPalette(pal);
}

void ESFrame::setLocked( bool in_is_locked )
{

	
	if ( is_used )
	{
		is_locked = in_is_locked;
		QPalette pal = palette();
		QColor fg;
		if(parentWidget())
		{
			fg = parentWidget()->palette().color(QPalette::Foreground);
		}
		else
		{
			fg = palette().color(QPalette::Foreground);
		}
		if ( is_selected && is_locked )
		{
			pal.setColor( QPalette::Background, pal.color(QPalette::Link));
			pal.setColor( QPalette::Foreground, fg);
			
// 			setPaletteBackgroundColor( colorGroup().link());
// 			setPaletteForegroundColor( QColor( 0, 0, 0 ) );
		}
		else if ( is_selected && !is_locked )
		{
			pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight).light(200));
// 			setPaletteBackgroundColor( palette().color(QPalette::Active , QColorGroup::Highlight).light(200) );
		}
		else if ( !is_selected && is_locked )
		{
			pal.setColor( QPalette::Background, pal.color(QPalette::LinkVisited));
			pal.setColor( QPalette::Foreground, fg);
			
		}
		else
		{
			pal.setColor( QPalette::Background, QColor( 200, 200, 200 ) );
			pal.setColor( QPalette::Foreground, fg);
		}
		setPalette(pal);
	}

}

void ESFrame::setMotion( bool in_is_motion )
{
	is_motion = in_is_motion;
	update();
}

void ESFrame::setHasDrawing( bool in_has_drawing )
{
// 	dDebug(1) << "void ESFrame::setHasDrawing(" << in_has_drawing << ")";
	has_drawing = in_has_drawing;
	update();
}

void ESFrame::setName( const QString &new_name )
{
	dDebug() << "void ESFrame::setName( const QString &new_name )";
	if(m_initialText != new_name)
	{
		m_initialText = new_name;
		description->setText(new_name);
		setText( new_name );
	}
}

void ESFrame::clearTextfieldFocus()
{
	description -> clearFocus();
}

void ESFrame::setAllProperties( ESFrame *in_esframe )
{
	setUsed( in_esframe -> isUsed() );
	setLocked( in_esframe -> isLocked() );
	setText( in_esframe -> text() );
	setSelected( in_esframe -> isSelected() );
	setMotion( in_esframe -> isMotion() );
	setHasDrawing( in_esframe -> hasDrawing() );
}

//-------------- SLOTS --------------------

void ESFrame::slotSetDescription()
{
	if(m_initialText != description->text())
	{
		setText( description -> text() );
		emit renamed( m_id, description->text() );
		m_initialText = description -> text();
	}
	description->hide();
}


void ESFrame::otherSelected(int id)
{
	if(m_id != id)
	{
		setSelected(false);
		slotSetDescription();
		description->setReadOnly(true);
	}else
	{
		setSelected( true);
		description->setReadOnly(false);
	}
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void ESFrame::mousePressEvent( QMouseEvent *mouse_event )
{
// 	QToolTip::showText(mapToGlobal ( mouse_event->pos()), "offset " + QString::number(m_id), this);
	Q_CHECK_PTR( mouse_event );
	QPalette pal = palette();
	QColor fg;
	if(parentWidget())
	{
		fg = parentWidget()->palette().color(QPalette::Foreground);
	}
	else
	{
		fg = palette().color(QPalette::Foreground);
	}
	if ( !is_used )
	{
		is_selected = true;
		pal.setColor( QPalette::Background, pal.color(QPalette::Dark));
		pal.setColor( QPalette::Foreground, pal.color(QPalette::Dark));
		
// 		setPaletteBackgroundColor( QColor( 0, 0, 0 ) );
// 		setPaletteForegroundColor( QColor( 255, 255, 255 ) );
	}
	else if ( is_used && !is_locked )
	{
		is_selected = true;
		pal.setColor( QPalette::Background, palette().color(QPalette::Active , QPalette::Highlight).light(200));
		pal.setColor( QPalette::Foreground, fg);
// 		setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
// 		setPaletteForegroundColor( QColor( 0, 0, 0 ) );
	}
	else if ( is_locked )
	{
		is_selected = true;
		pal.setColor( QPalette::Background, pal.color(QPalette::LinkVisited));
		pal.setColor( QPalette::Foreground, fg);
	}

	emit clicked(m_id, mouse_event->button(), mouse_event->globalX(), mouse_event->globalY() );
	emit selected();
// 	if ( mouse_event->button() == Qt::RightButton && is_used )
// 	{
// 		right_click_menu -> setItemVisible( id_rename, true );
// 		right_click_menu -> setItemVisible( id_remove, true );
// 		right_click_menu -> setItemVisible( id_lock, true );
// 		right_click_menu -> setItemVisible( id_copy, true );
// 		right_click_menu -> setItemVisible( id_paste, true );
// 		right_click_menu -> exec( QCursor::pos() );
// 	}
// 	else if ( mouse_event -> button() == Qt::RightButton && !is_used )
// 	{
// 		right_click_menu -> setItemVisible( id_rename, false );
// 		right_click_menu -> setItemVisible( id_remove, false );
// 		right_click_menu -> setItemVisible( id_lock, false );
// 		right_click_menu -> setItemVisible( id_copy, false );
// 		right_click_menu -> setItemVisible( id_paste, true );
// 		right_click_menu -> exec( QCursor::pos() );
// 	}

	mouse_event -> accept();

}

void ESFrame::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	if ( is_used == true && mouse_event -> button() == Qt::LeftButton )
	{
		description -> show();
		description -> setText( completeText());
		description -> setFocus();
		mouse_event -> accept();
	}
	else
	{
		mouse_event -> ignore();
	}
}

void ESFrame::drawContents( QPainter *painter )
{
	
	Q_CHECK_PTR( painter );
	if ( has_drawing )
	{
		painter -> setPen( QColor( 170, 70, 10 ) );
	}
	else
	{
		if(parentWidget())
		{
			painter->setPen(parentWidget()->palette().color(QPalette::Foreground));
		}
		else
		{
			painter->setPen( QColor( 0, 0, 0 ) );
		}
	}
	
	painter -> drawText( 5, 16, text() );
	if ( is_motion )
	{
		painter -> setPen( QPen( QColor( 120, 120, 120 ), 2 ) );
		painter -> drawLine( 4, 14, 4, 24 );
		painter -> drawLine( 4, 14, 8, 14 );
	}
}
void ESFrame::resizeEvent ( QResizeEvent * e )
{
	description -> resize( width(), height() );
	DSqueezeLabel::resizeEvent(e);
}

void ESFrame::keyPressEvent(QKeyEvent *)
{

}

void ESFrame::slotSendDoubleClickEvent()
{
	QMouseEvent mouse_event( QEvent::MouseButtonDblClick, QPoint( x(), y() ), Qt::LeftButton,  Qt::NoButton , Qt::NoModifier );
	QApplication::sendEvent( this, &mouse_event );
}

