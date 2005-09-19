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

#ifndef GLDRAWING_H
#define GLDRAWING_H

/**
 * @file gldrawing.h
 * @brief Include this file if you need the class GLDrawing
 */

#include <q3ptrlist.h>
#include <qdom.h>

#include "glgraphiccomponent.h"

/**
 * @brief Class that encapsulates the concept of a drawing
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the drawing storing scheme. It provides a list of graphic components.
 */
class GLDrawing : public QObject
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a GLDrawing object.
     */
    GLDrawing(QObject *parent = 0);
    /**
     * @brief Copy Constructor
     *
     * Constructs a GLDrawing object from another GLDrawing data.
     * @param to_copy The drawing to copy
     */
    GLDrawing( const GLDrawing &to_copy, QObject *parent = 0 );
    /**
     * @brief Default Destructor
     *
     * Destroys the GLDrawing object.
     */
    ~GLDrawing();

    //Set Methods
    /**
     * @brief Sets the graphic components to this drawing
     *
     * @param graphic_components The new list of graphic components
     */
    void setGraphicComponents( Q3PtrList<GLGraphicComponent> graphic_components );

    //Get Methods
    /**
     * @brief Gets this drawing's graphic components
     *
     * @return The list of graphic components
     */
    Q3PtrList<GLGraphicComponent> graphicComponents() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    Q3PtrList<GLGraphicComponent> graphic_components;

};

#endif
