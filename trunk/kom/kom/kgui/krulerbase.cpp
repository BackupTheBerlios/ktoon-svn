/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "krulerbase.h"
#include "kdebug.h"

#include <iostream>
using namespace std;
#include <QMouseEvent>
#include <QFrame>
#include <QResizeEvent>
#include <QPaintEvent> 
#include <QPainter>

struct KRulerBase::Private
{
    int position;
    int separation;
    int width; 
    int height;
    bool drawPointer;

    Qt::Orientation orientation;

    QMenu *menu;
    QPointF zero;
    QPolygonF pArrow;

    double scaleFactor;
};

KRulerBase::KRulerBase(Qt::Orientation orientation, QWidget *parent) : QFrame(parent), k(new Private)
{
    k->position = 0;
    k->orientation = orientation;
    k->drawPointer = false;
    k->separation = 10;

    k->zero = QPointF(0,0);
    k->pArrow = QPolygonF(3);

    k->scaleFactor = 1.0;

    if (k->orientation == Qt::Horizontal) {
        setMaximumHeight(10);
        setMinimumHeight(20);

        k->width = width();
        k->height = height();

        k->pArrow << QPointF(0.0, 0.0);
        k->pArrow << QPointF(5.0,  5.0);
        k->pArrow << QPointF(10.0, 0.0);

        k->pArrow.translate(0, 10);
    } else {
        setMaximumWidth(20);
        setMinimumWidth(20);

        k->width = height();
        k->height =  width();

        k->pArrow << QPointF(0.0, 0.0);
        k->pArrow << QPointF(5.0, 5.0);
        k->pArrow << QPointF(0.0, 10.0);

        k->pArrow.translate(10,0);
    }


    setMouseTracking ( true );

    connect(this, SIGNAL(displayMenu(KRulerBase *, QPoint)), this, SLOT(showMenu(KRulerBase *, QPoint)));

    k->menu = new QMenu(this);

    QAction *to5 = k->menu->addAction( tr("Change scale to 5..."));
    QAction *to10 = k->menu->addAction( tr("Change scale to 10..."));

    connect(to5, SIGNAL(triggered()), this, SLOT(changeScaleTo5pts()));
    connect(to10, SIGNAL(triggered()), this, SLOT(changeScaleTo10pts()));
}


KRulerBase::~KRulerBase()
{
    delete k;
}

void KRulerBase::paintEvent ( QPaintEvent * )
{
    QPainter p(this);

    if (k->orientation == Qt::Vertical)
        p.scale(1.0, k->scaleFactor);
    else
        p.scale(k->scaleFactor, 1.0);

    drawScale(&p);
    p.setBrush(palette ().color(QPalette::Foreground));

    p.save();

    p.drawConvexPolygon(k->pArrow);
    p.restore();

    p.end();
	
}

void KRulerBase::drawScale(QPainter *painter)
{
    painter->save();
    QFont kfont(font().family(), 7);
    QFontMetrics fm(kfont);
    painter->setFont(kfont);

    int fact = 1;
    int init;

    if (k->orientation == Qt::Horizontal) {
        painter->translate(k->zero.x(), 0);
        init = (int)k->zero.x();
        painter->drawLine(-390,height()-1, width(), height()-1);
    } else {
        painter->drawLine(width()-1, 0, width()-1, height());
        fact = -1;
        painter->translate(0, k->zero.y());
        painter->rotate(90);
        init = (int)k->zero.y();
    }

    for (int i = 0; i < k->width; i += k->separation) {
         QSize sizeFont = fm.size(Qt::TextSingleLine, QString::number(i));
 
         if (i % 100 == 0) { // FIX ME
             painter->drawLine ( i, k->height*fact, i, 0 );
             if (k->orientation == Qt::Vertical)
                 painter->drawText( QPoint(i + 3, k->height/2 - sizeFont.height()), QString::number(i));
             else
                 painter->drawText( QPoint(i + 3, k->height/2), QString::number(i));
         } else {
                 painter->drawLine ( i, k->height*fact, i, k->height*fact - k->height/4*fact );
         }
    }

    cout << "Var: " << init << endl;

    for (int i = init; i > 0 ; i -= k->separation) {
         QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
         if (i % 100  == 0) { // FIX ME
             // Numbers and marks
             painter->drawLine ( -i, k->height*fact, -i, 0 );
             if (k->orientation == Qt::Vertical)
                 painter->drawText(QPoint(-i, k->height/2 -sizeFont.height()), QString::number(-i));
             else
                 painter->drawText(-i, k->height/2,  QString::number(-i));
         } else {
                 // Short Lines
                 painter->drawLine ( -i, k->height*fact, -i, k->height*fact - k->height/4*fact );
         }
    }

    painter->restore();
}

void KRulerBase::resizeEvent ( QResizeEvent * )
{
    if (k->orientation == Qt::Horizontal) {
        k->width = width();
        k->height = height();
    } else if (k->orientation == Qt::Vertical) {
        k->width = height();
        k->height =  width();
    }

    update();
}

void KRulerBase::mouseMoveEvent ( QMouseEvent * e )
{
    if (k->drawPointer)
        movePointers(e->pos()/*-QPoint(d->zero, d->zero)*/);
}

void KRulerBase::setDrawPointer(bool yes)
{
    k->drawPointer = yes;
    update();
}

void KRulerBase::setSeparation(int sep)
{
    if (sep > 0 && sep <= 10000) {
        k->separation = sep;
        update();
    } else {
        kError() << "I can't assign separation : " << sep << endl;
    }
}

void KRulerBase::mousePressEvent (QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        emit displayMenu(this, e->globalPos());
}

Qt::Orientation KRulerBase::orientation()
{
    return k->orientation;
}

int KRulerBase::separation() const
{
    return k->separation;
}

double KRulerBase::scaleFactor() const
{
    return k->scaleFactor;
}

void KRulerBase::showMenu(KRulerBase *ruler, QPoint pos)
{
    if (ruler)
        k->menu->popup(pos);
}

void KRulerBase::slide(int value)
{
    if (k->orientation == Qt::Horizontal)
        move(-value+k->height, pos().y());
    else
        move(pos().x(), -value+k->height);
}

QPointF KRulerBase::zero() const
{
    return k->zero;
}

void KRulerBase::translateArrow(double dx, double dy)
{
    k->pArrow.translate(dx, dy);
}

void KRulerBase::setZeroAt(const QPointF & pos)
{
    k->zero = pos;
    update();
}

void KRulerBase::scale(double factor)
{
    k->scaleFactor = factor;
    update();
}

QSize KRulerBase::sizeHint() const
{
    if (k->orientation == Qt::Horizontal)
        return QSize(k->width/3, height());

    return QSize( width(), k->height/3 );
}

void KRulerBase::changeScaleTo5pts()
{
    setSeparation(5);
}

void KRulerBase::changeScaleTo10pts()
{
    setSeparation(10);
}
