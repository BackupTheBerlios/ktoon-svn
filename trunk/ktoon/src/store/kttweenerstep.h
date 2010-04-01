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

#ifndef KTTWEENERSTEP_H
#define KTTWEENERSTEP_H

#include <ktabstractserializable.h>
#include <QPointF>

#include "ktglobal_store.h"

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTTweenerStep : public KTAbstractSerializable
{
    public:
        enum Type {
            None = 0x0,
            Position = 0x01,
            Translation = 0x02,
            Rotation = 0x04,
            Shear = 0x8,
            Scale = 0x10
        };
        
        KTTweenerStep(int n);
        
        ~KTTweenerStep();
        
        void setPosition(const QPointF &pos);
        void setTranslation(double dx, double dy);
        void setRotation(double angle);
        void setShear(double sh, double sv);
        void setScale(double sx, double sy);
        
        QPointF position() const;
        double horizontalScale() const;
        double verticalScale() const;
        double horizontalShear() const;
        double verticalShear() const;
        double rotation() const;
        double xTranslation() const;
        double yTranslation() const;
        
        
        bool has(Type type) const;
        int n() const;
    
        virtual QDomElement toXml(QDomDocument& doc) const;
        virtual void fromXml(const QString& xml);
        
        //static QDomDocument createXml(int frames, const QVector<KTTweenerStep *> &steps);
        
    private:
        struct Private;
        Private *const k;
};

#endif
