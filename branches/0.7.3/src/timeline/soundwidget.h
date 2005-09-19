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

#ifndef SOUNDWIDGET_H
#define SOUNDWIDGET_H

/**
 * @file soundwidget.h
 * @brief Include this file if you need the class SoundWidget
 */

#include <qgridview.h>

class QColor;
class QPainter;

/**
 * @brief Class that handles the timeline's sound widget
 *
 * <b>Date of Creation: October 13 - 2004.</b>\n
 * This is a widget that shows a sound file preview with its waveform, in order to use it as a guide when animating.
 */
class SoundWidget: public QGridView
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a SoundWidget object.
     * @param parent The parent widget
     */
    SoundWidget( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the SoundWidget object.
     */
    ~SoundWidget();

private:
    QWidget *parent_widget;
    QColor default_color, border_color, special_color, waveform_color, offset_color, drag_offset_color;
    int offset_cell, drag_offset_cell;

signals:

public slots:
    /**
     * @brief Moves this widget according to the scroll bar value
     *
     * @param value The scroll bar value
     */
    void slotMoveSoundWidget( int value );
    /**
     * @brief Sets the offset of the sound widget to be the current offset
     *
     * @param current_offset The current offset
     */
    void slotChangeOffset( int current_offset );
    /**
     * @brief Sets the drag offset of the sound widget to be the current offset
     *
     * @param current_offset The current drag offset
     */
    void slotChangeDragOffset( int current_offset );

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Paints a single cell at (\a row, \a col) using painter \a painter.
     *
     * Reimplemented from QGridView.
     * @param painter The painter
     * @param row The cell row
     * @param col The cell col
     */
    void paintCell( QPainter *painter, int row, int col );

};

#endif
