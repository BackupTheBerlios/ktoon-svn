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

#ifndef KDATEPICKER_H
#define KDATEPICKER_H

#include <QFrame>
#include <QToolButton>

#include "kgui/kdatetable.h"
#include "kcore/kglobal.h"

class QComboBox;
class QLabel;

/**
 * 
 * @author David Cuadrado <krawek@gmail.com>
 * 
*/

class K_GUI_EXPORT KDatePicker : public QFrame
{
    Q_OBJECT;
    public:
        KDatePicker(QWidget *parent = 0);
        ~KDatePicker();
        void setDate(const QDate &date);
        QDate date() const;
        
    private:
        void fillWeeks(const QDate &date);
        
    public slots:
        void setWeek(int week);
        void setYear(int year);
        
    protected slots:
        void previousYear();
        void nextYear();
        
        void previousMounth();
        void nextMounth();
        
    private slots:
        void mounthFromAction(QAction *act);
        
    signals:
        void dateChanged(const QDate &date);
        
    private:
        QComboBox *m_week;
        KDateTable *m_dateTable;
        
        class EditableButton;
        
         QToolButton *m_mounth;
         EditableButton *m_year;
};

class KDatePicker::EditableButton : public QToolButton
{
    Q_OBJECT
    public:
        EditableButton();
        ~EditableButton();
        
    public slots:
        void edit();
        
    private slots:
        void emitYearSelected();
        
    signals:
        void yearSelected(int year);
        
    private:
        QLineEdit *m_editor;
};

#endif
