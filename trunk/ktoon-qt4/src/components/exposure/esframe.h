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

#ifndef ESFRAME_H
#define ESFRAME_H

/**
 * @file esframe.h
 * @brief Include this file if you need the class ESFrame
 */

#include <QLabel>
#include <QLineEdit>
#include <QColor>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QKeyEvent>

#include "dsqueezelabel.h"

/**
 * @brief Class that handles the Frame objects belonging to the Exposure Sheet dialog box
 *
 * <b>Date of Creation: June 7 - 2004.</b>\n
 * This is a widget that represents the exposure sheet keyframes
 */
class ESFrame : public DSqueezeLabel
{
    Q_OBJECT
	public:
		/**
		* @brief Default Constructor
		*
		* Constructs an ESFrame object.
		* @param initial_text The text that this object will display
		* @param parent The parent widget
		*/
		ESFrame( int id, QWidget *parent);
		/**
		* @brief Default Destructor
		*
		* Destroys the ColorCells object.
		*/
		~ESFrame();
	
		/**
		* @brief Returns the use state of the ESFrame
		*
		* A frame is used when it is possible to make a drawing into the drawing area and associate it with the frame.
		* It is distinguished from an unused frame by its darker color.
		* @return <b>true</b> if the ESFrame is used, otherwise returns <b>false</b>
		* @sa setUsed()
		*/
		bool isUsed();
		/**
		* @brief Returns the select state of the ESFrame
		*
		* @return <b>true</b> if the ESFrame is selected, otherwise returns <b>false</b>
		* @sa setSelected()
		*/
		bool isSelected();
		/**
		* @brief Returns the lock state of the ESFrame
		*
		* When a frame is locked it is not possible to make drawings into the associated drawing area.
		* @return <b>true</b> if the ESFrame is lock, otherwise returns <b>false</b>
		* @sa setLocked()
		*/
		bool isLocked();
		/**
		* @brief Returns the motion state of the ESFrame
		*
		* This property shows when the ESFrame begins a motion tween.
		* @return <b>true</b> if the ESFrame is a motion frame, otherwise returns <b>false</b>
		* @sa setMotion()
		*/
		bool isMotion();
		/**
		* @brief Returns the "has drawing" state of the ESFrame
		*
		* This property shows when the ESFrame has graphic components inside.
		* @return <b>true</b> if the ESFrame is has a drawing, otherwise returns <b>false</b>
		* @sa setHasDrawing()
		*/
		bool hasDrawing();
		/**
		* @brief Sets the use state of the ESFrame
		*
		* @param in_is_used The boolean value of the use state
		* @sa isUsed()
		*/
		void setUsed( bool in_is_used );
		/**
		* @brief Sets the select state of the ESFrame
		*
		* @param in_is_selected The boolean value of the select state
		* @sa isSelected()
		*/
		void setSelected( bool in_is_selected );
		/**
		* @brief Sets the lock state of the ESFrame
		*
		* It is only possible to lock used frames.
		* @param in_is_locked The boolean value of the lock state
		* @sa isLocked()
		*/
		void setLocked( bool in_is_locked );
		/**
		* @brief Sets the motion state of the ESFrame
		*
		* @param in_is_motion The boolean value of the motion state
		* @sa isMotion()
		*/
		void setMotion( bool in_is_motion );
		/**
		* @brief Sets the "has drawing" state of the ESFrame
		*
		* @param in_has_drawing The boolean value of the "has drawing" state
		* @sa hasDrawing()
		*/
		void setHasDrawing( bool in_has_drawing );
		/**
		* @brief Sets a new name for this ESFrame and displays it
		*
		* @param new_name The new name
		*/
		void setName( const QString &new_name );
		/**
		* @brief Clears the textfield focus on the frame
		*
		* This is a convenience function that clears explicitly the focus of the textfield "description"
		* in order to emit the signal lostFocus(). You won't need to use it.
		*/
		void clearTextfieldFocus();
		/**
		* @brief Sets the properties of this frame to be the argument properties
		*
		* The properties set are \e used, \e selected, \e locked and \e motion. It leaves this frame's name
		* and other properties as they are.
		* @param in_esframe The argument frame
		*/
		void setAllProperties( ESFrame *in_esframe );
		void setId(int id);
		int id();
		QString name();
	
	private:
	
		bool is_used;
		bool is_selected;
		bool is_locked;
		bool is_motion;
		bool has_drawing;
		int m_id;
		QString m_initialText;
		QLineEdit *description;
	
	signals:
		/**
		* This signal is emitted when the ESFrame is selected.
		*/
		void selected();
		
		
		void clicked(int , int, int, int );
		
		/**
		* This signal is emitted when the ESFrame is renamed
		*
		* @param name The new frame name
		*/
		void renamed( int id, const QString &name );
	
	public slots:
		/**
		* @brief Sets the textfield text that appears when someone double-clicks the frame, to this frame's description.
		*
		* You should never use it from outside.
		*/
		void slotSetDescription();
		/**
		* @brief Sends the double click event in order to the right click menu may call the rename routine
		*
		* You should never use it from ouside.
		*/
		void slotSendDoubleClickEvent();
			
		void otherSelected(int id);
	protected:
		/**
		* @brief Event for widget mouse press control
		*
		* Reimplemented from QWidget.
		* @param mouse_event The input event
		*/
		void mousePressEvent( QMouseEvent *mouse_event );
		/**
		* @brief Event for widget mouse double click control
		*
		* Reimplemented from QWidget.
		* @param mouse_event The input event
		*/
		void mouseDoubleClickEvent( QMouseEvent *mouse_event );
		/**
		* @brief Draw this widget contents using the painter \a painter
		*
		* Reimplemented from QLabel.
		* @param painter The painter
		*/
		void drawContents( QPainter *painter );
		void resizeEvent ( QResizeEvent * e );
		
		void keyPressEvent(QKeyEvent *e);
};

#endif
