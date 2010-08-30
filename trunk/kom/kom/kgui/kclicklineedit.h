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

#ifndef KCLICKLINEEDIT_H
#define KCLICKLINEEDIT_H

#include <QLineEdit>
#include "kcore/kglobal.h"

class K_GUI_EXPORT KClickLineEdit : public QLineEdit
{
    public:
        KClickLineEdit(const QString &msg, QWidget *parent = 0);
        ~KClickLineEdit();
        
        void setClickMessage(const QString &msg);
        QString clickMessage() const;
        
        virtual void setText(const QString& txt);
    
    protected:
        virtual void paintEvent(QPaintEvent *ev);
        virtual void focusInEvent(QFocusEvent *ev);
        virtual void focusOutEvent(QFocusEvent *ev);
    
    private:
        struct Private;
        Private *const k;
};

#endif
