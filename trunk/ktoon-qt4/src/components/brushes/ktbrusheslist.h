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

// #include <QTreeWidget>
// #include <QTreeWidgetItem>

#include <QListWidget>
#include <QListWidgetItem>
#include <QPainterPath>

#include "ktimagestable.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTBrushesList : public KTCellView
{
	Q_OBJECT
	public:
		KTBrushesList(QWidget *parent = 0);
		~KTBrushesList();
		void addBrush(int thickness, int smooth, const QPainterPath &form, QString name);
		int removeCurrentBrush();
		int indexCurrentBrush();
		QPainterPath path(int);
		
	public slots:
		void changeCurrentValueMin(int min);
		void changeCurrentValueMax(int max);
		void changeCurrentValueSmooth(int smooth);
		void changeCurrentValueName(QString name);
		void changeCurrentBrush();
		
	signals:
		void changeCurrentBrush( QListWidgetItem*);
		
	private:
		QList<QPainterPath> m_forms;
		const int MAX_COLUMS;

};

#endif
