//
// C++ Implementation: ktbrusheslist
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ktbrusheslist.h"
#include <QPainter>
#include <QStringList>
#include "ktdebug.h"

KTBrushesList::KTBrushesList(QWidget *parent)
	: KTCellView(parent), MAX_COLUMS(10)
{
}


KTBrushesList::~KTBrushesList()
{
}

void KTBrushesList::addBrush(int thickness, int smooth, const QPainterPath &form, QString name)
{
	KT_FUNCINFO;
	
	const int columns = columnCount();
	const int rows = rowCount();
	
	KTCellViewItem *newBrush = new KTCellViewItem();

	QImage tbrush(form.boundingRect().width()+2, form.boundingRect().height()+2, QImage::Format_RGB32);
	
	tbrush.fill(qRgba(255,255,255,0));
	QPainter p(&tbrush);
	p.setRenderHint(QPainter::Antialiasing);
	
	p.setPen(QPen(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	
// 	p.setBrush( QBrush( foregroundColor (), Qt::SolidPattern));
	
	
	p.drawPath(form);
	
	newBrush->setImage( tbrush );
// 	newBrush->setBackground( QColor(34,34,234,60 ) );
	
	m_forms << form;
	
	int row = 0, col = 0;
	
	if ( columns < m_forms.count() % MAX_COLUMS)
	{
		insertRow( rows + 1);
		row = rows;
	}
	
	if ( columns == MAX_COLUMS || rows == 0 )
	{
		insertColumn( m_forms.count() % MAX_COLUMS );
		col = ( m_forms.count() % MAX_COLUMS ) -1;
	}

	setItem( row, col, newBrush);
}

QPainterPath KTBrushesList::path(int index)
{
	return m_forms[index];
}

void KTBrushesList::changeCurrentValueMin( int min)
{
// 	if(currentItem() )
// 	{
// 		currentItem()->setText(0,QString::number(min));
// 	}
}

void KTBrushesList::changeCurrentValueMax( int max)
{
// 	if(currentItem() )
// 	{
// 		currentItem()->setText(1,QString::number(max));
// 	}
}

void KTBrushesList::changeCurrentValueSmooth( int smooth)
{
// 	if(currentItem() )
// 	{
// 		currentItem()->setText(2,QString::number(smooth));
// 	}
}

void KTBrushesList::changeCurrentValueName( QString name)
{
// 	if(currentItem() )
// 	{
// 		currentItem()->setText(3, name);
// 	}
}

int KTBrushesList::removeCurrentBrush()
{
// 	QPoint p = visualItemRect ( currentItem()).topRight();
// 	delete currentItem();
// 	QTreeWidgetItem *item = itemAt( p -= QPoint(2,2) );
// 	setCurrentItem( item );
	
// 	int index = indexCurrentBrush();
// 	delete currentItem();
// 	if(index < 1)
// 	{
// 		setCurrentItem( topLevelItem ( 0 ) );
// 	}
// 	else
// 	{
// 		setCurrentItem( topLevelItem ( index -1 ) );
// 	}
// 	return index;
	
}

void KTBrushesList::changeCurrentBrush()
{
// 	int min = QString(currentItem()->text(0)).toInt();
// 	int max = QString(currentItem()->text(1)).toInt();
// 	int smooth = QString(currentItem()->text(2)).toInt();
// 	QString name = currentItem()->text(3);
// 	emit( changeCurrentBrush(currentItem()));
}

int KTBrushesList::indexCurrentBrush()
{
	return 0;
// 	return indexOfTopLevelItem(currentItem());
}
