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

#ifndef SCENE_H
#define SCENE_H

/**
 * @file scene.h
 * @brief Include this file if you need the class Scene
 */

#include <qstring.h>
#include <qptrlist.h>
#include <qdom.h>

#include "layer.h"

/**
 * @brief Class that encapsulates the concept of Scene
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Scene storing scheme. It provides a list of Layers.
 */
class Scene
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Scene object.
     */
    Scene();
    /**
     * @brief Default Destructor
     *
     * Destroys the Scene object.
     */
    ~Scene();

    /**
     * @brief Sets a new name to this scene
     *
     * @param name The new name
     * @sa nameScene()
     */
    void setNameScene( const QString &name );
    /**
     * @brief Sets the Layers to this Scene
     *
     * @param layers The Layers
     * @sa getLayers()
     */
    void setLayers( QPtrList<Layer> layers );

    /**
     * @brief Gets this scene's name
     *
     * @return The current name
     * @sa setNameScene()
     */
    QString nameScene() const;
    /**
     * @brief Gets this scene's layers
     *
     * @return A list of layers
     */
    QPtrList<Layer> getLayers();

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    QString name;
    QPtrList<Layer> layers;

};

#endif
