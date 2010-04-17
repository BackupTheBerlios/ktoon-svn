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
 *   License:                                                              *
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

#include "ktprojectactionbar.h"

#include <QVBoxLayout>
#include <QVariant>
#include <QSpacerItem>

#include <ktglobal.h>
#include <kcore/kdebug.h>

#include <kgui/kseparator.h>
#include <kcore/kconfig.h>
#include <kgui/koptionaldialog.h>
#include <kgui/kimagebutton.h>

struct KTProjectActionBar::Private
{
    Private(Qt::Orientation orientation) : orientation(orientation), isAnimated(true) {}
    Qt::Orientation orientation;
    int fixedSize;
    QButtonGroup actions;
    QBoxLayout *buttonLayout;
    bool isAnimated;
};

KTProjectActionBar::KTProjectActionBar(Actions actions, Qt::Orientation orientation, QWidget *parent) : QWidget(parent ), k(new Private(orientation))
{
    connect(&k->actions, SIGNAL(buttonClicked(int)), this, SLOT(emitActionSelected(int)));
    
    setup(actions);
    
    setFixedSize(22);
}

KTProjectActionBar::~KTProjectActionBar()
{
}

void KTProjectActionBar::setFixedSize(int s)
{
    k->fixedSize = s;
}

void KTProjectActionBar::setup(Actions actions)
{
    QBoxLayout *mainLayout = 0;
    
    switch (k->orientation) {
            case Qt::Vertical:
            {
                 mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
                 k->buttonLayout = new QBoxLayout(QBoxLayout::TopToBottom);
            }
            break;
            case Qt::Horizontal:
            {
                 mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
                 k->buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
            }
            break;
    }
    
    mainLayout->setSpacing(0);
    mainLayout->setMargin(1);
    
    k->buttonLayout->setSpacing(1);
    k->buttonLayout->setMargin(1);
    
    k->buttonLayout->addStretch();
    
    int size = 16;
    
    if (actions & InsertFrame) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/add_frame.png"), size);
        button->setToolTip(tr("Insert frame"));
        button->setShortcut(QKeySequence(Qt::Key_Plus));
        
        k->actions.addButton(button, InsertFrame);
        
        k->buttonLayout->addWidget(button);
        
        button->setAnimated(k->isAnimated);
    }
    
    if (actions & RemoveFrame) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/remove_frame.png"), size);
        button->setToolTip(tr("Remove the frame"));
        
        k->actions.addButton(button, RemoveFrame);
        
        button->setShortcut(QKeySequence(Qt::Key_Minus));
        
        k->buttonLayout->addWidget(button);
        
        button->setAnimated(k->isAnimated);
    }
     
    if (actions & MoveFrameUp) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_frame_up.png"), size);
        button->setToolTip(tr("Move frame up"));
        
        k->actions.addButton(button, MoveFrameUp);
        
        k->buttonLayout->addWidget(button);
        
        button->setAnimated(k->isAnimated);
    }
    
    if (actions & MoveFrameDown) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_frame_down.png"), size);
        button->setToolTip(tr("Move frame down"));
        
        k->actions.addButton(button, MoveFrameDown);
        
        k->buttonLayout->addWidget(button);
        
        button->setAnimated(k->isAnimated);
    }
    
    if (actions & LockFrame) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/padlock.png"), size);
        button->setToolTip(tr("Lock frame"));
        
        k->actions.addButton(button, LockFrame);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }

    if (actions & Separator) {
        k->buttonLayout->addWidget(new KSeparator(Qt::Vertical));
    }
    
    if (actions & InsertLayer) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/add_layer.png"), size);
        button->setToolTip(tr("Insert a layer"));
        //button->setEnabled(false);
        
        k->actions.addButton(button, InsertLayer);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
     
    if (actions & RemoveLayer) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/remove_layer.png"), size);
        button->setToolTip(tr("Remove the layer"));
        //button->setEnabled(false);
        
        k->actions.addButton(button, RemoveLayer);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
     
    if (actions & MoveLayerUp) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_layer_up.png"), size);
        
        button->setToolTip(tr("Move layer up"));
        
        k->actions.addButton(button, MoveLayerUp);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(true);
    }
     
    if (actions & MoveLayerDown) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_layer_down.png" ), size);
        
        button->setToolTip(tr("Move layer down"));
        
        k->actions.addButton(button, MoveLayerDown);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
    
    if (actions & LockLayer) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_layer_down.png" ), 22);
        button->setToolTip(tr("Lock layer"));
        
        k->actions.addButton(button, LockLayer);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }

    if (actions & InsertScene) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/add_scene.png"), size);  // TODO
        
        button->setToolTip(tr("Insert a scene"));
        
        k->actions.addButton(button, InsertScene);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
     
    if (actions & RemoveScene) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/remove_scene.png"), size);  // TODO
        
        button->setToolTip(tr("Remove the scene"));
        k->actions.addButton(button, RemoveScene);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }

    if (actions & MoveSceneUp) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_scene_up.png"), size);
        
        button->setToolTip(tr("Move scene up"));
        
        k->actions.addButton(button, MoveSceneUp);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
     
    if (actions & MoveSceneDown) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/move_scene_down.png"), size);
        
        button->setToolTip( tr("Move scene down"));
        
        k->actions.addButton(button, MoveSceneDown);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }
    
    if (actions & LockScene) {
        KImageButton *button = new KImageButton(QIcon(THEME_DIR + "icons/lock_scene.png"), size);
        button->setToolTip(tr("Lock scene"));
        
        k->actions.addButton(button, LockScene);
        
        k->buttonLayout->addWidget(button);
        button->setAnimated(k->isAnimated);
    }

    k->buttonLayout->addStretch();
    
    mainLayout->addWidget(new KSeparator(Qt::Horizontal));
    mainLayout->addLayout(k->buttonLayout);
    mainLayout->addWidget(new KSeparator(Qt::Horizontal));
}

// TODO: Check why this function do nothing :S

void KTProjectActionBar::insertSeparator(int position)
{
    Qt::Orientation sepOrientation = Qt::Vertical;
    
    switch (k->orientation) {
            case Qt::Vertical:
            {
                 sepOrientation = Qt::Horizontal;
            }
            break;
            case Qt::Horizontal:
            {
                 sepOrientation = Qt::Vertical;
            }
            break;
    }

    k->buttonLayout->insertWidget(position + 1, new KSeparator(Qt::Vertical), 1, Qt::AlignCenter);
}

void KTProjectActionBar::insertBlankSpace(int position)
{
    Qt::Orientation sepOrientation = Qt::Vertical;
   
    switch (k->orientation) {
            case Qt::Vertical:
            {
                 sepOrientation = Qt::Horizontal;
            }
            break;
            case Qt::Horizontal:
            {
                 sepOrientation = Qt::Vertical;
            }
            break;
    }

    QWidget *widget = new QWidget();
    widget->setFixedSize(5,5);
   
    k->buttonLayout->insertWidget(position + 1, widget, 1, Qt::AlignCenter);
}

KImageButton *KTProjectActionBar::button(Action action)
{
    return qobject_cast<KImageButton *>(k->actions.button(action));
}

void KTProjectActionBar::emitActionSelected(int action)
{
    switch(action)
    {
        case RemoveFrame:
        {
            bool noAsk = qvariant_cast<bool>(KCONFIG->value("RemoveWithoutAskFrame", false));

            if (! noAsk) {
                KOptionalDialog dialog(tr("Do you want to remove this frame?"), tr("Remove?"), this);

                if (dialog.exec() == QDialog::Rejected)
                    return;

                KCONFIG->setValue("RemoveWithoutAskFrame", dialog.shownAgain());
                KCONFIG->sync();
            }
        }
        break;
        case RemoveLayer:
        {
            bool noAsk = qvariant_cast<bool>(KCONFIG->value("RemoveWithoutAskLayer", false));
            if (! noAsk) {
                KOptionalDialog dialog(tr("Do you want to remove this layer?"), tr("Remove?"), this);

                if (dialog.exec() == QDialog::Rejected)
                    return;

                KCONFIG->setValue("RemoveWithoutAskLayer", dialog.shownAgain());
                KCONFIG->sync();
            }
        }
        break;
        case RemoveScene:
        {
            bool noAsk = qvariant_cast<bool>(KCONFIG->value("RemoveWithoutAskScene", false));
            if (! noAsk) {
                KOptionalDialog dialog(tr("Do you want to remove this scene?"), tr("Remove?"), this);

                if (dialog.exec() == QDialog::Rejected)
                    return;

                KCONFIG->setValue("RemoveWithoutAskScene", dialog.shownAgain());
                KCONFIG->sync();
            }
        }
        break;
    }
    
    emit actionSelected(action);
}

