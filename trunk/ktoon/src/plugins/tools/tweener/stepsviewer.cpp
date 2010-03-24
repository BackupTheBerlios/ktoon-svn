/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2007 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include <QGraphicsPathItem>
#include <QDebug>
#include <cmath>

#include "stepsviewer.h"
#include "kttweenerstep.h"
#include "spinboxdelegate.h"

struct StepsViewer::Private
{
    QPolygonF points;
    QPolygonF stops;
    QList<int> frames;
};

StepsViewer::StepsViewer(QWidget *parent) : QTableWidget(parent), k(new Private)
{
    setFont(QFont("Arial", 8, QFont::Normal, false));
    setColumnCount(2);
    setHorizontalHeaderLabels(QStringList() << tr("Interval") << tr("Frames"));
    setItemDelegate(new SpinBoxDelegate);

    //setMinimumWidth(142);
    setMaximumWidth(120);
}

StepsViewer::~StepsViewer()
{
    delete k;
}

void StepsViewer::setPath(const QGraphicsPathItem *path)
{
    QPolygonF points = path->path().toFillPolygon();
    
    if (points.count() > 2) {
        k->points.clear();
        k->frames.clear();
        
        points.pop_back();
        k->points = points;
        points.pop_front();
        
        int count = 1;
        setRowCount(0);

        for (int i = 1; i < path->path().elementCount(); i++) {
             QPainterPath::Element e  = path->path().elementAt(i);
            
             if (e.type != QPainterPath::CurveToElement) {
                 if (e.type  == QPainterPath::CurveToDataElement && path->path().elementAt(i-1).type  == QPainterPath::CurveToElement) 
                     continue;
             
                 k->stops << e;
                
                 int frames = 0;
                
                 QPolygonF::iterator it = points.begin();
                
                 while (it != points.end()) {
                        frames++;
                        if (e == (*it))
                            break;
                        else
                            it = points.erase(it);
                 }

                 k->frames << frames;
                 setRowCount(rowCount() + 1);

                 QTableWidgetItem *intervalItem = new QTableWidgetItem();
                 intervalItem->setTextAlignment(Qt::AlignCenter);
                 intervalItem->setText(QString::number(count));
                 intervalItem->setFlags(intervalItem->flags() & ~Qt::ItemIsEditable);

                 QTableWidgetItem *framesItem = new QTableWidgetItem();
                 framesItem->setTextAlignment(Qt::AlignCenter);
                 framesItem->setText(QString::number(frames));
                
                 setItem(count-1, 0, intervalItem);
                 setItem(count-1, 1, framesItem);
                 setRowHeight(count-1, 20);

                 count++;
            }
        }
    }
}

QVector<KTTweenerStep *> StepsViewer::steps()
{
    QVector<KTTweenerStep *> s;
    QVectorIterator<QPointF> point(k->points);
    int count = 0;
    
    KTTweenerStep *step = new KTTweenerStep(0);
    step->setPosition(point.next());
    s << step;
    
    for (int i = 0; i < rowCount(); i++) {
        for (int j = 0; j < k->frames[i]; j++) {
             int frames = item(i,1)->text().toInt();
             count += (int)::ceil(frames/k->frames[i]);
             KTTweenerStep *step = new KTTweenerStep(count);
             step->setPosition(point.next());
             s << step;
        }
    }

    return s;
}

int StepsViewer::totalSteps()
{
    int total = 0;
    for (int i = 0; i < rowCount(); i++)
         total += item(i,1)->text().toInt();

    return total;
}

