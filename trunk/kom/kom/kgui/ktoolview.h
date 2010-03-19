/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
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

#ifndef KTOOLVIEW_H
#define KTOOLVIEW_H

#include <QDockWidget>
#include <QIcon>
#include <kgui/kideality.h>

class KViewButton;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_IDEAL_EXPORT KToolView : public QDockWidget
{
    Q_OBJECT;

    public:
        KToolView(const QString &title, const QIcon &icon = QIcon(), QWidget * parent = 0);
        virtual ~KToolView();

        void setDescription(const QString &description);
        KViewButton *button() const;
        void expandDock(bool flag);
        bool isExpanded();
        void setExpandingFlag();
        void setSizeHint();
        QSize sizeHint() const;
        void setPerspective(int wsp);
        int perspective() const;
        void setFixedSize(int s);
        int fixedSize() const;
        void enableButton(bool flag);
        QString getObjectID();
        bool isChecked();

    public slots:
        void saveSize(bool checked);

    private:
        void setup(const QString &label);

    protected:
        virtual void showEvent(QShowEvent *e);

/*		
#if QT_VERSION < 0x040200
    protected:
        virtual bool event(QEvent *e);

    private:
        Qt::DockWidgetArea m_area;
#endif
*/

    private:
        KViewButton *m_button;
        int m_size;
        int m_perspective;
        bool expanded;
};

#endif
