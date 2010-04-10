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

#ifndef KTITEMTWEENER_H
#define KTITEMTWEENER_H

#include <QObject>
#include <QMatrix>
#include <QPointF>

#include "kttweenerstep.h"
#include "ktglobal_store.h"

class QGraphicsItem;

/**
 * @todo - setColorAt, setZAt
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class STORE_EXPORT KTItemTweener : public QObject, public KTAbstractSerializable
{
    public:
        KTItemTweener(int frames = 0, QObject *parent = 0);
        ~KTItemTweener();

        QString name();
        
        double horizontalScaleAt(int step) const;
        double horizontalShearAt(int step) const;
        
        void setItem(QGraphicsItem * item);
        void setPosAt(int step, const QPointF & point);
        void setRotationAt(int step, double angle);
        void setScaleAt(int step, double sx, double sy);
        void setShearAt(int step, double sh, double sv);
        void setTranslationAt(int step, double dx, double dy);
        
        QMatrix matrixAt(int step) const;
        QPointF posAt(int step) const;
        double rotationAt(int step) const;
        double verticalScaleAt(int step) const;
        double verticalShearAt(int step) const;
        double xTranslationAt(int step) const;
        double yTranslationAt(int step) const;
        
        void addStep(const KTTweenerStep &step);
        
        QGraphicsItem *item() const;
        
        void setFrames(int frames);
        int frames() const;
        
        void setStep(int step);
        
        void fromXml(const QString &xml);
        QDomElement toXml(QDomDocument &doc) const;
        static QDomDocument tweenToXml(QString name, int initFrame, int frames, const QVector<KTTweenerStep *> &steps);
        
    private:
        struct Private;
        Private *const k;
};

#endif
