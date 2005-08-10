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

#ifndef ILAYER_H
#define ILAYER_H

/**
 * @file ilayer.h
 * @brief Include this file if you need the class ILayer
 */

#include <qptrlist.h>

#include "esframe.h"
#include "eslayer.h"

/**
 Geton* @brief Intermediate Class that is used for store some exposure sheet layer information
 *
 * <b>Date of Creation: June 7 - 2004.</b>\n
 * See the Public Members for an idea of what this class is done for.
 */
class ILayer : public QObject
{

public:
    //! Constants for defining the gradient type
    enum TranslationDirection
    {
        LEFT = 301, /**< The left direction */
        RIGHT = 302, /**< The right direction */
    };

    /**
     * @brief Default Constructor
     *
     * Constructs an ILayer object.
     * @param initial_frames The initial used frames
     * @param in_interface_element The corresponding ESLayer widget
     */
    ILayer( int initial_frames, ESLayer *in_interface_element );
    /**
     * @brief Default Destructor
     *
     * Destroys the ILayer object.
     */
    ~ILayer();

    /**
     * @brief Gets the number of used frames
     *
     * @return The number of used frames that the layer contains
     * @sa setNumberOfFrames()
     */
    int numberOfFrames();
    /**
     * @brief Gets the associated interface element
     *
     * @return A pointer to the corresponding ESLayer widget
     * @sa setInterfaceElement()
     */
    ESLayer *interfaceElement();
    /**
     * @brief Gets this layer's last used frame
     *
     * @return A pointer to the last used ESFrame
     * @sa setLastFrame()
     */
    ESFrame *lastFrame();
    /**
     * @brief Gets this layer's selected frame
     *
     * @return A pointer to the selected ESFrame. If there is not a selected ESFrame, returns NULL.
     * @sa setSelectedFrame()
     */
    ESFrame *selectedFrame();
    /**
     * @brief Gets all the frames belonging this layer including used and unused
     *
     * @param empty_list_to_be_filled A pointer to an empty list that will be filled with this layer's ESFrames.
     * @sa setAvailableFrames()
     */
    void availableFrames( QPtrList<ESFrame> *empty_list_to_be_filled );
    /**
     * @brief Adds an used frame to this layer
     *
     * @sa removeFrame()
     */
    void addFrame();
    /**
     * @brief Removes an used frame from this layer
     *
     * @sa addFrame()
     */
    void removeFrame();
    /**
     * @brief Sets the number of used frames to this layer
     *
     * @param in_number_of_frames The new number of used frames
     * @sa numberOfFrames()
     */
    void setNumberOfFrames( int in_number_of_frames );
    /**
     * @brief Sets the interface element associated with this layer
     *
     * @param in_interface_element The new associated ESLayer widget
     * @sa interfaceElement()
     */
    void setInterfaceElement( ESLayer *in_interface_element );
    /**
     * @brief Sets the last used frame to this layer
     *
     * @param in_last_frame The new last ESFrame
     * @sa lastFrame()
     */
    void setLastFrame( ESFrame *in_last_frame );
    /**
     * @brief Sets the selected frame to this layer
     *
     * @param in_selected_frame The new selected ESFrame
     * @sa selectedFrame()
     */
    void setSelectedFrame( ESFrame *in_selected_frame );
    /**
     * @brief Sets this layer's available frames
     *
     * @param in_available_frames A list that contains all the ESFrame widgets that will be associated with this layer.
     * @warning It makes a pointer copy, so you should take care of what you do with the argument list.
     * @sa availableFrames()
     */
    void setAvailableFrames( QPtrList<ESFrame> in_available_frames );
    /**
     * @brief Translates the layer one position to the direction specified
     *
     * @param direction The translation direction. See TranslationDirection.
     */
    void reacommodate( int direction );
    /**
     * @brief Destroys all the ESFrame widgets associated with this layer
     */
    void deleteAllFrames();
    /**
     * @brief Gets the position of the specified frame into this layer
     *
     * @param frame The frame
     * @return The argument frame's position
     */
    int framePosition( ESFrame *frame );
    /**
     * @brief Gets the frame at the specified position
     *
     * @param pos The frame position
     * @return The frame at the argument position
     */
    ESFrame *frameAt( int pos );

private:
    int number_of_frames;
    ESLayer *interface_element;
    ESFrame *last_frame;
    ESFrame *selected_frame;
    QPtrList<ESFrame> available_frames;

};

#endif
