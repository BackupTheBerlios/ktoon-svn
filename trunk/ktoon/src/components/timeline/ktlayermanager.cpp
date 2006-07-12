/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktlayermanager.h"

#include "dapplication.h"
#include <qlabel.h>
#include <qlayout.h>

#include <QPixmap>
#include <QHeaderView>
#include <QPainter>
#include <QItemDelegate>

#include "dseparator.h"

#include "ddebug.h"


// Header

class KTLayerManagerHeader : public QHeaderView
{
	public:
		KTLayerManagerHeader(QWidget * parent = 0);
		 ~KTLayerManagerHeader();
		 
		 void paintSection(QPainter *painter, const QRect & rect, int logicalIndex ) const;
		 
	private:
		QPixmap m_lockIcon;
		QPixmap m_viewIcon;
};

KTLayerManagerHeader::KTLayerManagerHeader(QWidget * parent ) : QHeaderView(Qt::Horizontal , parent)
{
	setClickable(true);
	setCascadingSectionResizes(true);
	
	m_lockIcon = QPixmap(THEME_DIR+"/icons/lock.png");
	m_viewIcon = QPixmap(THEME_DIR+"/icons/show_hide_all_layers.png");
}


KTLayerManagerHeader::~KTLayerManagerHeader()
{
}

void KTLayerManagerHeader::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
	if ( !rect.isValid() ) return;
	
	painter->drawRect( rect.normalized().adjusted(0, 1, 0, -1) );
	
	QString text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();;
	
	QFontMetrics fm( painter->font() );
	
	int x = rect.x() + (sectionSize(logicalIndex) - fm.width( text ))/2;
	int y = fm.height() + ( rect.y() / 2);
	
	painter->drawText( x, y, text);
	
	QPen originalPen = painter->pen();
	
	painter->setPen(QPen(palette().text().color(), 2) );
	painter->drawRect(rect.normalized().adjusted(1, 1, -2, -2));
	
	
	painter->setPen(originalPen);
	switch(logicalIndex)
	{
		case 0:
		{
			// nothing...
		}
		break;
		case 1:
		{
			painter->drawPixmap( rect.normalized().adjusted(3,3, -3, -3), m_lockIcon );
		}
		break;
		case 2:
		{
			painter->drawPixmap( rect.normalized().adjusted(3, 3, -3, -3), m_viewIcon );
		}
		break;
	}
}


// DELEGATE


class KTLayerManagerItemDelegate : public QItemDelegate
{
	public:
		KTLayerManagerItemDelegate(QObject * parent = 0 );
		~KTLayerManagerItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

KTLayerManagerItemDelegate::KTLayerManagerItemDelegate(QObject * parent) :  QItemDelegate(parent)
{
}

KTLayerManagerItemDelegate::~KTLayerManagerItemDelegate()
{
}

void KTLayerManagerItemDelegate::paint ( QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	
	QItemDelegate::paint(painter, option, index);
	
	KTLayerManager *table = qobject_cast<KTLayerManager *>(index.model()->parent());
	
	QTableWidgetItem *item = table->itemFromIndex(index);
	
	if ( item )
	{
		switch( index.column() )
		{
			case 0:
			{
				if ( item->isSelected() )
				{
					painter->drawRect(option.rect.normalized().adjusted(1,1,-2, -2 ));
				}
			}
			break;
			case 1:
			case 2:
			{
				QStyleOptionButton checkOption;
		
				checkOption.state = option.state;
		
				if ( item->checkState() == Qt::Checked )
				{
					checkOption.state |= QStyle::State_On;
				}
		
				checkOption.rect = option.rect.normalized().adjusted(2,2,-2,-2);
				
				table->style()->drawPrimitive ( QStyle::PE_IndicatorCheckBox, &checkOption, painter);
			}
			break;
		}
	}
}



////////////////////////////////

KTLayerManager::KTLayerManager(QWidget *parent) : QTableWidget(0, 3, parent), m_allSelected(false), m_allVisible(true), m_allLock(false), m_rowHeight(20)
{
	DINIT;
	
	QTableWidgetItem *prototype = new QTableWidgetItem;
	prototype->setTextAlignment(Qt::AlignCenter);
	prototype->setBackgroundColor( palette().text().color() );
	prototype->setTextColor(palette().background().color() );
	
	setItemPrototype(prototype);
	
	setHorizontalHeaderLabels(QStringList() << tr("Layer") << tr("L") << tr("V") );
	verticalHeader()->hide();
	
	setHorizontalHeader(new KTLayerManagerHeader(this));
	setItemDelegate(new KTLayerManagerItemDelegate(this));
}


KTLayerManager::~KTLayerManager()
{
	DEND;
}


void KTLayerManager::insertLayer(int position, const QString &name)
{
	QTableWidgetItem *newLayer = new QTableWidgetItem(name);
	newLayer->setTextAlignment(Qt::AlignCenter);
	
	if (  position >= 0 && position <= rowCount())
	{
		newLayer->setBackgroundColor( palette().background().color() );
		newLayer->setTextColor(palette().foreground().color() );
		
		insertRow (position);
		setItem(position, 0, newLayer);
		fixSize();
		
		QTableWidgetItem *lockItem = new QTableWidgetItem;
		lockItem->setFlags(Qt::ItemIsUserCheckable  | Qt::ItemIsEnabled);
		lockItem->setCheckState( Qt::Unchecked);
		
		setItem(position, 1, lockItem);
		
		QTableWidgetItem *viewItem = new QTableWidgetItem;
		viewItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		viewItem->setCheckState( Qt::Checked);
		
		setItem(position, 2, viewItem);

	}
	else
	{
		delete newLayer;
	}
}

void KTLayerManager::removeLayer(int position)
{
	D_FUNCINFO << position;
	removeRow(position);
}


void KTLayerManager::resizeEvent( QResizeEvent *)
{
	fixSize();
}

void KTLayerManager::fixSize()
{
	int offset = 0;
	if ( verticalScrollBar()->isVisible() )
	{
		offset = verticalScrollBar()->width()-2;
	}
	else
	{
		offset = -2;
	}
	
	int width = this->width() - offset;
		
	horizontalHeader()->resizeSection(0, width-(m_rowHeight*2)-8 );
	horizontalHeader()->resizeSection(1, m_rowHeight );
	horizontalHeader()->resizeSection(2, m_rowHeight );
	
	for(int row = 0; row < rowCount(); row++)
	{
		verticalHeader()->resizeSection(row, m_rowHeight);
	}
}


void KTLayerManager::setRowHeight(int rowHeight)
{
	m_rowHeight = rowHeight;
}


