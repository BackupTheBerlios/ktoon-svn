/***************************************************************************
 *   Copyright (C) 2004 by Fernando José Roldán Correa                     *
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

#ifndef GLIMAGE_H
#define GLIMAGE_H

/*!
 * \file glimage.h
 * \brief Include this file if you need the class GLImage
 */

#include "glgraphiccomponent.h"

/*!
 * \brief Class that represents the drawing area's image object
 *
 * <b>Date of Creation: December 21 - 2004.</b>\n
 * This class contains relevant information about an image imported from outside, processing it with the OpenGL textures feature.
 * It implements the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLImage : public GLGraphicComponent
{

public:
    /*!
     * \brief Default constructor
     *
     * Constructs a GLImage object
     * \param parent The parent widget
     * \param file_name The image's file name
     * \param origin The origin point
     * \param end The end point
     */
    GLImage( QGLWidget *parent, const QString &file_name, const QPoint &origin, const QPoint &end );
    /*!
     * \brief Default destructor
     *
     * Destroys the GLImage object
     */
    ~GLImage();

    //Set methods
    /*!
     * \brief Sets a new end point to this image
     *
     * \param end The new end point
     * \sa endImage()
     */
    void setEndImage( const QPoint &end );
    /*!
     * \brief Calculates the top left point of the rectangle that fits outside this image
     */
    void calculateTopLeft();
    /*!
     * \brief Calculates the bottom right point of the rectangle that fits outside this image
     */
    void calculateBottomRight();

    //Get methods
    /*!
     * \brief Gets this image's end point
     *
     * \return The current end point
     * \sa setEndImage()
     */
    QPoint endImage() const;
    /*!
     * \brief Gets the clipboard representation of this image
     *
     * \return A string that represents this image's properties
     */
    QString clipboardGraphic();

    /*!
     * \brief Creates an XML tag for this object
     *
     * \param doc The DOM Document
     * \return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

protected:
    QString file_name;
    QPoint end;
    GLuint id_texture;

    /*!
     * \brief Constructs an openGL display list with instructions for drawing this image
     */
    void buildList();
    /*!
     * \brief Load the Texture to select the current graphic in selection mode
     *
     * \param filename The path of the texture file
     * \return The texture's display list identifier
     */
    GLuint loadTexture( const QString & filename );

};

#endif
