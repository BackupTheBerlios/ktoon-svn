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

#ifndef SOUNDCLIP_H
#define SOUNDCLIP_H

/**
 * @file soundclip.h
 * @brief Include this file if you need the class SoundClip
 */

#include <qsound.h>
#include <qstring.h>

/**
 * @brief Class that encapsulates the concept of Sound Clip
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Sound Clip storing scheme.
 */
class SoundClip : QSound
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a SoundClip object.
     */
    SoundClip();
    /**
     * @brief Default Destructor
     *
     * Destroys the SoundClip object.
     */
    ~SoundClip();

    /**
     * @brief Sets a new name to this sound clip
     *
     * @param name The new name
     * @sa nameSoundClip()
     */
    void setNameSoundClip( const QString &name );
    /**
     * @brief Sets a new offset to this sound clip
     *
     * This offset concept is the same as it is handled in the class Keyframe.
     * @param offset The new offset
     * @sa offsetSoundClip()
     */
    void setOffsetSoundClip( const int &offset );

    /**
     * @brief Gets this sound clip's name
     *
     * @return The current name
     * @sa setNameSoundClip()
     */
    QString nameSoundClip() const;
    /**
     * @brief Gets this sound clip's offset
     *
     * This offset concept is the same as it is handled in the class Keyframe.
     * @return The current offset
     * @sa setOffsetSoundClip()
     */
    int offsetSoundClip() const;
    /**
     * @brief Gets this sound clip's id
     *
     * @return The sound clip identifier
     */
    int idSoundClip() const;

private:
    QString name;
    int offset;
    int length;
    int id_sound_clip;

};

#endif
