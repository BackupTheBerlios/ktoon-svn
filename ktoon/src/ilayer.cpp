/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
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

#include "ilayer.h"

//--------------- CONSTRUCTOR --------------------

ILayer::ILayer( int initial_frames, ESLayer *in_interface_element )
{
    number_of_frames = initial_frames;
    interface_element = in_interface_element;
    last_frame = NULL;
    selected_frame = NULL;
}

//--------------- DESTRUCTOR --------------------

ILayer::~ILayer()
{
    deleteAllFrames();
}

//-------------- PUBLIC MEMBERS ----------------

int ILayer::numberOfFrames()
{
    return number_of_frames;
}

ESLayer *ILayer::interfaceElement()
{
    return interface_element;
}

ESFrame *ILayer::lastFrame()
{
    return last_frame;
}

ESFrame *ILayer::selectedFrame()
{
    return selected_frame;
}

void ILayer::availableFrames( QPtrList<ESFrame> *empty_list_to_be_filled )
{
    ESFrame *next_frame;

    empty_list_to_be_filled -> clear();

    for ( next_frame = available_frames.first(); next_frame; next_frame = available_frames.next() )
    {
        empty_list_to_be_filled -> append( next_frame );
    }
}

void ILayer::addFrame()
{
    number_of_frames++;
}

void ILayer::removeFrame()
{
    number_of_frames--;
}

void ILayer::setNumberOfFrames( int in_number_of_frames )
{
    number_of_frames = in_number_of_frames;
}

void ILayer::setInterfaceElement( ESLayer *in_interface_element )
{
    interface_element = in_interface_element;
}

void ILayer::setLastFrame( ESFrame *in_last_frame )
{
    last_frame = in_last_frame;
}

void ILayer::setSelectedFrame( ESFrame *in_selected_frame )
{
    selected_frame = in_selected_frame;
}

void ILayer::setAvailableFrames( QPtrList<ESFrame> in_available_frames )
{
    ESFrame *next_frame;

    for ( next_frame = in_available_frames.first(); next_frame; next_frame = in_available_frames.next() )
    {
        available_frames.append( next_frame );
    }
}

void ILayer::reacommodate( int direction )
{
    if ( direction == LEFT )
    {
    	interface_element -> move( interface_element -> x() - interface_element -> width(), interface_element -> y() );

    	//Move all the frames to the left
    	ESFrame *frame_iterator;

    	for ( frame_iterator = available_frames.first(); frame_iterator; frame_iterator = available_frames.next() )
    	{
            frame_iterator -> move( frame_iterator -> x() - frame_iterator -> width(), frame_iterator -> y() );
    	}
    }
    else if ( direction == RIGHT )
    {
    	interface_element -> move( interface_element -> x() + interface_element -> width(), interface_element -> y() );

    	//Move all the frames to the right
    	ESFrame *frame_iterator;

    	for ( frame_iterator = available_frames.first(); frame_iterator; frame_iterator = available_frames.next() )
    	{
            frame_iterator -> move( frame_iterator -> x() + frame_iterator -> width(), frame_iterator -> y() );
    	}
    }
}

void ILayer::deleteAllFrames()
{
    ESFrame *frame_to_delete;

    for ( frame_to_delete = available_frames.first(); frame_to_delete; frame_to_delete = available_frames.next() )
    {
        delete frame_to_delete;
    }

    number_of_frames = 0;
}

int ILayer::framePosition( ESFrame *frame )
{
    ESFrame *frame_iterator;
    int i = 0;
    for ( frame_iterator = available_frames.first(); frame_iterator; frame_iterator = available_frames.next() )
    {
	if ( frame_iterator == frame )
	    return i;
	i++;
    }

    return -1;
}

ESFrame *ILayer::frameAt( int pos )
{
    ESFrame *frame_iterator;
    int i = 0;
    for ( frame_iterator = available_frames.first(); frame_iterator; frame_iterator = available_frames.next() )
    {
	if ( i == pos )
	    return frame_iterator;
	i++;
    }

    return NULL;
}
