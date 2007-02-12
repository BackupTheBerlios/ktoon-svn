/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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

#include <QHeaderView>
#include <QPainter>
#include <QStyleOptionButton>
#include <QMap>
#include <QVector>
#include <QItemDelegate>
#include <QLineEdit>
#include <QMouseEvent>

#include <ddebug.h>
#include <ktglobal.h>

struct LayerItem
{
	QString title;
	int lastFrame;
	bool isVisible;
	bool isLocked;
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
		void insertLayer(int logicalIndex, const QString &text);
		void setLayerName(int logicalIndex, const QString &text);
		void setLastFrame(int logicalIndex, int num);
		int lastFrame(int logicalIndex);
		void removeLayer(int logicalIndex);
		void moveLayer(int index, int newIndex);
		void setLockLayer(int logicalndex, bool lock);
		bool signalMovedBlocked();
		void setVisibilityChanged(int logicalndex, bool visibility);
		
	private slots:
		void emitVisiblityChanged(int section);
		void showEditorName(int section );
		void hideEditorName();
		
	protected:
		virtual void mousePressEvent ( QMouseEvent * event );
// 		
	private:
		QVector<LayerItem> m_layers;
		
		QLineEdit *m_editor;
		int m_sectionEdited, m_blockSectionMoved;
		
	signals:
		void changedName(int indexLayer, const QString & name );
		void visiblityChanged(int indexLayer, bool visibility );
};

KTExposureHeader::KTExposureHeader(QWidget * parent ) : QHeaderView(Qt::Horizontal , parent), m_sectionEdited(-1), m_blockSectionMoved(false)
{
	setClickable(true);
	setMovable(true);
	
// 	connect(this, SIGNAL(sectionClicked(int)), this, SLOT(emitVisiblityChanged(int)));
	connect(this, SIGNAL(sectionDoubleClicked( int )), this, SLOT(showEditorName(int)));
	
	m_editor = new QLineEdit(this);
	m_editor->setFocusPolicy( Qt::ClickFocus);
	m_editor->setInputMask("");
// 	connect (m_editor, SIGNAL( returnPressed ()), this, SLOT(hideEditorName()));
	connect (m_editor, SIGNAL( editingFinished () ), this, SLOT(hideEditorName()));
	m_editor->hide();
}


KTExposureHeader::~KTExposureHeader()
{
}

void KTExposureHeader::emitVisiblityChanged(int section)
{
	emit visiblityChanged(visualIndex(section), !m_layers[section].isVisible );
}

void KTExposureHeader::setVisibilityChanged(int logicalndex, bool visibility)
{
	m_layers[logicalndex].isVisible = !m_layers[logicalndex].isVisible;
	updateSection(logicalndex);
}

void KTExposureHeader::showEditorName(int section)
{
	int x = sectionViewportPosition ( section );
	m_editor->setGeometry(x, 0, sectionSize(section), height() );
	m_sectionEdited = section;
	m_editor->setText(m_layers[section].title);
	m_editor->show();
	m_editor->setFocus();
}

void KTExposureHeader::hideEditorName()
{
	m_editor->hide();
	
	if(m_sectionEdited != -1 && m_editor->isModified())
	{
		emit changedName(m_sectionEdited, m_editor->text());
	}
	m_sectionEdited = -1;
}

void KTExposureHeader::insertLayer(int logicalIndex, const QString &text)
{
	LayerItem layer;
	layer.title = text;
	layer.lastFrame = 0;
	layer.isVisible = true;
 	layer.isLocked = false;
	m_layers.insert(logicalIndex, layer);
}

void KTExposureHeader::setLayerName(int logicalIndex, const QString &text)
{
	m_layers[logicalIndex].title = text;
	
	updateSection(logicalIndex);
}

bool KTExposureHeader::signalMovedBlocked()
{
	return m_blockSectionMoved;
}

void KTExposureHeader::setLockLayer(int logicalndex, bool lock)
{
// 	dDebug() << "KTExposureHeader::setLockLayer( " <<  logicalndex << "," << lock << ")" ;
	m_layers[logicalndex].isLocked = lock;
	updateSection(logicalndex);
}

void KTExposureHeader::moveLayer(int position, int newPosition)
{
// 	dDebug() << "moveLayer(" << position << "," << newPosition << ")";
	m_blockSectionMoved = true;
	moveSection ( position, newPosition );
	m_blockSectionMoved = false;
}

int KTExposureHeader::lastFrame( int logicalIndex)
{
	return m_layers[logicalIndex].lastFrame;
}

void KTExposureHeader::removeLayer(int logicalIndex)
{
	m_layers.remove(logicalIndex);
}

void KTExposureHeader::setLastFrame(int logicalIndex, int num)
{
	m_layers[logicalIndex].lastFrame = num;
}

void KTExposureHeader::mousePressEvent ( QMouseEvent * event )
{
	int section = logicalIndexAt(  event->pos() );
	int x = sectionViewportPosition ( section ) + 3;
	
	QRect rect(x+3, 3, height()-3, height()-3);
	if(rect.contains(event->pos()))
	{
		emitVisiblityChanged(section);
	}
	else
	{
		QHeaderView::mousePressEvent(event);
	}
}

void KTExposureHeader::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
	if ( !rect.isValid() ) return;
	
	QStyleOptionHeader headerOption;
	headerOption.rect = rect;
	headerOption.orientation = Qt::Horizontal;
	headerOption.position = QStyleOptionHeader::Middle;
	headerOption.text = "";
	
	QStyle::State state = QStyle::State_None;
	if (isEnabled())
		state |= QStyle::State_Enabled;
	if (window()->isActiveWindow())
		state |= QStyle::State_Active;
	
	
	
	style()->drawControl ( QStyle::CE_HeaderSection, &headerOption, painter );
	
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
	
	int pxWidth = 0;
	if(m_layers[logicalIndex].isLocked)
	{
		QPixmap pixmap(HOME_DIR+"/themes/default/icons/kilit_pic.png");
		painter->drawPixmap(x+fm.width( text )+3, 3, pixmap);
		pxWidth =  pixmap.width();
	}
	
	
	buttonOption.rect = QRect(rect.x()+3, rect.y()+3, height, height);
	style()->drawControl( QStyle::CE_PushButton, &buttonOption, painter);
// 	int size = height + 3 + fm.width( text )+3 + pxWidth;
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



void KTExposureItemDelegate::paint( QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QItemDelegate::paint(painter, option, index);
	
	KTExposureTable *table = qobject_cast<KTExposureTable *>(index.model()->parent());
	
	QTableWidgetItem *item = table->itemFromIndex(index);
	
	if ( item )
	{
		if ( item->data(KTExposureTable::IsLocked).toBool() )
		{
			painter->drawLine( option.rect.topLeft(),  option.rect.bottomRight());
			painter->drawLine( option.rect.topRight(),  option.rect.bottomLeft());
			
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
	
	setItemDelegate(new KTExposureItemDelegate(this));
	
	QTableWidgetItem *prototype = new QTableWidgetItem();
	
	prototype->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable );
	
	prototype->setTextAlignment(Qt::AlignCenter);
	
	setItemPrototype(prototype);
	
	setRowCount( 100 );
	
	m_header = new KTExposureHeader(this);
	connect(m_header, SIGNAL(visiblityChanged ( int, bool )), this, SIGNAL(requestChangeVisiblityLayer( int, bool )));
	connect(m_header, SIGNAL(changedName ( int, const QString & )), this, SIGNAL(requestRenameLayer( int, const QString & )));
	
	
	connect(m_header, SIGNAL(sectionMoved ( int , int , int  )), this, SLOT(emitRequestMoveLayer( int, int , int )));
	
	
	setHorizontalHeader( m_header);
	
	connect(this, SIGNAL(cellClicked ( int, int )), this, SLOT(emitRequestSetUsedFrame(int, int)));
	connect(this, SIGNAL( currentCellChanged ( int , int , int , int  )), this, SLOT( emitRequestSelectFrame(  int , int , int , int  )));
	
	setSelectionBehavior (QAbstractItemView::SelectItems);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

void KTExposureTable::emitRequestRenameFrame( QTableWidgetItem * item )
{
	QModelIndex  index = indexFromItem ( item );
	emit requestRenameFrame(index.column(), index.row(), item->text());
}

void KTExposureTable::emitRequestSelectFrame(  int currentRow_, int currentColumn_, int previousRow, int previousColumn )
{
	if(previousRow != currentRow_ || previousColumn != currentColumn_)
	{
		emit  requestSelectFrame(currentLayer(), currentRow() );
	}
}

void KTExposureTable::emitRequestMoveLayer( int , int oldVisualIndex, int newVisualIndex  )
{
	if(! m_header->signalMovedBlocked())
	{
		m_header->moveLayer( newVisualIndex, oldVisualIndex);
		emit requestMoveLayer( oldVisualIndex,  newVisualIndex );
	}
}



KTExposureTable::~KTExposureTable()
{
}

void KTExposureTable::setFrameName(int indexLayer, int indexFrame,const QString & name)
{
	QTableWidgetItem *frame = item( indexFrame , indexLayer);
	if(frame)
	{
		if(frame->text() != name)
		{
			frame->setText(name);
		}
	}
}

void KTExposureTable::setLayerName(int indexLayer, const QString & name)
{
	m_header->setLayerName( m_header->logicalIndex(indexLayer), name);
}

bool KTExposureTable::frameIsLocked(int indexLayer, int indexFrame)
{
	QTableWidgetItem *frame = item(indexFrame, indexLayer);
	if(frame)
	{
		return frame->data(IsLocked).toBool();
	}
	else
	{
		dWarning() << "frame " << indexLayer << "," << indexFrame << " not exist";
	}
	return false;
}


void KTExposureTable::selectFrame( int indexLayer, int indexFrame)
{
	setCurrentCell(indexFrame,  m_header->logicalIndex(indexLayer));
}

int KTExposureTable::currentLayer() const
{
	D_FUNCINFO;
	return m_header->visualIndex(currentColumn());
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
	insertColumn( index );
	m_header->insertLayer( index, name );
}

void KTExposureTable::setUseFrame(int indexLayer, int indexFrame, const QString & name)
{
	QTableWidgetItem * frame = new QTableWidgetItem;
	
	frame->setBackgroundColor(Qt::gray );
	frame->setText(name);
	frame->setData(IsUsed, true);
	frame->setTextAlignment(Qt::AlignCenter);
	
	int logicalIndex = m_header->logicalIndex( indexLayer);
	
	m_header->setLastFrame( logicalIndex, m_header->lastFrame(logicalIndex)+1 );
	
	setItem(m_header->lastFrame(logicalIndex)-1, logicalIndex, frame);
	
	for(int i = m_header->lastFrame(logicalIndex)-1; i > indexFrame; i--)
	{
		moveFrame(  indexLayer, i , indexLayer, i-1);
	}
	
	if(m_header->lastFrame(logicalIndex)  == rowCount ())
	{
		setRowCount( m_header->lastFrame(logicalIndex) + 50 );
	}
}

void KTExposureTable::setLockFrame(int indexLayer, int indexFrame, bool locked)
{
	
	int logicalIndex = m_header->logicalIndex ( indexLayer);
	QTableWidgetItem * frame = item(indexFrame, logicalIndex);
	if(frame)
	{
		if(frame->data(IsUsed).toBool())
		{
			if(locked)
			{
				frame->setBackgroundColor(Qt::red);
			}
			else
			{
				frame->setBackgroundColor(Qt::gray);
			}
			frame->setData(IsLocked, locked);
		}
	}
}

void KTExposureTable::setLockLayer(int indexLayer,  bool locked)
{
	m_header->setLockLayer ( indexLayer, locked);
}


void KTExposureTable::setVisibilityChanged(int visualIndex, bool visibility)
{
	m_header->setVisibilityChanged(m_header->logicalIndex(visualIndex), visibility);
}

void KTExposureTable::removeLayer(int indexLayer )
{
	
	int logicalIndex = m_header->logicalIndex(indexLayer);
	m_header->removeLayer(logicalIndex);
	removeColumn ( logicalIndex );
}

void KTExposureTable::removeFrame(int indexLayer, int indexFrame)
{
	setUpdatesEnabled(false);
	QTableWidgetItem *i  = takeItem (  indexFrame, indexLayer );
	if(i)
	{
		for(int index = indexFrame+1; index < m_header->lastFrame(indexLayer); index++ )
		{
			QTableWidgetItem * idx  = takeItem (  index, indexLayer );
			if(idx)
			{
				setItem(index-1, indexLayer, idx);
			}
		}
		m_header->setLastFrame( indexLayer, m_header->lastFrame(indexLayer)-1 );
	}
	setUpdatesEnabled(true);
}

void KTExposureTable::moveFrame(  int oldPosLayer, int oldPosFrame, int newPosLayer, int newPosFrame)
{
	QTableWidgetItem * oldItem  = takeItem(oldPosFrame, oldPosLayer );
	QTableWidgetItem * newItem  = takeItem(newPosFrame, newPosLayer );
	
	setItem(newPosFrame, newPosLayer, oldItem);
	setItem(oldPosFrame, oldPosLayer, newItem);
	//FIXME remover esto
	setCurrentItem(oldItem);
	//
}

void KTExposureTable::moveLayer( int oldPosLayer, int newPosLayer )
{
	m_header->moveLayer( oldPosLayer, newPosLayer );
}


void KTExposureTable::emitRequestSetUsedFrame( int indexFrame,  int indexLayer)
{
	int visualIndex = m_header->visualIndex ( indexLayer);
	
	if(indexFrame == m_header->lastFrame(indexLayer))
	{
		emit requestSetUsedFrame(visualIndex, indexFrame);
		emit requestSelectFrame( visualIndex, indexFrame);
	}
}

int KTExposureTable::numUsed() const
{
	return m_header->lastFrame( currentColumn() );
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

void KTExposureTable::commitData ( QWidget *editor )
{
	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	
	QTableWidget::commitData(0); // Don't rename
	
	if ( lineEdit )
	{
		emit requestRenameFrame(currentLayer(),  currentFrame(), lineEdit->text());
	}
}

