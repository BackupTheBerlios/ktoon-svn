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
#include "ktdebug.h"

#include <QTreeWidgetItem>

KTScenesList::KTScenesList(QWidget *parent)
 : QTreeWidget(parent)
{
	setHeaderLabels ( QStringList() << "name" );
// 	hide();
	connect(this, SIGNAL( itemSelectionChanged ()), this, SLOT(changeCurrentScene()));
	
// 	setDragEnabled ( true);
}


KTScenesList::~KTScenesList()
{
	
}

void KTScenesList::addScene( QString name)
{
	QTreeWidgetItem *newScene = new QTreeWidgetItem( this);
// 	newScene->setFlags(Qt::ItemIsEditable/*Qt::ItemIsEnabled*//*Qt::ItemIsUserCheckable*/ );//Qt::ItemIsDragEnabled);

	newScene->setText(0, name);
	setCurrentItem(newScene);
// 	emit changeCurrent(name, indexCurrentScene());
}

void KTScenesList::changeCurrentName(QString name)
{
	currentItem()->setText(0, name);
} 

int KTScenesList::removeCurrentScene()
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


void KTScenesList::changeCurrentScene()
{
	QString name = currentItem()->text(0);
	int index = indexCurrentScene();
	emit(changeCurrent( name, index));
}

int KTScenesList::moveCurrentSceneUp()
{
	int index = indexCurrentScene();
	if(index > 0)
	{
		insertTopLevelItem(  index-1, takeTopLevelItem( index ) );
		setCurrentItem( topLevelItem ( index-1 ) );
	}
	return index;
}

int KTScenesList::moveCurrentSceneDown()
{
	int index = indexCurrentScene();
	if(index < topLevelItemCount()-1)
	{
		insertTopLevelItem(  index+1, takeTopLevelItem( index ) );
		setCurrentItem( topLevelItem ( index+1 ) );
	}
	return index;
}


int KTScenesList::indexCurrentScene()
{
	return indexOfTopLevelItem(currentItem());
}

QString KTScenesList::nameCurrentScene()
{
	return currentItem()->text(0);
}

int KTScenesList::scenesCount()
{
	return topLevelItemCount();
}

