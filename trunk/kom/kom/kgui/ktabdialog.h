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

#ifndef KTABDIALOG_H
#define KTABDIALOG_H

#include "kgui/ktabwidget.h"

#include <QDialog>
#include <QHash>

typedef QHash<int, QPushButton *> Buttons;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KTabDialog : public QDialog
{
    Q_OBJECT
    public:
        enum Button
        {
            Help    = 1<<2,
            Ok      = 1<<3,
            Apply   = 1<<4,
            Cancel  = 1<<5,
            Custom1 = 1<<6,
            Custom2 = 1<<7,
            Custom3 = 1<<8
        };
        KTabDialog(QWidget *parent = 0, bool modal = true);
        KTabDialog(int buttons = Ok|Cancel, QWidget *parent = 0, bool modal = true);
        ~KTabDialog();
        
        void addTab(QWidget * child, const QString & label);
        void addTab(QWidget * child, const QIcon & iconset, const QString & label);
        
        QWidget *currentTab();
        
        void setButtonText(Button b, const QString &text);
        QPushButton *button(Button b);
        
        KTabWidget *tabWidget() const;
        
    private:
        void setupButtons(int buttons);
        
    public slots:
        virtual void ok();
        virtual void cancel();
        virtual void apply();
        virtual void help(){};
        virtual void custom1() {};
        virtual void custom2() {};
        virtual void custom3() {};
        
    private:
        KTabWidget *m_tabWidget;
        Buttons m_buttons;
};

#endif
