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
#include <QHeaderView>
#include <QStringList>
#include "ktdebug.h"

KTBrushesList::KTBrushesList(QWidget *parent)
 : QTreeWidget(parent)
{
	setHeaderLabels ( QStringList() << tr("Min") << tr( "Max" ) << tr( "Smo" ) << tr( "Name" ) );
	header()->setResizeMode(QHeaderView::Stretch);
	connect(this, SIGNAL( itemSelectionChanged ()), this, SLOT(changeCurrentBrush()));
}


KTBrushesList::~KTBrushesList()
{
}

void KTBrushesList::addBrush(int min, int max, int smooth, QString name)
{
	QTreeWidgetItem *newBrush = new QTreeWidgetItem(this);
	QStringList valuesBrush;
	valuesBrush << QString::number(min) << QString::number(max) << QString::number(smooth) << name;
	
	for(int i = 0; i < valuesBrush.count() ; i++)
	{
		newBrush->setText(i, valuesBrush[i]);
	}
	setCurrentItem ( newBrush );
}

void KTBrushesList::changeCurrentValueMin( int min)
{
	if(currentItem() )
	{
		currentItem()->setText(0,QString::number(min));
	}
}

void KTBrushesList::changeCurrentValueMax( int max)
{
	if(currentItem() )
	{
		currentItem()->setText(1,QString::number(max));
	}
}

void KTBrushesList::changeCurrentValueSmooth( int smooth)
{
	if(currentItem() )
	{
		currentItem()->setText(2,QString::number(smooth));
	}
}

void KTBrushesList::changeCurrentValueName( QString name)
{
	if(currentItem() )
	{
		currentItem()->setText(3, name);
	}
}

int KTBrushesList::removeCurrentBrush()
{
// 	QPoint p = visualItemRect ( currentItem()).topRight();
// 	delete currentItem();
// 	QTreeWidgetItem *item = itemAt( p -= QPoint(2,2) );
// 	setCurrentItem( item );
	
	int index = indexCurrentBrush();
	delete currentItem();
	if(index < 1)
	{
		setCurrentItem( topLevelItem ( 0 ) );
	}
	else
	{
		setCurrentItem( topLevelItem ( index -1 ) );
	}
	return index;
	
}

void KTBrushesList::changeCurrentBrush()
{
	int min = QString(currentItem()->text(0)).toInt();
	int max = QString(currentItem()->text(1)).toInt();
	int smooth = QString(currentItem()->text(2)).toInt();
	QString name = currentItem()->text(3);
	emit( changeCurrentBrush( min, max,  smooth, name));
}

int KTBrushesList::indexCurrentBrush()
{
	return indexOfTopLevelItem(currentItem());
}
