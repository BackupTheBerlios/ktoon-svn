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


#ifndef KBUTTONBAR_H
#define KBUTTONBAR_H

#include <kgui/kideality.h>

#include <QToolBar>
#include <QButtonGroup>
#include <QMap>
#include <QTimer>

class KViewButton;
class KToolView;
class QAction;
class QMenu;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_IDEAL_EXPORT KButtonBar : public QToolBar
{
    Q_OBJECT;
    public:
        KButtonBar(Qt::ToolBarArea area, QWidget *parent = 0);
        ~KButtonBar();

        void addButton(KViewButton *viewButton);
        void removeButton(KViewButton *viewButton);
        bool isEmpty() const;
        void disable(KViewButton *v);
        void enable(KViewButton *v);

        bool isExclusive() const;
        bool autohide() const;
        void showSeparator(bool e);

        int count() const;
        void setEnableButtonBlending(bool enable);

    public slots:
        void onlyShow(KToolView *tool, bool ensureVisible = false);
        void setExclusive(bool excl);
        void setAutoHide(bool autohide);
        void setShowOnlyIcons();
        void setShowOnlyTexts();

    private:
        QMenu *createMenu();

    private slots:
        void hideOthers(QAbstractButton *source);
        void doNotHide();

    protected:
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void enterEvent(QEvent *e);
        virtual void leaveEvent(QEvent *e);

    private:
        QButtonGroup m_buttons;
        QMap<QWidget *, QAction *> m_actionForWidget;
        QAction *m_separator;
        QTimer m_hider;
        bool m_autoHide;
        bool m_blockHider;
};

#endif
