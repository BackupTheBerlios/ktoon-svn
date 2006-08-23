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


#include "ktcellscolor.h"
#include "ddebug.h"
#include "ktpalettedocument.h"

#include <QFile>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QApplication>
#include <QPainter>


KTCellsColor::KTCellsColor(QWidget *parent, Type type)
	: DCellView(16, parent), m_type(type), m_readOnly(false)
{
	setAcceptDrops(true);
}


KTCellsColor::~KTCellsColor()
{
	
}


void KTCellsColor::setReadOnly(bool enable)
{
	m_readOnly = enable;
}

bool KTCellsColor::isReadOnly()
{
	return m_readOnly;
}

void KTCellsColor::setType(Type type)
{
	m_type = type;
}

int KTCellsColor::type()
{
	return m_type;
}

QString KTCellsColor::name() const
{
	return m_name;
}

void KTCellsColor::setName(const QString& name)
{
	m_name = name;
}

void KTCellsColor::save( const QString &path)
{
	QFile save(path);
	KTPaletteDocument document(m_name, true);
	
	for(int i = 0; i < columnCount() ; i++)
	{
		for (int  j = 0; j < rowCount() ; j++)
		{
			QTableWidgetItem *tmpItem = itemAt(i*25, j*25);
			if(tmpItem)
			{
				if(tmpItem->background().gradient())
				{
					document.addGradient(*tmpItem->background().gradient());
				}
				else if(tmpItem->background().color().isValid())
				{
					document.addColor(tmpItem->background().color());
				}
			}
		}
	}
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << document.toString();
		save.close();
	}
}

void KTCellsColor::dragEnterEvent( QDragEnterEvent *event )
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

void KTCellsColor::dropEvent( QDropEvent *event )
{
	if (event->mimeData()->hasColor())
	{
		QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
		
		// TODO: crear item
		
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

void KTCellsColor::mousePressEvent(QMouseEvent* e)
{
	DCellView::mousePressEvent(e);
	m_startDragPosition = e->pos();
	
}

void KTCellsColor::mouseMoveEvent(QMouseEvent* e)
{
	DCellView::mouseMoveEvent(e);
	
	if ((e->pos() - m_startDragPosition).manhattanLength() <  QApplication::startDragDistance() || !currentItem() )
		return;

	QDrag *drag = new QDrag( this );
	QPixmap pix( 25, 25 );
	QColor color=  currentItem()->background().color();
	pix.fill( color);
	
	QPainter painter( &pix );
	painter.drawRect( 0, 0, pix.width(), pix.height() );
	painter.end();
		
	QMimeData *mimeData = new QMimeData;
	mimeData->setColorData(currentItem()->background().color());
		
	drag->setMimeData(mimeData);
	drag->setPixmap( pix );
		
	Qt::DropAction dropAction = drag->start(Qt::MoveAction);

}




