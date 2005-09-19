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

#ifndef LAYER_H
#define LAYER_H

/**
 * @file layer.h
 * @brief Include this file if you need the class Layer
 */

#include <qstring.h>
#include <qptrlist.h>
#include <qdom.h>

#include "keyframe.h"

/**
 * @brief Class that encapsulates the concept of layer
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Layer storing scheme. It provides a list of KeyFrames.
 */
class Layer : public QObject
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Layer object.
     */
    Layer();
    /**
     * @brief Default Destructor
     *
     * Destroys the Layer object.
     */
    ~Layer();

    /**
     * @brief Sets a new index to this layer
     *
     * The index is the absolute position of this layer within a list of layers from a scene.
     * @param index The new index
     * @sa indexLayer()
     */
    void setIndexLayer( const int &index );
    /**
     * @brief Sets a new name to this layer
     *
     * @param name The new name
     * @sa nameLayer()
     */
    void setNameLayer( const QString &name );
    /**
     * @brief Sets the KeyFrames to this layer
     *
     * @param frames The KeyFrames
     * @sa keyFrames()
     */
    void setKeyFrames( QPtrList<KeyFrame> frames );

    /**
     * @brief Gets this layer's index
     *
     * The index is the absolute position of this layer within a list of layers from a scene.
     * @return The current index
     * @sa setIndexLayer()
     */
    int indexLayer() const;
    /**
     * @brief Gets this layer's name
     *
     * @return The current name
     * @sa setNameLayer()
     */
    QString nameLayer() const;
    /**
     * @brief Gets this layer's KeyFrames
     *
     * @return A list with the KeyFrames
     * @sa setKeyFrames()
     */
    QPtrList<KeyFrame> keyFrames() const;
    /**
     * @brief Gets the keyframe at an animation offset range
     *
     * @param start The offset at the beginning
     * @param end The offset at the end
     * @return The keyframe at [start-end]
     */
    KeyFrame *keyFrameAt( int start, int end );

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    int index;
    QString name;
    QPtrList<KeyFrame> keyframes;

};

#endif
