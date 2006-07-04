//
// C++ Implementation: ktsceneslist
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ktsceneslist.h"
#include "ddebug.h"

#include <QTreeWidgetItem>

KTSceneManagersList::KTSceneManagersList(QWidget *parent)
	: DTreeListWidget(parent)
{
	setHeaderLabels ( QStringList() << "name" );
// 	hide();
	connect(this, SIGNAL( itemSelectionChanged ()), this, SLOT(changeCurrentScene()));
	connect(this, SIGNAL(itemClicked ( QTreeWidgetItem *, int)), this, SLOT(changeCurrentScene(QTreeWidgetItem *, int)));
// 	setDragEnabled ( true);
}


KTSceneManagersList::~KTSceneManagersList()
{
	
}

void KTSceneManagersList::addScene( QString name)
{
	QTreeWidgetItem *newScene = new QTreeWidgetItem( this);
// 	newScene->setFlags(Qt::ItemIsEditable/*Qt::ItemIsEnabled*//*Qt::ItemIsUserCheckable*/ );//Qt::ItemIsDragEnabled);

	newScene->setText(0, name);
	setCurrentItem(newScene);
// 	emit changeCurrent(name, indexCurrentScene());
}

void KTSceneManagersList::changeCurrentName(QString name)
{
	currentItem()->setText(0, name);
} 

int KTSceneManagersList::removeCurrentScene()
{
	int index = indexCurrentScene();
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


void KTSceneManagersList::changeCurrentScene()
{
	QString name = currentItem()->text(0);
	int index = indexCurrentScene();
	emit(changeCurrent( name, index));
}

void KTSceneManagersList::changeCurrentScene(QTreeWidgetItem *item, int )
{
	QString name = item->text(0);
	int index = indexOfTopLevelItem(item);
	emit(changeCurrent( name, index));
}

int KTSceneManagersList::moveCurrentSceneUp()
{
	int index = indexCurrentScene();
	if(index > 0)
	{
		insertTopLevelItem(  index-1, takeTopLevelItem( index ) );
		setCurrentItem( topLevelItem ( index-1 ) );
	}
	return index;
}

int KTSceneManagersList::moveCurrentSceneDown()
{
	int index = indexCurrentScene();
	if(index < topLevelItemCount()-1)
	{
		insertTopLevelItem(  index+1, takeTopLevelItem( index ) );
		setCurrentItem( topLevelItem ( index+1 ) );
	}
	return index;
}


int KTSceneManagersList::indexCurrentScene()
{
	return indexOfTopLevelItem(currentItem());
}

QString KTSceneManagersList::nameCurrentScene()
{
	return currentItem()->text(0);
}

int KTSceneManagersList::scenesCount()
{
	return topLevelItemCount();
}

