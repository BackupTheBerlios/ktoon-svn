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

#ifndef TLFRAMESEQUENCEMANAGER_H
#define TLFRAMESEQUENCEMANAGER_H

/**
 * @file tlframesequencemanager.h
 * @brief Include this file if you need the class TLFrameSequenceManager
 */

#include <qframe.h>
#include <qpixmap.h>
#include <qpushbutton.h>

#include "tlframesequencelayout.h"
#include "tlruler.h"

/**
 * @brief Class that handles the Timeline's Frame Sequence Manager
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * The frame sequence manager contains the frame sequence layout, a TLRuler and an horizontal scroll bar.
 */
class TLFrameSequenceManager : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a TLFrameSequenceManager object.
     * @param parent The parent widget
     * @param grandparent The grandparent widget. It is used for slot connections or function calls
     */
    TLFrameSequenceManager( QWidget *parent, QWidget *grandparent );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLFrameSequenceManager object.
     */
    ~TLFrameSequenceManager();

    /**
     * @brief Gets the frame sequence layout
     *
     * @return A pointer to the frame sequence layout
     */
    TLFrameSequenceLayout *frameLayout();
    /**
     * @brief Gets the frame sequence layout's vertical scroll bar
     *
     * @return A pointer to the frame sequence layout's vertical scroll bar
     */
    QScrollBar *verticalScrollBar();
    /**
     * @brief Gets the frame sequence manager's horizontal scroll bar
     *
     * @return A pointer to the frame sequence manager's horizontal scroll bar
     */
    QScrollBar *horizontalScrollBar();
    /**
     * @brief Gets the ruler object
     *
     * @return A pointer to the ruler object
     */
    TLRuler *getRuler();
    /**
     * @brief Gets the sound button
     *
     * @return A pointer to the sound button
     */
    QPushButton *soundButton();

private:
    //General purpose variables
    QWidget *parent_widget, *grandparent_widget;
    int max_value_frame;

    //A Pixmap
    QPixmap i_show_sound_widget;

    //An special ruler
    TLRuler *ruler;

    //Bottom Panel
    QFrame *utils;
    QScrollBar *horizontal_scroll_bar;
    QPushButton *show_sound_widget;

    //The frame layout!!
    TLFrameSequenceLayout *frame_layout;

signals:
    /**
     * This signal is emitted when a frame is selected. This is a bridge signal from TLFrameSequenceLayout.
     * @param frame_sequence The frame sequence that corresponds to the selected frame
     */
    void frameSelected( TLFrameSequence *frame_sequence );
    /**
     * This signal is emitted when a layer has been selected. It is connected to a slot from TLFrameSequenceLayout.
     * @param layer The selected layer
     */
    void layerSelected( TLLayer *layer );
    /**
     * This signal is emitted when a layer has been released above its original position after a drag.
     * @param pos1 The position the layer had first
     * @param pos2 The release position
     */
    void layerReleasedAbove( int pos1, int pos2 );
    /**
     * This signal is emitted when a layer has been released below its original position after a drag.
     * @param pos1 The position the layer had first
     * @param pos2 The release position
     */
    void layerReleasedBelow( int pos1, int pos2 );
    /**
     * This signal is emitted when this widget's width has changed
     * @param width The new width
     */
    void widthChanged( int width );

protected slots:
    /**
     * @brief Moves the ruler and frame sequences horizontally when the horizontal scroll bar is pressed
     */
    void slotMoveRulerAndFrames();
    /**
     * @brief Shows or hides the sound widget
     *
     * @param show The boolean value of the show state
     */
    void slotShowSoundWidget( bool show );

protected:
    /**
     * @brief Event for widget resizing control
     *
     * Reimplemented from QWidget.
     * @param resize_event The input event
     */
    void resizeEvent( QResizeEvent *resize_event );

};

#endif
