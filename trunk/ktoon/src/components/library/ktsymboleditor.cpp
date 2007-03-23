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
#include <QToolBar>
#include <QTimer>

#include "ktlibraryobject.h"
#include "ktpluginmanager.h"
#include "kttoolplugin.h"

#include <dgui/daction.h>

#include <dcore/ddebug.h>


class View : public QGraphicsView
{
	public:
		View();
		~View();
};

View::View()
{
}

View::~View()
{
}


struct KTSymbolEditor::Private
{
	View *view;
	QGraphicsScene *scene;
	
	KTLibraryObject *symbol;
	
	QToolBar *selectionTools;
	QToolBar *fillTools;
	QToolBar *viewTools;
	QToolBar *brushTools;
};

KTSymbolEditor::KTSymbolEditor(QWidget *parent)
	: QMainWindow(parent), d(new Private)
{
	setWindowTitle(tr("Symbol editor"));
	
	d->view = new View;
	d->view->setRenderHints(QPainter::Antialiasing);
	
	d->scene = new QGraphicsScene;
	d->view->setScene(d->scene);
	
	setCentralWidget(d->view);
	
	d->brushTools = new QToolBar(tr("Brushes"));
	addToolBar(Qt::BottomToolBarArea, d->brushTools);
	
	d->selectionTools = new QToolBar(tr("Selection"));
	addToolBar(Qt::BottomToolBarArea, d->selectionTools);
	
	d->fillTools = new QToolBar(tr("Fill"));
	addToolBar(Qt::BottomToolBarArea, d->fillTools);
	
	d->viewTools = new QToolBar(tr("View"));
	addToolBar(Qt::BottomToolBarArea, d->viewTools);
	
	QTimer::singleShot(0, this, SLOT(loadTools()));
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

void KTSymbolEditor::loadTools()
{
	QActionGroup *group = new QActionGroup(this);
	group->setExclusive(true);
	
	foreach(QObject *plugin, KTPluginManager::instance()->tools() )
	{
		KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(plugin);
		
		QStringList::iterator it;
		QStringList keys = tool->keys();
			
		for (it = keys.begin(); it != keys.end(); ++it)
		{
			dDebug("plugins") << "*******Tool Loaded: " << *it;
			
			DAction *act = tool->actions()[*it];
			if ( act )
			{
				connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
				
				switch( tool->toolType() )
				{
					case KTToolInterface::Selection:
					{
						d->selectionTools->addAction(act);
					}
					break;
					case KTToolInterface::Fill:
					{
						d->fillTools->addAction(act);
					}
					break;
					case KTToolInterface::View:
					{
						d->viewTools->addAction(act);
					}
					break;
					case KTToolInterface::Brush:
					{
						d->brushTools->addAction(act);
					}
					break;
				}
				
				group->addAction(act);
				act->setCheckable(true);
				act->setParent(plugin);
			}
		}
	}
}

void KTSymbolEditor::selectTool()
{
	D_FUNCINFO;
	DAction *action = qobject_cast<DAction *>(sender());
	
	if ( action )
	{
		KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(action->parent());
		tool->setCurrentTool( action->text() );
		
		
	}
}

