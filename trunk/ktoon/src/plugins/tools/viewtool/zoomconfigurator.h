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

#ifndef ZOOMCONFIGURATOR_H
#define ZOOMCONFIGURATOR_H

#include <QResizeEvent>
#include <QLabel>
#include <QRadioButton>
#include <QDoubleSpinBox>

class QTableWidget;
class QTableWidgetItem;

/**
 * @author Jorge Cuadrado <krawek@toonka.com>
*/
class ZoomConfigurator : public QWidget
{
    Q_OBJECT;
    public:
        ZoomConfigurator(QWidget *parent = 0);
        ~ZoomConfigurator();
        //bool zoomIn();
        double getFactor() const;

    protected:
        void resizeEvent(QResizeEvent *);

    /* private slots:
        void setZoomIn();
        void setZoomOut(); */

    private:
        //QRadioButton *in;
        //QRadioButton *out;
        QDoubleSpinBox *factor;
        QLabel *scale;
};

#endif
