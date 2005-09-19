/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
 *                                                                         *
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

#ifndef ABOUT_H
#define ABOUT_H

/**
 * @file about.h
 * @brief Include this file if you need the class About
 */

#include <qtabdialog.h>
#include <qscrollview.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qpainter.h>

/**
 * @brief Class that handles the about dialog box
 *
 * <b>Date of Creation: July 28 - 2004.</b>\n
 * This is a dialog box that contains info about the authors of the application,
 * the application itself and another related stuff. It's a subclass of QTabDialog,
 * and provides 6 Tabs:
 * - Copyrights
 * - Credits
 * - Acknowledgements
 * - SVN Log
 * - About Toonka Films
 * - GNU Public License
 *
 * Moreover, it provides an OK Button and a Cancel Button.
 */
class About : public QTabDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an About Dialog.
     * @param parent The parent widget
     */
    About( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the About Dialog.
     */
    ~About();

private:
    QWidget *parent_widget;

    //Widgets for the tabs:
    //1:
    QLabel *credits;
    QPainter *credits_painter;
    QPixmap credits_pixmap;
    QString credits_text;
    int file_id;
    QTimer *credits_timer;
    //2:
    QLabel *ack;
    QPixmap ack_pixmap;
    int file_id2;
    QTimer *ack_timer;
    //3:
    QScrollView *scroll_container_log;
    QFrame *scroll_log;
    QLabel *text_log;
    //4:
    QLabel *toonka;
    QPixmap toonka_pixmap;
    //5:
    QLabel *laboratoon;
    QPixmap laboratoon_pixmap;
    //6:
    QScrollView *scroll_container_gnu;
    QFrame *scroll_gnu;
    QLabel *text_gnu;

public slots:
    /**
     * @brief Starts the credits animation
     */
    void slotDisplayCreditsPixmap();
    /**
     * @brief Starts the acknowledgements animation
     */
    void slotDisplayAcknowledgementsPixmap();
    /**
     * @brief Slot that performs some actions when a page is selected
     *
     * @param page_widget The widget associated with the current page
     */
    void slotPerformPageChangeActions( QWidget *page_widget );

protected:

};

#endif
