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

#ifndef CAMERA_H
#define CAMERA_H

/**
 * @file camera.h
 * @brief Include this file if you need the class Camera
 */

#include <qdom.h>

#include "point3d.h"

/**
 * @brief Class that encapsulates the concept of camera
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the camera storing scheme. It is used to handle the side, top and render
 * camera and to animate in dimension 2.5.
 */
class Camera
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Camera object.
     */
    Camera();
    /**
     * @brief Default Destructor
     *
     * Destroys the Camera object.
     */
    ~Camera();

    //Set methods
    /**
     * @brief Sets a new position for this camera
     *
     * @param position A point in the space that indicates the new position
     * @sa positionCamera()
     */
    void setPositionCamera( const Point3D & position );
    /**
     * @brief Sets a new center for this camera
     *
     * @param center A point in the space that indicates the new camera center
     * @sa centerCamera()
     */
    void setCenterCamera( const Point3D & center );
    /**
     * @brief Sets a new up direction for this camera
     *
     * @param up A point in the space that indicates the new camera up direction
     * @sa centerCamera()
     */
    void setUpCamera( const Point3D & up );

    //Get methods
    /**
     * @brief Gets the current camera position
     *
     * @return A point in the space with the current camera position
     * @sa setPositionCamera()
     */
    Point3D positionCamera( ) const;
    /**
     * @brief Gets the current camera center
     *
     * @return A point in the space with the current camera center
     * @sa setCenterCamera()
     */
    Point3D centerCamera( ) const;
    /**
     * @brief Gets the current camera up direction
     *
     * @return A point in the space with the current camera up direction
     * @sa setUpCamera()
     */
    Point3D upCamera( ) const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    Point3D position;
    Point3D center;
    Point3D up;

};

#endif
