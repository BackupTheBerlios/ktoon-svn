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

#include "ktermtab.h"

#ifdef Q_WS_X11

#include <QtGlobal> 
#include <QTabBar>

#include <qlabel.h>
#include "kterm.h"

KTermTab::KTermTab(QWidget *parent) : KTabWidget(parent)
{
    // tabBar()->setShape(QTabBar::TriangularNorth);
    m_newTab = new QToolButton(this);
    m_newTab->setText(tr("New"));
    
    connect(m_newTab, SIGNAL(clicked()), this, SLOT(newTerm()));
    
    m_closeTab = new QToolButton(this);
    m_closeTab->setText(tr("Close"));
    
    connect(m_closeTab, SIGNAL(clicked()), this, SLOT(closeCurrentTerm()));
    
    setCornerWidget(m_newTab, Qt::TopLeftCorner);
    setCornerWidget(m_closeTab, Qt::TopRightCorner);
    
    m_closeTab->show();
    m_newTab->show();
    
    newTerm();
}

KTermTab::~KTermTab()
{
}

void KTermTab::newTerm()
{
    KTerm *term = new KTerm;
    addTab(term, tr("Console %1").arg(count()));
    term->showTerm();
    connect(term, SIGNAL(termClosed()), this, SLOT(closeTermTab()));
}

void KTermTab::closeCurrentTerm()
{
    removeTab(currentIndex());
}

void KTermTab::closeTermTab()
{
    KTerm *term = qobject_cast<KTerm *>(sender());

    if (term)
        closeTermTab(term);
}

void KTermTab::closeTermTab(KTerm *term)
{
    removeTab(indexOf(term));
}

#endif // Q_WS_X11
