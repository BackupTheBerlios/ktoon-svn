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

#ifndef KRADIOBUTTONGROUP_H
#define KRADIOBUTTONGROUP_H

#include <QGroupBox>
#include <QRadioButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KRadioButtonGroup : public QGroupBox
{
    Q_OBJECT
    public:
        KRadioButtonGroup(const QString &title, Qt::Orientation orientation, QWidget *parent = 0);
        ~KRadioButtonGroup();
        
        void addItem(const QString & text, int index = -1);
        void addItems(const QStringList &texts);
        
        int currentIndex() const;
        void setCurrentIndex(int index);
        
    private slots:
        void emitButtonId(QAbstractButton *button);
        
    signals:
        void clicked(int index);
        
    private:
        Qt::Orientation m_orientation;
        QBoxLayout *m_layout;
        QButtonGroup *m_buttonGroup;
};

#endif
