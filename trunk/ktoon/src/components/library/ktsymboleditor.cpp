/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ktsymboleditor.h"

#include <QGraphicsView>
#include <QGraphicsScene>

#include "ktlibraryobject.h"

struct KTSymbolEditor::Private
{
	QGraphicsView *view;
	QGraphicsScene *scene;
	
	KTLibraryObject *symbol;
};

KTSymbolEditor::KTSymbolEditor(QWidget *parent)
	: QMainWindow(parent), d(new Private)
{
	setWindowTitle(tr("Symbol editor"));
	
	d->view = new QGraphicsView;
	d->view->setRenderHints(QPainter::Antialiasing);
	
	d->scene = new QGraphicsScene;
	d->view->setScene(d->scene);
	
	setCentralWidget(d->view);
}


KTSymbolEditor::~KTSymbolEditor()
{
	delete d;
}


void KTSymbolEditor::setSymbol(KTLibraryObject *object)
{
	if(  QGraphicsItem *item = qvariant_cast<QGraphicsItem *>(object->data()) )
	{
		d->symbol = object;
		d->scene->addItem(item);
		
	}
}


