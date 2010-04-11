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

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QFrame>
class QGraphicsPathItem;
/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class Configurator : public QFrame
{
    Q_OBJECT
    public:
        Configurator(QWidget *parent = 0);
        ~Configurator();
        
        void updateSteps(const QGraphicsPathItem *path);
        QString steps(int currentFrame, QString path);
        int totalSteps();
        void activatePathMode();
        void activateSelectionMode();
        
    private slots:
        void emitOptionChanged(int option);
        
    signals:
        void clikedCreatePath();
        void clikedSelect();
        void clikedApplyTweener();
        
    private:
        struct Private;
        Private *const k;
};

#endif
