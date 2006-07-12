/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#include "ktexposuretable.h"

#include<QHeaderView>
#include <QPainter>
#include <QStyleOptionButton>
#include <QMap>
#include <QVector>
#include <QItemDelegate>

#include <ddebug.h>

struct LayerItem
{
	QString title;
	int lastFrame;
	bool isVisible;
};

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.co>
 */
class KTExposureHeader: public QHeaderView
{
	Q_OBJECT;
	public:
		KTExposureHeader(QWidget * parent = 0 );
		~KTExposureHeader();
		void paintSection(QPainter *painter, const QRect & rect, int logicalIndex ) const;
		void setLabel(int logicalIndex, const QString &text);
		void advanceFrame(int logicalIndex);
		int lastFrame(int logicalIndex);
		
	private slots:
		void changeVisibility(int section);
		
	private:
		QVector<LayerItem> m_layers;
};

KTExposureHeader::KTExposureHeader(QWidget * parent ) : QHeaderView(Qt::Horizontal , parent)
{
	setClickable(true);
// 	setCascadingSectionResizes(true );
	connect(this, SIGNAL(sectionClicked(int)), this, SLOT(changeVisibility(int)));
}


KTExposureHeader::~KTExposureHeader()
{
}

void KTExposureHeader::changeVisibility(int section)
{
	m_layers[section].isVisible = !m_layers[section].isVisible;
	updateSection(section);
}

void KTExposureHeader::setLabel(int logicalIndex, const QString &text)
{
	LayerItem layer;
	layer.title = text;
	layer.lastFrame = 0;
	layer.isVisible = true;
	
	m_layers.insert(logicalIndex, layer);
}


int KTExposureHeader::lastFrame( int logicalIndex)
{
	return m_layers[logicalIndex].lastFrame;
}

void KTExposureHeader::advanceFrame(int logicalIndex)
{
	m_layers[logicalIndex].lastFrame++;
}

void KTExposureHeader::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
	if ( !rect.isValid() ) return;
	
	int height = rect.height() - 5;
	
	painter->drawRect( rect.normalized().adjusted(0, 1, 0, -1) );
	
	QString text = m_layers[logicalIndex].title;
	QFontMetrics fm( painter->font() );
	
	int x = rect.x() + (sectionSize(logicalIndex) - fm.width( text ))/2;
	int y = fm.height() + ( rect.y() / 2);
	
	painter->drawText( x, y, text);
	
	
	QStyleOptionButton buttonOption;
	
	if ( m_layers[logicalIndex].isVisible )
	{
		buttonOption.palette.setBrush(QPalette::Button, Qt::green);
		
	}
	else
	{
		buttonOption.palette.setBrush(QPalette::Button, Qt::red);
		buttonOption.state |= QStyle::State_Sunken;
	}
	
	buttonOption.rect = QRect(rect.x()+3, rect.y()+3, height, height);
	style()->drawControl( QStyle::CE_PushButton, &buttonOption, painter);
}

#include "ktexposuretable.moc"

/////////////////

class KTExposureItemDelegate : public QItemDelegate
{
	public:
		KTExposureItemDelegate(QObject * parent = 0 );
		~KTExposureItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

KTExposureItemDelegate::KTExposureItemDelegate(QObject * parent) :  QItemDelegate(parent)
{
}

KTExposureItemDelegate::~KTExposureItemDelegate()
{
}

void KTExposureItemDelegate::paint ( QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QItemDelegate::paint(painter, option, index);
	
	KTExposureTable *table = qobject_cast<KTExposureTable *>(index.model()->parent());
	
	QTableWidgetItem *item = table->itemFromIndex(index);
	
	if ( item )
	{
		if ( item->data(KTExposureTable::IsLocked).toBool() )
		{
			
		}
		
		if ( item->data(KTExposureTable::IsUsed).toBool() )
		{
			painter->drawRoundRect( option.rect );
		}
	}
}


////////////////////////


KTExposureTable::KTExposureTable(QWidget * parent) : QTableWidget(parent)
{
// 	setEditTriggers( QAbstractItemView::NoEditTriggers );
	
	setItemDelegate(new KTExposureItemDelegate(this));
	
	QTableWidgetItem *prototype = new QTableWidgetItem();
	
	prototype->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable );
	
	prototype->setTextAlignment(Qt::AlignCenter);
	
	setItemPrototype (prototype);
	
// 	setColumnCount( 10 );
	setRowCount( 10 );
	
	m_header = new KTExposureHeader(this);
	
	setHorizontalHeader( m_header);
	connect(this, SIGNAL(cellClicked ( int, int )), this, SLOT(emitRequestSetUsedFrame(int, int)));
}


KTExposureTable::~KTExposureTable()
{
}

int KTExposureTable::currentLayer() const
{
	return currentColumn();
}

int KTExposureTable::currentFrame() const
{
	QTableWidgetItem *frame = currentItem ();
	if(frame)
	{
		if(frame->data(IsUsed).toBool())
		{
			return currentRow();
		}
		else
		{
			return currentRow()-1;
		}
	}
	return currentRow()-1;
}

void KTExposureTable::insertLayer(int index, const QString & name)
{
	insertColumn ( index );
	setCurrentCell ( 0, index );
	m_header->setLabel( index, name );
}

void KTExposureTable::setUseFrame(int indexLayer, int indexFrame, const QString & name)
{
	D_FUNCINFO;
	
	QTableWidgetItem * frame = new QTableWidgetItem;
	
	frame->setBackgroundColor (Qt::gray );
	frame->setText(name);
	frame->setData(IsUsed, true);
	frame->setTextAlignment(Qt::AlignCenter);
	
// 	m_numUsed++;
	
	m_header->advanceFrame(indexLayer);
	setItem(indexFrame, indexLayer, frame);
	
	setCurrentCell(indexFrame, indexLayer);
	
	if(m_header->lastFrame(indexLayer)  == rowCount ())
	{
		setRowCount( m_header->lastFrame(indexLayer) +10 );
	}
}

void KTExposureTable::removeLayer(int indexLayer )
{
}

void KTExposureTable::removeFrame(int indexLayer, int indexFrame)
{
}

void KTExposureTable::moveFrame(  int oldPos, int newPos )
{
}

void KTExposureTable::setLockFrame(int indexLayer, int indexFrame, bool locked)
{
	
}

void KTExposureTable::emitRequestSetUsedFrame( int indexFrame,  int indexLayer)
{
	D_FUNCINFO;
	dDebug() << indexFrame << " " << m_header->lastFrame(indexLayer) << " " <<  indexLayer;
	if(indexFrame == m_header->lastFrame(indexLayer))
	{
		emit requestSetUsedFrame(indexLayer, indexFrame);
	}
}

int KTExposureTable::numUsed() const
{
	return m_header->lastFrame( currentLayer() );
}

bool KTExposureTable::edit( const QModelIndex & index, EditTrigger trigger, QEvent *event )
{
	QTableWidgetItem *item = itemFromIndex(index);
	
	if ( item )
	{
		if ( item->data(IsUsed).toBool() )
		{
			QTableWidget::edit(index, trigger, event);
		}
		else
		{
			return false;
		} 
	}
	
	return false;
}

