//
// C++ Interface: ktsfunctionview
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTSFUNCTIONVIEW_H
#define KTSFUNCTIONVIEW_H

#include <QTreeWidget>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTSFunctionView : public QTreeWidget
{
	Q_OBJECT
	public:
		KTSFunctionView(QWidget *parent = 0);
		~KTSFunctionView();
		void addModule(const QString &module, const QString &sections, QStringList functions);
		
	protected:
		bool dropMimeData ( QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action );

};

#endif
