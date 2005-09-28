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
#include <QTreeWidgetItem>

KTScenesList::KTScenesList(QWidget *parent)
 : QTreeWidget(parent)
{
	setHeaderLabels ( QStringList() << "name" );
	hide();
}


KTScenesList::~KTScenesList()
{
	
}

void KTScenesList::addScenes( QString name)
{
	QTreeWidgetItem *newScenes = new QTreeWidgetItem( this);
	newScenes->setText(0, name);
}

