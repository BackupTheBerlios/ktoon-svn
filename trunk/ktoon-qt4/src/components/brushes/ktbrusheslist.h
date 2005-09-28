//
// C++ Interface: ktbrusheslist
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTBRUSHESLIST_H
#define KTBRUSHESLIST_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

/**
*	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTBrushesList : public QTreeWidget
{
	Q_OBJECT
	public:
		KTBrushesList(QWidget *parent = 0);
		~KTBrushesList();
		void addBrush(int min, int max, int smooth, QString name);
		void removeCurrentBrush();
		int indexCurrentBrush();
		
	public slots:
		void changeCurrentValueMin(int min);
		void changeCurrentValueMax(int max);
		void changeCurrentValueSmooth(int smooth);
		void changeCurrentValueName(QString name);
		void changeCurrentBrush();
		
	signals:
		void changeCurrentBrush(int min, int max, int smooth, QString name);

};

#endif
