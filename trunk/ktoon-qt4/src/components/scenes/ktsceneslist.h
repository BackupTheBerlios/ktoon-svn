//
// C++ Interface: ktsceneslist
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTSCENESLIST_H
#define KTSCENESLIST_H

#include <QTreeWidget>

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTScenesList : public QTreeWidget
{
	Q_OBJECT
	public:
		KTScenesList(QWidget *parent = 0);
		~KTScenesList();
		void addScenes(QString name);
};

#endif
