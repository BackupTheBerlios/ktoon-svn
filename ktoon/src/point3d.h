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

#ifndef POINT3D_H
#define POINT3D_H

/*!
 * \file point3d.h
 * \brief Include this file if you need the class Point3D
 */

#include <qdom.h>

/*!
 * \brief Class that encapsulates the concept of a 3D point
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Point3D storing scheme. It provides the point <x,y,z> components.
 */
class Point3D
{

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a Point3D object.
     */
    Point3D();
    /*!
     * \brief Parametric Constructor
     *
     * Constructs a Point3D object with the parameters specified.
     * \param x The point's x axis component
     * \param y The point's y axis component
     * \param z The point's z axis component
     */
    Point3D( int x, int y , int z );
    /*!
     * \brief Default Destructor
     *
     * Destroys the Point3D object.
     */
    ~Point3D();

    /*!
     * \brief Sets a new x component to this point
     *
     * \param x The x component
     * \sa x()
     */
    void setX( const int &x );
    /*!
     * \brief Sets a new y component to this point
     *
     * \param y The y component
     * \sa y()
     */
    void setY( const int &y );
    /*!
     * \brief Sets a new z component to this point
     *
     * \param z The z component
     * \sa z()
     */
    void setZ( const int &z );

    /*!
     * \brief Gets this point's x component
     *
     * \return The current x component
     * \sa setX()
     */
    int x() const;
    /*!
     * \brief Gets this point's y component
     *
     * \return The current y component
     * \sa setY()
     */
    int y() const;
    /*!
     * \brief Gets this point's z component
     *
     * \return The current z component
     * \sa setZ()
     */
    int z() const;

    /*!
     * \brief Creates an XML tag for this object
     *
     * \param doc The DOM Document
     * \return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    int pos_x, pos_y, pos_z;

};

#endif
