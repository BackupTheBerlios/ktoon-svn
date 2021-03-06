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

#ifndef KMAINWINDOW_H
#define KMAINWINDOW_H

#include <kgui/kideality.h>

// Project begin: Aug 4 2006

#include <QMainWindow>
#include <QHash>
#include <QMap>
#include <QKeySequence>

class KButtonBar;
class KToolView;
class KMainWindowAbstractSettings;

/**
 * iDeality Main Window
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_IDEAL_EXPORT KMainWindow : public QMainWindow
{
    Q_OBJECT;
    public:
        enum
         {
           None = 0, DefaultPerspective
         };

        KMainWindow(QWidget *parent = 0);
        ~KMainWindow();

        KToolView *addToolView(QWidget *widget, Qt::DockWidgetArea area, int perspective = DefaultPerspective, const QString &code = QString(), QKeySequence shortcut = QKeySequence(""));

        void removeToolView(KToolView *view);

        // FIXME: remove tool view
        void moveToolView(KToolView *view, Qt::DockWidgetArea newPlace);

        void addToPerspective(QWidget *widget, int perspective = DefaultPerspective);
        void removeFromPerspective(QWidget *widget);

        void addToPerspective(QAction *action, int perspective);
        void addToPerspective(const QList<QAction *> &actions, int perspective);
        void removeFromPerspective(QAction *action);
        void setCurrentPerspective(int wsp);
        int currentPerspective() const;
        void enableToolViews(bool flag);

        void setAutoRestore(bool autoRestore);
        bool autoRestore() const;

        virtual QMenu *createPopupMenu();

        void setSettingsHandler(KMainWindowAbstractSettings *settings);
        void restoreGUI();
        void saveGUI();

        QHash<Qt::ToolBarArea, KButtonBar *> buttonBars() const;
        QHash<KButtonBar *, QList<KToolView*> > toolViews() const;

    private:
        Qt::DockWidgetArea toDockWidgetArea(Qt::ToolBarArea area);
        Qt::ToolBarArea toToolBarArea(Qt::DockWidgetArea area);

    public slots:
        void setEnableButtonBlending(bool enable);

    private slots:
        void relayoutViewButton(bool topLevel);
        void relayoutToolView();

    signals:
        void perspectiveChanged(int wps);

    protected:
        void addButtonBar(Qt::ToolBarArea area);

    protected:
        virtual void closeEvent(QCloseEvent *e);
        virtual void showEvent(QShowEvent *e);
        virtual bool event(QEvent *e);

    private:
        KToolView *m_forRelayout;

    private:
        QHash<Qt::ToolBarArea, KButtonBar *> m_buttonBars;
        QHash<KButtonBar *, QList<KToolView*> > m_toolViews;
        QHash<QWidget *, int> m_managedWidgets;
        QHash<QAction *, int> m_managedActions;

        int m_currentPerspective;

        KMainWindowAbstractSettings *m_settings;
        bool m_autoRestore;
};

#endif
