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
	: QListWidget(parent)
{
// 	setHeaderLabels ( QStringList() << tr("Min") << tr( "Max" ) << tr( "Smo" ) << tr( "Name" ) );
	QListView::setViewMode(QListView::IconMode);
	setDragEnabled ( false );
// 	header()->setResizeMode(QHeaderView::Stretch);
	connect(this, SIGNAL( itemSelectionChanged ()), this, SLOT(changeCurrentBrush()));
}


KTBrushesList::~KTBrushesList()
{
}

void KTBrushesList::addBrush(int thickness, int smooth, const QPainterPath &form, QString name)
{
	QListWidgetItem *newBrush = new QListWidgetItem(this);
	
	m_forms << form;
// 	newBrush->setToolTip ( tr("Thickness: %1\nSmooth: %1").arg(thickness).arg(smooth) );
	
	QPixmap px(form.boundingRect().width(), form.boundingRect().height());
	
	px.fill(QColor(0,0,0,0));
	QPainter p;
	p.begin(&px);
// 	p.setPen(Qt::NoPen);
	p.setBrush( QBrush( foregroundColor (), Qt::SolidPattern));
	p.setRenderHint(QPainter::Antialiasing);
	p.drawPath(form);
	newBrush->setIcon ( QIcon(px) );
	newBrush->setText(name);
	
	setCurrentItem ( newBrush );
	
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
	emit( changeCurrentBrush(currentItem()));
}

int KTBrushesList::indexCurrentBrush()
{
	return 0;
// 	return indexOfTopLevelItem(currentItem());
}
