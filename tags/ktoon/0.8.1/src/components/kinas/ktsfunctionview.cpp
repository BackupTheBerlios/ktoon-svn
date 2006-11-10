//
// C++ Implementation: ktsfunctionview
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ktsfunctionview.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QItemDelegate>

KTSFunctionView::KTSFunctionView(QWidget *parent) : QTreeWidget(parent)
{
	setItemDelegate(new QItemDelegate(this));
	setColumnCount(1);
	setHeaderLabels ( QStringList() << "Members"/*"Module" << "Section" << "Function"*/);
	
	setRootIsDecorated (true);
	header()->setResizeMode(QHeaderView::Stretch);
	
// 	setEditTriggers(QAbstractItemView::AnyKeyPressed);
	
	addModule("A Module", "A section", QStringList() << "Function1" << "Function2" << "Function3" << "Function4" );
	
	setDragEnabled ( true);
}

KTSFunctionView::~KTSFunctionView()
{
}

void KTSFunctionView::addModule(const QString &module, const QString &section, QStringList functions)
{
	// TODO: mirar si los modulos y secciones existen, sino crearlas
	
	
	QTreeWidgetItem *itemModule = new QTreeWidgetItem(this);
	itemModule->setText(0, module);
	
	QTreeWidgetItem *itemSection = new QTreeWidgetItem(itemModule);
	itemSection->setText(0, section);
	
	QStringListIterator iterator(functions);
	
	while (iterator.hasNext())
	{
		QTreeWidgetItem *itemFunction = new QTreeWidgetItem(itemSection);
		itemFunction->setText(0, iterator.next());
	}
}

bool KTSFunctionView::dropMimeData ( QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action )
{
	return true;
}
