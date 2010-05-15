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

#ifndef KCOLLAPSIBLEWIDGET_H
#define KCOLLAPSIBLEWIDGET_H

#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include "kcore/kglobal.h"

class QScrollArea;
class QTextDocument;

class K_GUI_EXPORT KClickableLabel : public QWidget
{
    Q_OBJECT
    public:
        KClickableLabel(QWidget* parent = 0);
        ~KClickableLabel();
        
        void setText(const QString &text);
        QString text() const;
        
        void setChecked(bool c);
        bool isChecked() const;
        QSize sizeHint() const;
        
    protected:
        void paintEvent(QPaintEvent *e);
        void enterEvent(QEvent * e);
        void leaveEvent(QEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent* e);
        
    signals:
        void clicked();
        
    private:
        bool m_isEnter;
        QPoint m_position;
        QTextDocument *m_text;
        bool m_isDragging;
        bool m_checked;
};

/**
  @short A widget that has a caption and a collapsible widget
  @author Daniel Molkentin <molkentin@kde.org>
 */
class K_GUI_EXPORT KCollapsibleWidget : public QWidget
{
    Q_OBJECT
    public:
        KCollapsibleWidget(QWidget *parent = 0);
        KCollapsibleWidget(const QString& caption, QWidget *parent = 0);
        ~KCollapsibleWidget();
    
        QString caption() const;
        bool isExpanded() const;

        QWidget* innerWidget() const;
        void setInnerWidget(QWidget *w);

    public slots:
        void setExpanded(bool collapsed);
        void setCaption(const QString& caption);
        
    private slots:
        void toggleExpanded();
        
    protected:
        void init();

    private:
        Q_DISABLE_COPY(KCollapsibleWidget);
        class Private;
        Private *k;
};

/**
  @short A scrollable container that contains groups of settings,
         usually in the form of KCollapsibleWidgets.
  @author Daniel Molkentin <molkentin@kde.org>
 */
class K_GUI_EXPORT KSettingsContainer : public QScrollArea
{
    Q_ENUMS(CollapseState);
    Q_OBJECT;
    public:
        enum CollapseState { Collapsed, Uncollapsed };
        KSettingsContainer(QWidget *parent = 0);
        ~KSettingsContainer();

        KCollapsibleWidget* insertWidget(QWidget* w, const QString& name);
        void removeWidget(QWidget *w);
        
    protected:
        void dragEnterEvent(QDragEnterEvent * event);
        void dragMoveEvent(QDragMoveEvent* event);
        void dropEvent(QDropEvent* e);

    private:
        Q_DISABLE_COPY(KSettingsContainer);
        class Private;
        Private *k;
};

#endif // KCOLLAPSIBLEWIDGET_H
