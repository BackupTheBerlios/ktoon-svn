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

#ifndef TLFRAME_H
#define TLFRAME_H

/**
 * @file tlframe.h
 * @brief Include this file if you need the class TLFrame
 */

#include <qframe.h>
#include <qpainter.h>
#include <qpopupmenu.h>

/**
 * @brief Class that handles the timeline frames
 *
 * <b>Date of Creation: June 22 - 2004.</b>\n
 * This is a widget that represents the timeline frames, as they are used in Flash.
 */
class TLFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a TLFrame object.
     * @param parent The parent widget
     */
     TLFrame( QWidget *parent ); // DEPRECATED
	      
	      
    TLFrame( int ID, QWidget *parent );
    /**
     * @brief Alternative Constructor
     *
     * Constructs a TLFrame object according to the argument's properties.
     * @param in_frame The frame which properties will be set to this frame
     */
    TLFrame( TLFrame *in_frame );
    /**
     * @brief Default Destructor
     *
     * Destroys the TLFrame object.
     */
    ~TLFrame();

    /**
     * @brief Returns the special state of the TLFrame
     *
     * A special frame is an unused frame that appears in a frame sequence each 5 frames. It is distinguished
     * from the other because of its darker color.
     * @return <b>true</b> if the TLFrame is special, otherwise returns <b>false</b>
     * @sa setSpecial()
     */
    bool isSpecial();
    /**
     * @brief Returns the select state of the TLFrame
     *
     * @return <b>true</b> if the TLFrame is selected, otherwise returns <b>false</b>
     * @sa setSelected()
     */
    bool isSelected();
    /**
     * @brief Returns the use state of the TLFrame
     *
     * The used frames are the frames that appears when the user presses the key F5. They are distinguished
     * from an unused frame because all unused frames have a gray border. A keyframe is also an used frame.
     * @return <b>true</b> if the TLFrame is used, otherwise returns <b>false</b>
     * @sa setUsed()
     */
    bool isUsed();
    /**
     * @brief Returns the last state of the TLFrame
     *
     * A last frame ends a group of frames belonging a keyframe. They are distinguished because they have a
     * rectangle drawing.
     * @return <b>true</b> if the TLFrame is last, otherwise returns <b>false</b>
     * @sa setLast()
     */
    bool isLast();
    /**
     * @brief Returns the key state of the TLFrame
     *
     * A keyframe is a frame that has a main drawing that is going to be animated. They are created in the
     * exposure sheet and they are distinguished by its circle drawing.
     * @return <b>true</b> if the TLFrame is key, otherwise returns <b>false</b>
     * @sa setKey()
     */
    bool isKey();
    /**
     * @brief Returns the offset state of the TLFrame
     *
     * An offset frame is a frame that is currently displayed in the render preview. There is only one offset frame
     * for each frame sequence. Their feature is that they have a vertical red line.
     * @return <b>true</b> if the TLFrame is offset, otherwise returns <b>false</b>
     * @sa setOffset()
     */
    bool isOffset();
    /**
     * @brief Returns the offset state of the TLFrame
     *
     * A drag offset frame is also an offset frame but with a vertical black line instead of a red. They indicates
     * that a ruler offset drag operation is in course.
     * @return <b>true</b> if the TLFrame is drag offset, otherwise returns <b>false</b>
     * @sa setDragOffset()
     */
    bool isDragOffset();
    /**
     * @brief Returns the unknown motion state of the TLFrame
     *
     * This property assures when the frame takes place within a motion tween but the involved symbols are unknown.
     * @return <b>true</b> if the TLFrame is unknown motion, otherwise returns <b>false</b>
     * @sa setUnknownMotion()
     */
    bool isUnknownMotion();
    /**
     * @brief Returns the motion state of the TLFrame
     *
     * This property assures when the frame takes place within a motion tween and the involved symbols are known.
     * @return <b>true</b> if the TLFrame is motion, otherwise returns <b>false</b>
     * @sa setMotion()
     */
    bool isMotion();
    /**
     * @brief Returns the drawing state of the TLFrame
     *
     * @return <b>true</b> if the TLFrame has a drawing, otherwise returns <b>false</b>
     * @sa setHasDrawing()
     */
    bool hasDrawing();
    /**
     * @brief Sets the special state of the TLFrame
     *
     * @param in_is_special The boolean value of the special state
     * @sa isSpecial()
     */
    void setSpecial( bool in_is_special );
    /**
     * @brief Sets the select state of the TLFrame
     *
     * @param in_is_selected The boolean value of the select state
     * @sa isSelected()
     */
    void setSelected( bool in_is_selected );
    /**
     * @brief Sets the use state of the TLFrame
     *
     * @param in_is_used The boolean value of the use state
     * @sa isUsed()
     */
    void setUsed( bool in_is_used );
    /**
     * @brief Sets the last state of the TLFrame
     *
     * @param in_is_last The boolean value of the last state
     * @sa isLast()
     */
    void setLast( bool in_is_last );
    /**
     * @brief Sets the key state of the TLFrame
     *
     * @param in_is_key The boolean value of the key state
     * @sa isKey()
     */
    void setKey( bool in_is_key );
    /**
     * @brief Sets the offset state of the TLFrame
     *
     * @param in_is_offset The boolean value of the offset state
     * @sa isOffset()
     */
    void setOffset( bool in_is_offset );
    /**
     * @brief Sets the drag offset state of the TLFrame
     *
     * @param in_is_drag_offset The boolean value of the drag offset state
     * @sa isDragOffset()
     */
    void setDragOffset( bool in_is_drag_offset );
    /**
     * @brief Sets the unknown motion state of the TLFrame
     *
     * @param in_is_unknown_motion The boolean value of the unknown motion state
     * @sa isUnknownMotion()
     */
    void setUnknownMotion( bool in_is_unknown_motion );
    /**
     * @brief Sets the motion state of the TLFrame
     *
     * @param in_is_motion The boolean value of the motion state
     * @sa isMotion()
     */
    void setMotion( bool in_is_motion );
    /**
     * @brief Sets whether this TLFrame has a drawing or not
     *
     * @param in_has_drawing The boolean value of the drawing state
     * @sa hasDrawing()
     */
    void setHasDrawing( bool in_has_drawing );
    /**
     * @brief Sets all properties to this frame to be the argument's properties
     *
     * @param in_frame The frame which properties are going to be copied
     */
    void setUseProperties( TLFrame *in_frame );

private:
    QPainter p;
    QPopupMenu *right_click_menu;
    bool is_special;
    bool is_selected;
    bool is_used;
    bool is_last;
    bool is_key;
    bool is_offset;
    bool is_drag_offset;
    bool is_unknown_motion;
    bool is_motion;
    bool has_drawing;
    int m_ID;

signals:
    /**
     * This signal is emitted when the TLFrame is selected.
     */
	void selected();// DEPRECATED
    
    //FIXME
    void selected(int);

public slots:

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget painting control
     *
     * Reimplemented from QWidget.
     * @param paint_event The input event
     */
    void paintEvent( QPaintEvent *paint_event );
    /**
     * @brief Perfroms a paint when the frame is not selected
     */
    void paintNotSelected();
    /**
     * @brief Perfroms a paint when the frame is selected
     */
    void paintSelected();

};

#endif
