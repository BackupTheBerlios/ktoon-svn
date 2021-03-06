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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
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

#ifndef KELABEL_H
#define KELABEL_H

#include <kgui/ksqueezelabel.h>
#include <kcore/kglobal.h>

#include <QLineEdit>
#include <QMouseEvent>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KELabel : public KSqueezeLabel
{
    Q_OBJECT

    public:
        KELabel(const QString &text, QWidget *parent = 0);
        ~KELabel();
        void setValidator(const QValidator * v);
        
    public slots:
        void edit();
        void clearFocus();
        
    private slots:
        void applyText();
        
    signals:
        void edited(const QString &);
        
    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *mouse_event);
        
    private:
        QLineEdit *m_editor;
};

#endif
