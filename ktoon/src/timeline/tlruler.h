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

#ifndef TLRULER_H
#define TLRULER_H

/*!
 * \file tlruler.h
 * \brief Include this file if you need the class TLRuler
 */

#include <qframe.h>
#include <qpainter.h>

class Ktoon;

/*!
 * \brief Class that handles the Timeline Ruler for arranging frames
 *
 * <b>Date of Creation: June 10 - 2004.</b>\n
 * This is a special ruler with tiny marks, a numerated scale from five by five, and an offset rectangle that may
 * be dragged.
 */
class TLRuler : public QFrame
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a TLRuler object.
     * \param parent The parent widget
     */
    TLRuler( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the TLRuler object.
     */
    ~TLRuler();

    /*!
     * \brief Gets the ruler offset
     *
     * \return The ruler offset
     */
    int getOffset();
    /*!
     * \brief Gets the ruler's current maximum offset
     *
     * \return The ruler maximum offset
     */
    int getMaxOffset();
    /*!
     * \brief Gets the animation play state
     *
     * \return <b>true</b> if the animation is being played, otherwise returns <b>false</b>
     */
    bool isAnimationPlaying();

private:
    QWidget *parent_widget;
    QColor offset_outline_color, offset_fill_color, offset_drag_outline_color, offset_drag_fill_color;
    QRect offset_rectangle, offset_drag_rectangle;
    int offset;
    int max_offset;
    bool dragging;
    bool playing;
    Ktoon *k_toon;
    QTimer *animation_timer;

signals:
    /*!
     * This signal is emitted when the ruler offset has changed.
     * \param offset The new ruler offset
     */
    void offsetChanged( int offset );
    /*!
     * This signal is emitted when the ruler offset has been dragged.
     * \param offset The new ruler offset
     */
    void offsetDragged( int offset );
    /*!
     * This signal is emitted when the ruler maximum offset has changed.
     * \param max The new ruler maximum offset
     */
    void maxOffsetChanged( int max );

public slots:
    /*!
     * \brief Updates the max offset value so the offset may be dragged until that position
     *
     * \param value The new max offset value
     */
    void slotUpdateMaxOffset( int value );
    /*!
     * \brief Sets a new offset to this ruler
     *
     * \param offset The new offset
     */
    void slotSetOffset( int offset );
    /*!
     * \brief Plays the animation from the current ruler offset
     */
    void slotPlay();
    /*!
     * \brief Stops the animation that is currently being played
     */
    void slotStop();
    /*!
     * \brief Stops the animation that is currently being played when it reaches the final frame
     */
    void slotStopWhenReachEnd();
    /*!
     * \brief Steps one frame forward from the current ruler offset
     */
    void slotStepForward();
    /*!
     * \brief Steps one frame backward from the current ruler offset
     */
    void slotStepBackward();

protected:
    /*!
     * \brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget mouse move control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mouseMoveEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Event for widget mouse release control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mouseReleaseEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Draw the contents of the TLRuler
     *
     * Reimplemented from QFrame.
     * \param painter The frame painter
     */
    void drawContents( QPainter *painter );
    /*!
     * \brief Draws a red rectangle over the offset position
     *
     * \param x The offset position
     */
    void drawOffsetRectangle( int x );
    /*!
     * \brief Draws a black rectangle over the offset position
     *
     * \param x The offset position
     */
    void drawOffsetDragRectangle( int x );

};

#endif
