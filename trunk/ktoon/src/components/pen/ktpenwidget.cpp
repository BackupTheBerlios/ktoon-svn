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

#include "ktpenwidget.h"
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

#include <QListWidget>

#include "ktpaintareaevent.h"

struct KTPenWidget::Private
{
    KEditSpinBox *thickness;
    QComboBox *capStyle;
    QComboBox *joinStyle;
    QComboBox *style;
    QListWidget *texturesList;
    QPen pen;
};

KTPenWidget::KTPenWidget(QWidget *parent) : KTModuleWidgetBase(parent), k(new Private)
{
    setWindowTitle(tr("Brush Properties"));

    KCONFIG->beginGroup("PenParameters");
    int thicknessValue = KCONFIG->value("Thickness", -1).toInt();

    // kFatal() << "KTPenWidget::KTPenWidget: " << thicknessValue;

    if (thicknessValue <= 0)
        thicknessValue = 3;

    k->thickness = new KEditSpinBox(thicknessValue, 1, 100, 1, tr("Thickness"));
    k->thickness->setValue(thicknessValue);
    //setThickness(thicknessValue);
 
    connect(k->thickness, SIGNAL(valueChanged(int)), this, SLOT(setThickness(int)));
    
    addChild(k->thickness);
    QWidget *space = new QWidget(this);
    space->setFixedHeight(5);
    addChild(space);

    QLabel *label = new QLabel(tr("Dashes") + ":", this);
    addChild(label);
    
    k->style = new QComboBox();
    
    k->style->addItem(tr("No pen"), Qt::NoPen);
    k->style->addItem(tr("Solid"), Qt::SolidLine);
    k->style->addItem(tr("Dash"), Qt::DashLine);
    k->style->addItem(tr("Dot"), Qt::DotLine);
    k->style->addItem(tr("Dash dot"), Qt::DashDotLine);
    k->style->addItem(tr("Dash dot dot"), Qt::DashDotDotLine);
    
    addChild(k->style);
    connect(k->style, SIGNAL(currentIndexChanged(int)), this, SLOT(setStyle(int)));

    space = new QWidget(this);
    space->setFixedHeight(5);
    addChild(space);

    label = new QLabel(tr("Cap") + ":", this);
    addChild(label);
    
    k->capStyle = new QComboBox();
    
    k->capStyle->addItem(tr("Flat"), Qt::FlatCap);
    k->capStyle->addItem(tr("Square"), Qt::SquareCap);
    k->capStyle->addItem(tr("Round"), Qt::RoundCap);
    
    addChild(k->capStyle);
    connect(k->capStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(setCapStyle(int)));

    space = new QWidget(this);
    space->setFixedHeight(5);
    addChild(space);

    label = new QLabel(tr("Join") + ":", this);
    addChild(label);
    
    k->joinStyle = new QComboBox();
    
    k->joinStyle->addItem(tr("Miter"), Qt::MiterJoin);
    k->joinStyle->addItem(tr("Bevel"), Qt::BevelJoin);
    k->joinStyle->addItem(tr("Round"), Qt::RoundJoin);
    
    addChild(k->joinStyle);
    connect(k->joinStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(setJoinStyle(int)));

    space = new QWidget(this);
    space->setFixedHeight(5);
    addChild(space);

    label = new QLabel(tr("Texture") + ":", this);
    addChild(label);

    k->texturesList = new QListWidget(this);
    k->texturesList->setViewMode(QListView::IconMode);
    k->texturesList->setWrapping(true);
    k->texturesList->setFlow(QListView::LeftToRight);
    k->texturesList->setMovement(QListView::Static);
    k->texturesList->setSpacing(5);

    QListWidgetItem *texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush_01.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Solid");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense1Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense2Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense3Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense4Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense5Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense6Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("Dense7Pattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("HotPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("VerPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("CrossPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("BDiagPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("FDiagPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("DiagCrossPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("LinearGradientPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("ConicalGradientPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("RadialGradientPattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    texture = new QListWidgetItem(k->texturesList);
    texture->setIcon(QIcon(THEME_DIR + "icons/brush.png"));
    texture->setFont(QFont("verdana", 8));
    texture->setToolTip("TexturePattern");
    texture->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    k->texturesList->setFixedWidth(255);
    k->texturesList->setFixedHeight(120);

    addChild(k->texturesList);

    connect(k->texturesList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(setBrushStyle(QListWidgetItem *)));
    
    boxLayout()->addStretch(2);
    
    setWindowIcon(QIcon(THEME_DIR + "icons/brush.png"));
    reset();
}

KTPenWidget::~KTPenWidget()
{
    delete k;
}

void KTPenWidget::setThickness(int value)
{
    if (value > 0) {
        k->pen.setWidth(value);

        KCONFIG->beginGroup("PenParameters");
        KCONFIG->setValue("Thickness", value);

        emitPenChanged();
    }
}

void KTPenWidget::setStyle(int s)
{
    kFatal() << "KTPenWidget::setStyle - emiting emitPenChanged()";

    k->pen.setStyle(Qt::PenStyle(k->style->itemData(s).toInt()));
    emitPenChanged();
}

void KTPenWidget::setJoinStyle(int s)
{
    k->pen.setJoinStyle(Qt::PenJoinStyle(k->joinStyle->itemData(s).toInt()));
    emitPenChanged();
}

void KTPenWidget::setCapStyle(int s)
{
    k->pen.setCapStyle(Qt::PenCapStyle(k->capStyle->itemData(s).toInt()));
    emitPenChanged();
}

void KTPenWidget::setBrushStyle(QListWidgetItem *item)
{
    int index = k->texturesList->row(item);

    QBrush brush; 
    brush.setStyle(Qt::BrushStyle(index+1));
    k->pen.setBrush(brush);
    emitPenChanged();
}

void KTPenWidget::reset()
{
    blockSignals(true);
    k->capStyle->setCurrentIndex(2);
    k->joinStyle->setCurrentIndex(2);
    //setThickness(3);
    blockSignals(false);
    k->style->setCurrentIndex(1);
    
    k->pen.setColor(QColor(0, 0, 0)); // invalid color
}

QPen KTPenWidget::pen() const
{
    return k->pen;
}

void KTPenWidget::emitPenChanged()
{
    emit penChanged(k->pen);
    
    KTPaintAreaEvent event(KTPaintAreaEvent::ChangePen, k->pen);
    emit paintAreaEventTriggered(&event);
}

