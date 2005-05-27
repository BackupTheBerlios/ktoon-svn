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

#ifndef SOUNDLAYER_H
#define SOUNDLAYER_H

/**
 * @file soundlayer.h
 * @brief Include this file if you need the class SoundLayer
 */

#include <qptrlist.h>

#include "layer.h"
#include "soundclip.h"

/**
 * @brief Class that encapsulates the concept of Sound Layer
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the Sound Layer storing scheme. It provides a list of sound clips.
 */
class SoundLayer : public Layer
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a SoundLayer object.
     */
    SoundLayer();
    /**
     * @brief Default Destructor
     *
     * Destroys the SoundLayer object.
     */
    ~SoundLayer();

private:
    QPtrList<SoundClip> sound_clips;

};

#endif
