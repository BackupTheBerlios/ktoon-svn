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

#include "soundclip.h"

SoundClip::SoundClip() : QSound( "" )
{

}
 
SoundClip::~SoundClip()
{

}

void SoundClip::setNameSoundClip( const QString & _name )
{
    name = _name;
}

void SoundClip::setOffsetSoundClip( const int & _offset )
{
    offset = _offset;
}

QString SoundClip::nameSoundClip() const
{
    return name;
}

int SoundClip::offsetSoundClip() const
{
    return offset;
}

int SoundClip::idSoundClip() const
{
    return id_sound_clip;
}
