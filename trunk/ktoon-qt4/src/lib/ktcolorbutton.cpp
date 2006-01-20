/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktcolorbutton.h"

#include <QColorDialog>
#include <QPalette>
#include <QCursor>
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QStyle>
#include <QStyleOptionButton>

#include <QPixmap>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>


KTColorButton::KTColorButton(QWidget* parent) : QAbstractButton( parent )
{
	setMinimumSize( minimumSizeHint() );
	setAcceptDrops( true );
	setCursor(QCursor(Qt::PointingHandCursor));

	connect( this, SIGNAL(clicked()), SLOT(showEditor()));
}


KTColorButton::~KTColorButton()
{
}


/*
 * Sets the color of this button
 */
void KTColorButton::setColor( const QColor& color )
{
	m_color = color;
	update();
}


/*
 * Returns the color of this button
 */
QColor KTColorButton::color() const
{
	return m_color;
}


/*
 * Returns the size hint for this widget.
 */
QSize KTColorButton::sizeHint() const
{
	return QSize( 50, 25 );
}


/*
 * Returns the minumum size hint for this widget.
 */
QSize KTColorButton::minimumSizeHint() const
{
	return QSize( 50, 25 );
}


/*
 * Draws the button.
 */
void KTColorButton::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	
	QStyleOptionButton opt;
	if ( isChecked() )
	{
		opt.state |= QStyle::State_Raised;
	}
	
	opt.rect = rect();
	
	opt.palette.setColor(QPalette::Button, m_color);
	opt.palette.setColor(QPalette::Background, m_color);
	
	style()->drawControl(QStyle::CE_PushButtonBevel, &opt, &painter, this);
}

/*
 * Shows a color selection dialog for editing the button color.
 */
void KTColorButton::showEditor()
{
	QColor c = QColorDialog::getColor( palette().background().color(), this );

	if (!c.isValid())
		return;

	setColor( c );
	
	m_color = c;
	emit colorChanged(c);
}


/*
 * Handles the mouse press event.
 */
void KTColorButton::mousePressEvent(QMouseEvent* e)
{
	QAbstractButton::mousePressEvent(e);
	m_position = e->pos();
}


#ifndef QT_NO_DRAGANDDROP

/*
 * Draws a rectangle with the buttons color as the mouse is dragged.
 */
void KTColorButton::mouseMoveEvent(QMouseEvent* e)
{
	QAbstractButton::mouseMoveEvent( e );

	if ((e->pos() - m_position).manhattanLength() <  QApplication::startDragDistance())
		return;

	QDrag *drag = new QDrag( this );
	QPixmap pix( 25, 25 );
	pix.fill( m_color );
		
	QPainter painter( &pix );
	painter.drawRect( 0, 0, pix.width(), pix.height() );
	painter.end();
		
	QMimeData *mimeData = new QMimeData;
	mimeData->setColorData(m_color);
		
	drag->setMimeData(mimeData);
	drag->setPixmap( pix );
		
	Qt::DropAction dropAction = drag->start(Qt::MoveAction);

}


/*
 * Accepts or ignores a drag enter event.
 */
void KTColorButton::dragEnterEvent( QDragEnterEvent *event )
{
	setFocus();

	if (event->mimeData()->hasColor()) 
	{
		if (event->source() == this) 
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else
		{
			event->acceptProposedAction();
		}
	} 
	else 
	{
		event->ignore();
	}
}

void KTColorButton::setPalette ( const QPalette & pal)
{
	m_color = pal.button().color();
}

/*
 * Accepts or ignores a drag move event.
 */
void KTColorButton::dragMoveEvent( QDragMoveEvent *event )
{
	if ( event->mimeData()->hasColor() )
	{
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
	}
}


/*
 * If a color has been dragged, it will become the new color of this button.
 */
void KTColorButton::dropEvent( QDropEvent *event )
{
	if (event->mimeData()->hasColor())
	{
		QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
		setColor( color );

		if (event->source() == this) 
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else 
		{
			event->acceptProposedAction();
		}
	} 
	else 
	{
		event->ignore();
	}
}

#endif // QT_NO_DRAGANDDROP
