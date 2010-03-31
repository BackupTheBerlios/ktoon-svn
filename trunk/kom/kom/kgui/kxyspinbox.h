/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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


#ifndef KXYSPINBOX_H
#define KXYSPINBOX_H

#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KXYSpinBox : public QGroupBox
{
    Q_OBJECT
    public:
        KXYSpinBox(const QString &title, QWidget *parent = 0);
        ~KXYSpinBox();
        void setSingleStep(int step);
        void setMinimum(int min);
        void setMaximum(int max);
        void setX(int x);
        void setY(int y);
        int x();
        int y();
        void setModifyTogether(bool enable);

    private slots:
        void updateXValue();
        void updateYValue();
        void toggleModify();

    private:
        QLabel *m_textX;
        QLabel *m_textY;
        QSpinBox *m_x;
        QSpinBox *m_y;
        QPushButton *m_separator;
        bool m_modifyTogether;

    /*
    signals:
        void valueXChanged(double);
        void valueYChanged(double);
        void valueXYChanged(double, double);
    */
};

#endif
