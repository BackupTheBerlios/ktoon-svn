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

#ifndef KEYFRAME_H
#define KEYFRAME_H

/**
 * @file keyframe.h
 * @brief Include this file if you need the class KeyFrame
 */

#include <QString>
// #include <QList>
#include <qdom.h>

#include "camera.h"
#include "gldrawing.h"

/**
 * @brief Class that encapsulates the concept of a keyframe
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Keyframe storing scheme. It provides a list of Drawings.
 */
class KeyFrame : public QObject
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a KeyFrame object.
     */
    KeyFrame();
    /**
     * @brief Default Destructor
     *
     * Destroys the KeyFrame object.
     */
    ~KeyFrame();

    /**
     * @brief Sets a new offset to this keyframe
     *
     * The offset is the absolute position in time that corresponds to this keyframe.
     * @param offset The new offset
     * @sa offsetKeyFrame()
     */
    void setOffsetKeyFrame( const int &offset );
    /**
     * @brief Sets a new length to this keyframe
     *
     * The length is the number of frames that corresponds to this keyframe.
     * @param length The new length
     * @sa lengthKeyFrame()
     */
    void setLengthKeyFrame( const int &length );
    /**
     * @brief Sets the motion property of this keyframe
     *
     * @param motion The boolean value of the motion property
     * @sa motionKeyFrame()
     */
    void setMotionKeyFrame( bool motion );
    /**
     * @brief Sets a new name to this keyframe
     *
     * @param name The new name
     * @sa nameKeyFrame()
     */
    void setNameKeyFrame( const QString &name );
    /**
     * @brief Sets a new Drawing to this keyframe
     *
     * @param drawing The new Drawing
     * @sa getDrawing()
     */
    void setDrawing( GLDrawing *drawing );
    /**
     * @brief Sets a new Camera to this keyframe
     *
     * @param camera The new Camera
     * @sa getCamera()
     */
    void setCamera( Camera *camera );

    /**
     * @brief Gets this keyframe's offset
     *
     * The offset is the absolute position in time that corresponds to this keyframe.
     * @return The current offset
     * @sa setOffsetKeyframe()
     */
    int offsetKeyFrame() const;
    /**
     * @brief Gets this keyframe's length
     *
     * The length is the number of frames that corresponds to this keyframe.
     * @return The current length
     * @sa setLengthKeyframe()
     */
    int lengthKeyFrame() const;
    /**
     * @brief Gets this keyframe's motion property
     *
     * @return The current boolean value that describes the motion property
     * @sa setMotionKeyframe()
     */
    bool motionKeyFrame() const;
    /**
     * @brief Gets this keyframe's name
     *
     * @return The current name
     * @sa setNameKeyframe()
     */
    QString nameKeyFrame() const;
    /**
     * @brief Gets this keyframe's Drawing
     *
     * @return The current Drawing
     * @sa setDrawing()
     */
    GLDrawing *getDrawing() const;
    /**
     * @brief Gets this keyframe's Camera
     *
     * @return The current Camera
     * @sa setCamera()
     */
    Camera *getCamera() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    int m_offset;
    int m_length;
    bool m_motion;
    QString m_name;
    Camera *m_camera;
    GLDrawing *m_drawing;
};

#endif
