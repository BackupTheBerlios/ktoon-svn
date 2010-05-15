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

#ifndef KTERM_H
#define KTERM_H

#include <qwidget.h>
#include <QProcess>
#include <QX11EmbedContainer>
#include <kcore/kglobal.h>

/**
    @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KTerm : public QX11EmbedContainer
{
    Q_OBJECT
    public:
        KTerm(QWidget *w = 0);
        ~KTerm();
        
        QSize sizeHint() const;
        
    public slots:
        void showTerm();
        void closeTerm(int ec, QProcess::ExitStatus s);
        
    signals:
        void termClosed();
        
    private:
        QProcess *m_process;
};

#endif
