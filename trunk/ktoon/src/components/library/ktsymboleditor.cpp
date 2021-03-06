/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktsymboleditor.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QTimer>

#include "ktlibraryobject.h"
#include "ktpluginmanager.h"
#include "kttoolplugin.h"

// KOM 
#include <kgui/kaction.h>
#include <kcore/kdebug.h>

/**
 * This class defines the toolbar at the left side of the paint area.
 * Here is where tool plugins are loaded and where the toolbar is created.
 * @author David Cuadrado <krawek@gmail.com>
*/

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

KTSymbolEditor::KTSymbolEditor(QWidget *parent) : QMainWindow(parent), k(new Private)
{
    setWindowTitle(tr("Symbol editor"));
    
    k->view = new View;
    k->view->setRenderHints(QPainter::Antialiasing);
    
    k->scene = new QGraphicsScene;
    k->view->setScene(k->scene);
    
    setCentralWidget(k->view);
    
    k->brushTools = new QToolBar(tr("Brushes"));
    addToolBar(Qt::BottomToolBarArea, k->brushTools);
    
    k->selectionTools = new QToolBar(tr("Selection"));
    addToolBar(Qt::BottomToolBarArea, k->selectionTools);
    
    k->fillTools = new QToolBar(tr("Fill"));
    addToolBar(Qt::BottomToolBarArea, k->fillTools);
    
    k->viewTools = new QToolBar(tr("View"));
    addToolBar(Qt::BottomToolBarArea, k->viewTools);
    
    QTimer::singleShot(0, this, SLOT(loadTools()));
}


KTSymbolEditor::~KTSymbolEditor()
{
    delete k;
}


void KTSymbolEditor::setSymbol(KTLibraryObject *object)
{
    if(  QGraphicsItem *item = qvariant_cast<QGraphicsItem *>(object->data()) )
    {
        k->symbol = object;
        k->scene->addItem(item);
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
            kDebug("plugins") << "*******Tool Loaded: " << *it;
            
            KAction *act = tool->actions()[*it];
            if ( act )
            {
                connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
                
                switch( tool->toolType() )
                {
                    case KTToolInterface::Selection:
                    {
                        k->selectionTools->addAction(act);
                    }
                    break;
                    case KTToolInterface::Fill:
                    {
                        k->fillTools->addAction(act);
                    }
                    break;
                    case KTToolInterface::View:
                    {
                        k->viewTools->addAction(act);
                    }
                    break;
                    case KTToolInterface::Brush:
                    {
                        k->brushTools->addAction(act);
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
    K_FUNCINFO;
    KAction *action = qobject_cast<KAction *>(sender());
    
    if ( action )
    {
        KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(action->parent());
        tool->setCurrentTool( action->text() );
    }
}
