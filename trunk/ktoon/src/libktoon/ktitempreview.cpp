/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
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

#include "ktitempreview.h"
#include "ktproxyitem.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <kcore/kdebug.h>

struct KTItemPreview::Private
{
    KTProxyItem *proxy;
};

KTItemPreview::KTItemPreview(QWidget *parent) : QWidget(parent), k(new Private)
{
    k->proxy = 0;
}

KTItemPreview::~KTItemPreview()
{
    delete k;
}

QSize KTItemPreview::sizeHint() const
{
    if (k->proxy)
        return k->proxy->boundingRect().size().toSize() + QSize(10,10);
    
    return QWidget::sizeHint().expandedTo(QSize(100,100));
}

void KTItemPreview::render(QGraphicsItem *item)
{
    if (!k->proxy)
        k->proxy = new KTProxyItem(item);
    else
        k->proxy->setItem(item);
    
    update();
}

void KTItemPreview::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    
    if (k->proxy) {
        QStyleOptionGraphicsItem opt;
        opt.state = QStyle::State_None;
        
        if (k->proxy->isEnabled())
            opt.state |= QStyle::State_Enabled;
        
        opt.exposedRect = QRectF(QPointF(0,0), k->proxy->boundingRect().size());
        opt.levelOfDetail = 1;
        
        //QMatrix matrix = k->proxy->sceneMatrix();
        QTransform matrix = k->proxy->sceneTransform();
        
        //QRect r(15,15, rect().width()-15 , rect().height()-15);
        
        opt.palette = palette();
        //p.setMatrix(matrix);
        p.setTransform(matrix);

        kFatal() << "Item Width: " << rect().width();
        kFatal() << "Item Height: " << rect().height();
        
        //p.translate((rect().width() - opt.exposedRect.width())/2, (rect().height() - opt.exposedRect.height())/2);

        if (QGraphicsPathItem *path = qgraphicsitem_cast<QGraphicsPathItem *>(k->proxy->item())) {

            if (opt.exposedRect.width() > rect().width() || opt.exposedRect.height() > rect().height()) {
                float distance = 0;
                float base = 0;

                if (path->path().boundingRect().width() > path->path().boundingRect().height()) {
                    distance = path->path().boundingRect().width();
                    base = rect().width();
                } else {
                    distance = path->path().boundingRect().height(); 
                    base = rect().height();
                }

                float calculation = (base*100)/distance;
                float factor = calculation / 100;
                float newWidth = path->path().boundingRect().width() * factor;
                float newPos = (path->path().boundingRect().width()-newWidth)/2;
                p.scale(factor, factor);
                //p.translate(newPos, 0);
                p.translate(-path->path().boundingRect().topLeft().x(), -path->path().boundingRect().topLeft().y());
                
                kFatal() << "Scale Factor: " << factor;
                kFatal() << "Calculation: " << calculation;
                kFatal() << "Width: " << rect().width() << " - " <<  path->path().boundingRect().width();
                kFatal() << "Height: " << rect().height() << " - " << path->path().boundingRect().height();

            } else {
                p.translate((rect().width() - opt.exposedRect.width())/2, (rect().height() - opt.exposedRect.height())/2);
                p.translate(-path->path().boundingRect().topLeft().x(), -path->path().boundingRect().topLeft().y());
            }

        } else {

          if (opt.exposedRect.width() > rect().width() || opt.exposedRect.height() > rect().height()) {
              float distance = 0;
              float base = 0;
              bool horizontal = false;

              if (opt.exposedRect.width() < opt.exposedRect.height()) {
                  distance = opt.exposedRect.width();  
                  base = rect().width();
                  horizontal = true;
              } else {
                  distance = opt.exposedRect.height();
                  base = rect().height();
              }
 
              //float calculation = (base*100)/distance;
              float factor = base/distance;

              if (horizontal) {
              kFatal() << "Doing horizontal calc!";
              kFatal() << "Factor: " << factor;
              float newWidth = opt.exposedRect.height() * factor;
              float newPos = (opt.exposedRect.height()-newWidth)/2;
              p.scale(factor, factor);
              p.translate(0, -newPos);
              } else {
              kFatal() << "Doing vertical calc!";
              float newWidth = opt.exposedRect.width() * factor;
              float newPos = (opt.exposedRect.width()-newWidth)/2;
              p.scale(factor, factor);
              p.translate(newPos, 0);
              }

          } else {
              p.translate((rect().width() - opt.exposedRect.width())/2, (rect().height() - opt.exposedRect.height())/2);
          }

        }

        k->proxy->paint(&p, &opt, this); // paint isn't const...
    }
}
