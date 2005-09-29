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
*	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTScenesList : public QTreeWidget
{
	Q_OBJECT
	public:
		KTScenesList(QWidget *parent = 0);
		~KTScenesList();
		void addScene(QString name);
		int removeCurrentScene();
		int indexCurrentScene();
		QString nameCurrentScene();
		
	private slots:
		void changeCurrentScene();
		
	public slots:
		int moveCurrentSceneUp();
		int moveCurrentSceneDown();
		void changeCurrentName(QString name);
		
	signals:
		void changeCurrent(QString name, int index);
};

#endif
