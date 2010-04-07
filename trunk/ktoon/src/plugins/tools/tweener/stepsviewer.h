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

#ifndef STEPSVIEWER_H
#define STEPSVIEWER_H

#include <QTableWidget>

class QGraphicsPathItem;
class KTTweenerStep;

/**
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
*/

class StepsViewer : public QTableWidget
{
    Q_OBJECT
    public:
        StepsViewer(QWidget *parent = 0);
        ~StepsViewer();
        void setPath(const QGraphicsPathItem *path);
        
        QVector<KTTweenerStep *> steps();
        int totalSteps();
        void cleanRows();
        
    private:
        struct Private;
        Private *const k;
};

#endif