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

#ifndef KTPENWIDGET_H
#define KTPENWIDGET_H

#include <QPen>
#include <QListWidgetItem>

#include <ktmodulewidgetbase.h>

#include <kgui/keditspinbox.h>
#include <QComboBox>
#include <QLabel>

class KTPaintAreaEvent;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTPenWidget : public KTModuleWidgetBase
{
    Q_OBJECT
    public:
        KTPenWidget(QWidget *parent = 0);
        ~KTPenWidget();
        QPen pen() const;
        
    private:
        void emitPenChanged();
        void emitBrushChanged();
        
    private:
        struct Private;
        Private *const k;
        
    public slots:
        void init();
        void setThickness(int value);
        void setPenColor(const QColor color);
        
    private slots:
        void setStyle(int s);
        void setJoinStyle(int s);
        void setCapStyle(int s);
        void setBrushStyle(QListWidgetItem *item);
        
    signals:
        void penChanged(const QPen &pen);
        void brushChanged(const QBrush &brush);
        void paintAreaEventTriggered(const KTPaintAreaEvent *e);

};

#endif
