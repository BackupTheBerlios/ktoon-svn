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

#include "ktpaintareastatus.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QObject>

#include <kgui/kseparator.h>
#include <kcore/kdebug.h>

#include "ktviewdocument.h"
#include "ktglobal.h"
#include "ktbrushmanager.h"
#include "ktcolorwidget.h"
#include "ktbrushstatus.h"

////////////////

struct KTPaintAreaStatus::Private
{
    KTViewDocument *viewDocument;
    //QPushButton *antialiasHint;
    QCheckBox *antialiasHint;
    //QComboBox *renderer;
    KTBrushStatus *brushStatus;
    QComboBox *rotation;
};

KTPaintAreaStatus::KTPaintAreaStatus(KTViewDocument *parent) : QStatusBar(parent), k( new Private)
{
    setSizeGripEnabled(false);
    k->viewDocument = parent;

    QWidget *rotContainer = new QWidget;
    QHBoxLayout *rotLayout = new QHBoxLayout(rotContainer);
    rotLayout->setSpacing(3);
    rotLayout->setMargin(1);

    rotLayout->addWidget(new QLabel(tr("Rotate")));

    k->rotation = new QComboBox();
    k->rotation->setDuplicatesEnabled(false);
    k->rotation->setEditable(true);

    for (int i = 0; i < 360; i+=30)
         k->rotation->addItem(QString::number(i), i);

    k->rotation->setValidator(new QIntValidator(-360, 360,this));

    rotLayout->addWidget(k->rotation);

    addPermanentWidget(rotContainer);

    connect(k->rotation, SIGNAL(activated(const QString &)), this, SLOT(applyRotationFromItem(const QString &)));

    ///////

    k->antialiasHint = new QCheckBox(tr("Antialiasing"));
    k->antialiasHint->setFocusPolicy(Qt::NoFocus);
    k->antialiasHint->setCheckable(true);
    k->antialiasHint->setChecked(true);
    addPermanentWidget(k->antialiasHint);

    connect(k->antialiasHint, SIGNAL(clicked()), this, SLOT(selectAntialiasingHint()));

    k->brushStatus = new KTBrushStatus;
    addPermanentWidget(k->brushStatus);

    //connect(k->antialiasHint, SIGNAL(toggled(bool)), this, SLOT(selectAntialiasingHint(bool)));
    //connect(k->antialiasHint, SIGNAL(clicked()), this, SLOT(selectAntialiasingHint(bool)));

    k->brushStatus->setBackground(k->viewDocument->brushManager()->brush());
    k->brushStatus->setForeground(k->viewDocument->brushManager()->pen());
}

KTPaintAreaStatus::~KTPaintAreaStatus()
{
    delete k;
}

void KTPaintAreaStatus::selectAntialiasingHint()
{
    k->viewDocument->setAntialiasing(k->antialiasHint->isChecked()); 
}

void KTPaintAreaStatus::selectRenderer(int id)
{
  /*
    KToon::RenderType type = KToon::RenderType(k->renderer->itemData(id ).toInt());

    if (type == KToon::OpenGL)
        k->viewDocument->setOpenGL(true);
    else
        k->viewDocument->setOpenGL(false);
   */
}

void KTPaintAreaStatus::setBrush(const QBrush &brush)
{
    kFatal() << "KTPaintAreaStatus::setBrush - Just tracing!";
    k->brushStatus->setBackground(brush);
}

void KTPaintAreaStatus::setPen(const QPen &pen)
{
    k->brushStatus->setForeground(pen);
}

void KTPaintAreaStatus::applyRotationFromItem(const QString & text)
{
    int angle = text.toInt();

    if (angle < 0)
        angle += 360;

    k->viewDocument->setRotationAngle(angle);
}

