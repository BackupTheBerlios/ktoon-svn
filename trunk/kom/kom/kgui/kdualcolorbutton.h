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

#ifndef KDUALCOLORBUTTON_H
#define KDUALCOLORBUTTON_H

class QBitmap;

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QPixmap>
#include <QPaintEvent>
#include <QDropEvent>
#include <QBrush>
#include <QWidget>
#include <kcore/kglobal.h>

/**
 * @note This class was adapted to KToon by David Cuadrado <krawek@toonka.com>
 * 
 * @short A widget for selecting two related colors.
 *
 * KDualColorButton allows the user to select two cascaded colors (usually a
 * foreground and background color). Other features include drag and drop
 * from other KDE color widgets, a reset to black and white control, and a
 * swap colors control.
 *
 * When the user clicks on the foreground or background rectangle the
 * rectangle is first sunken and the currentChanged() signal is emitted.
 * Further clicks will present a color dialog and emit either the fgChanged()
 * or bgChanged() if a new color is selected.
 *
 * Note: With drag and drop when dropping a color the current selected color
 * will be set, while when dragging a color it will use whatever color
 * rectangle the mouse was pressed inside.
 *
 * @author Daniel M. Duley <mosfet@kde.org>
 */
class K_GUI_EXPORT KDualColorButton : public QWidget
{
    Q_OBJECT
    Q_ENUMS(DualColor)
    Q_PROPERTY(QBrush foreground READ foreground WRITE setForeground)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QBrush currentColor READ currentColor WRITE setCurrentColor STORED false DESIGNABLE false)
    Q_PROPERTY(DualColor current READ current WRITE setCurrent)

    public:

        enum DualColor {Foreground, Background};
        /**
        * Constructs a new DDualColorButton using the default black and white
        * colors.
        *
        */

        /// KDE4 remove name argument
        KDualColorButton(QWidget *parent=0);

        /**
        * Constructs a new DDualColorButton with the supplied foreground and
        * background colors.
        */
        /// KDE4 remove name argument
        KDualColorButton(const QBrush &fgColor, const QBrush &bgColor,
                           QWidget *parent=0);
        ~KDualColorButton();

        /**
        * Returns the current foreground color.
        */
        QBrush foreground() const;

        /**
        * Returns the current background color.
        */
        QBrush background() const;

        /**
        * Returns the current color item selected by the user.
        */
        DualColor current() const;

        /**
        * Returns the color of the selected item.
        */
        QBrush currentColor() const;

        /**
        * Returns the minimum size needed to display the widget and all its
        * controls.
        */
        virtual QSize sizeHint() const;

    public slots:
        /**
         * Sets the foreground color.
         */
        void setForeground(const QBrush &c);

        /**
         * Sets the background color.
         */
        void setBackground(const QBrush &c);

        /**
         * Sets the current selected color item.
         */
        void setCurrent(DualColor s);

        /**
         * Sets the color of the selected item.
         */
        void setCurrentColor(const QBrush &c);

    signals:
        /**
         * Emitted when the foreground color is changed.
         */
        void fgChanged(const QBrush &c);

        /**
         * Emitted when the background color is changed.
         */
        void bgChanged(const QBrush &c);

        /**
         * Emitted when the user changes the current color selection.
         */
        void currentChanged(KDualColorButton::DualColor s);

        // void selectionChanged();

    protected:
        /**
         * Sets the supplied rectangles to the proper size and position for the
         * current widget size. You can reimplement this to change the layout
         * of the widget. Restrictions are that the swap control will always
         * be at the top right, the reset control will always be at the bottom
         * left, and you must leave at least a 14x14 space in those corners.
         */
        virtual void metrics(QRect &fgRect, QRect &bgRect);
        virtual void paintEvent(QPaintEvent *ev);
        virtual void mousePressEvent(QMouseEvent *ev);
        virtual void mouseMoveEvent(QMouseEvent* ev);
        // Dnd
        virtual void dragEnterEvent(QDragEnterEvent *ev);
        virtual void dropEvent(QDropEvent *ev);

    private:
        QPixmap *arrowBitmap;
        QPixmap *resetPixmap;
        QBrush fg, bg;
        QPoint mPos;
        bool dragFlag, miniCtlFlag;
        DualColor curColor, tmpColor;
};

#endif
