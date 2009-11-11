/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktpaintareastatus.h"

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QIntValidator>

#include <kgui/kseparator.h>

#include "ktviewdocument.h"
#include "ktglobal.h"
#include "ktbrushmanager.h"

/**
 * This class defines the options panel in the bottom of the paint area.
 * Controls for Rotation, Antialising and OpenGL
 * @author David Cuadrado <krawek@toonka.com>
*/

class ColorWidget : public QWidget
{
    public:
        ColorWidget() : m_brush(Qt::transparent) {};
        ~ColorWidget() {};
        void setBrush(const QBrush &brush);
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *);

    private:
        QBrush m_brush;
};

QSize ColorWidget::sizeHint() const
{
    QSize size(40, -1);

    return size;
}

void ColorWidget::setBrush(const QBrush &brush)
{
    m_brush = brush;
    update();
}

void ColorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_brush);
}

class BrushStatus : public QWidget
{
    public:
        BrushStatus();
        ~BrushStatus();

        void setForeground(const QPen &pen);
        void setBackground(const QBrush &brush);

    private:
        ColorWidget *m_pen;
        ColorWidget *m_brush;
};

BrushStatus::BrushStatus()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(2);
    layout->setSpacing(2);

    m_pen = new ColorWidget;
    m_brush = new ColorWidget;

    layout->addWidget(m_pen);
    layout->addWidget( new KSeparator(Qt::Vertical ) );
    layout->addWidget(m_brush);
}

BrushStatus::~BrushStatus()
{
}

void BrushStatus::setForeground(const QPen &pen)
{
    m_pen->setBrush( pen.brush() );
}

void BrushStatus::setBackground(const QBrush &brush)
{
    m_brush->setBrush(brush);
}

////////////////

struct KTPaintAreaStatus::Private
{
    KTViewDocument *viewDocument;
    QPushButton *antialiasHint;
    QComboBox *renderer;
    BrushStatus *brushStatus;
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

    rotLayout->addWidget( k->rotation);

    addPermanentWidget(rotContainer);

    connect(k->rotation, SIGNAL(activated(const QString &)), this, SLOT(applyRotationFromItem(const QString &)));

    ///////

    k->antialiasHint = new QPushButton;
    k->antialiasHint->setFocusPolicy( Qt::NoFocus);

    k->antialiasHint->setText(tr("Antialiasing"));
    k->antialiasHint->setCheckable(true);
	
    //k->antialiasHint->setChecked( parent->renderHints() & QPainter::Antialiasing );
    k->antialiasHint->setChecked(true);
	
    addPermanentWidget(k->antialiasHint/*,1*/);

    k->renderer = new QComboBox;

#ifdef QT_OPENGL_LIB
    k->renderer->addItem(tr("OpenGL"), KToon::OpenGL );
#endif

    k->renderer->addItem(tr("Native"), KToon::Native );
    k->renderer->setCurrentIndex(1);

    addPermanentWidget(k->renderer/*,1*/);

    k->brushStatus = new BrushStatus;
    addPermanentWidget(k->brushStatus);

    connect(k->antialiasHint, SIGNAL(toggled(bool)), this, SLOT(selectAntialiasingHint(bool) ));
    connect(k->renderer, SIGNAL(activated(int)), this, SLOT(selectRenderer(int)));

    k->brushStatus->setBackground( k->viewDocument->brushManager()->brush() );
    k->brushStatus->setForeground( k->viewDocument->brushManager()->pen() );
}

KTPaintAreaStatus::~KTPaintAreaStatus()
{
    delete k;
}


void KTPaintAreaStatus::selectAntialiasingHint(bool use)
{
    k->viewDocument->setAntialiasing( use ); 
}

void KTPaintAreaStatus::selectRenderer(int id)
{
    KToon::RenderType type = KToon::RenderType(k->renderer->itemData(id ).toInt());

    if (type == KToon::OpenGL)
        k->viewDocument->setOpenGL(true);
    else
        k->viewDocument->setOpenGL(false);
}


void KTPaintAreaStatus::setBrush(const QBrush &brush)
{
    k->brushStatus->setBackground( brush );
}

void KTPaintAreaStatus::setPen(const QPen &pen)
{
    k->brushStatus->setForeground( pen );
}


void KTPaintAreaStatus::applyRotationFromItem(const QString & text)
{
    int angle = text.toInt();

    if (angle < 0)
        angle += 360;

    k->viewDocument->setRotationAngle(angle);
}

