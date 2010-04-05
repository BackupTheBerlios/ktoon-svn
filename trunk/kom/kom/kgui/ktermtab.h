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

#ifndef KTERMTAB_H
#define KTERMTAB_H

#include <QToolButton>
#include <kcore/kglobal.h>
#include <kgui/ktabwidget.h>

class KTerm;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KTermTab : public KTabWidget
{
    Q_OBJECT
    public:
        KTermTab(QWidget *parent = 0);
        ~KTermTab();
        
    private slots:
        void closeTermTab();
        
    public slots:
        void closeTermTab(KTerm *term);
        void newTerm();
        void closeCurrentTerm();
        
    private:
        QToolButton *m_newTab, *m_closeTab;
        
};

#endif
