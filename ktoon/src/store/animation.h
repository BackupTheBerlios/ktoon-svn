/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#ifndef ANIMATION_H
#define ANIMATION_H

/*!
 * \file animation.h
 * \brief Include this file if you need the class Animation
 */

#include <qptrlist.h>
#include <qdom.h>

#include "scene.h"

/*!
 * \brief Class that encapsulates the concept of animation
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the animation storing scheme. It provides a list of Scenes.
 */
class Animation
{
public:
    /*!
     * \brief Default Constructor
     *
     * Constructs an Animation object.
     */
    Animation();
    /*!
     * \brief Default Destructor
     *
     * Destroys the Animation object.
     */
    ~Animation();

    //Set methods
    /*!
     * \brief Sets a new frame rate
     *
     * The frame rate is the number of animation frames that may be played in one second
     * \param rate The frame rate
     * \sa frameRate()
     */
    void setFrameRate( const int & rate );
    /*!
     * \brief Sets a new camera width
     *
     * The camera width is ...
     * \param width The camera width
     * \sa cameraWidth()
     */
    void setCameraWidth( const int & width );
    /*!
     * \brief Sets a new camera length
     *
     * The camera length is ...
     * \param length The camera length
     * \sa cameraLength()
     */
    void setCameraLength( const int & length );
    /*!
     * \brief Sets the animation scenes
     *
     * \param scenes The Scene
     * \sa getScenes()
     */
    void setScenes( QPtrList<Scene> scenes );

    //Get methods
    /*!
     * \brief Gets the current frame rate
     *
     * The frame rate is the number of animation frames that may be played in one second
     * \return The current frame rate
     * \sa setFrameRate()
     */
    int frameRate() const;
    /*!
     * \brief Gets the current camera width
     *
     * The camera width is ...
     * \return The current camera width
     * \sa setCameraWidth()
     */
    int cameraWidth() const;
    /*!
     * \brief Gets the current camera length
     *
     * The camera length is ...
     * \return The current camera length
     * \sa setCameraLength()
     */
    int cameraLength() const;
    /*!
     * \brief Gets the animation Scenes
     *
     * \return The current list of Scenes
     * \sa setScenes()
     */
    QPtrList<Scene> getScenes() const;

    /*!
     * \brief Creates an XML tag for this object
     *
     * \param doc The DOM Document
     * \return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    int frame_rate;
    int camera_width, camera_length;
    QPtrList<Scene> scenes;

};

#endif

