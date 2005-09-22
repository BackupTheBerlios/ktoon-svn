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

 #include "keyframe.h"
 
 #include <new>

//-------------- CONSTRUCTOR ---------------

KeyFrame::KeyFrame()
{
	m_offset = 0;
	m_length = 1;
	m_motion = false;
	m_name = "";
	m_camera = new Camera();
	m_drawing = new(std::nothrow) GLDrawing();
	if(m_drawing)
	{
		delete m_camera;
		//FIXME: para que se hace esto?throw std::bad_alloc();
// 		throw std::bad_alloc();
	}
}

//------------- DESTRUCTOR ------------------

KeyFrame::~KeyFrame()
{
	delete m_camera;
	delete m_drawing;
}

//------------ PUBLIC MEMBERS ---------------

void KeyFrame::setOffsetKeyFrame( const int & offset )
{
	m_offset = offset;
}

void KeyFrame::setLengthKeyFrame( const int & length )
{
	m_length = length;
}

void KeyFrame::setMotionKeyFrame( bool motion )
{
	m_motion = motion;
}

void KeyFrame::setNameKeyFrame( const QString & name )
{
	m_name = name;
}

void KeyFrame::setDrawing( GLDrawing *drawing )
{
	Q_CHECK_PTR( drawing );
	m_drawing = drawing;
}

void KeyFrame::setCamera( Camera *camera )
{
	Q_CHECK_PTR( camera );
	m_camera = camera;
}

int KeyFrame::offsetKeyFrame() const
{
	return m_offset;
}

int KeyFrame::lengthKeyFrame() const
{
	return m_length;
}

bool KeyFrame::motionKeyFrame() const
{
	return m_motion;
}

QString KeyFrame::nameKeyFrame() const
{
	return m_name;
}

GLDrawing *KeyFrame::getDrawing() const
{
	Q_CHECK_PTR( drawing );
	return m_drawing;
}

Camera *KeyFrame::getCamera() const
{
	Q_CHECK_PTR( camera );
	return m_camera;
}

QDomElement KeyFrame::createXML( QDomDocument &doc )
{
	QDomElement e = doc.createElement( "Keyframe" );

	QString bool_str;
	if ( m_motion )
		bool_str = "true";
	else
		bool_str = "false";
	e.setAttribute( "Name", m_name );
	e.setAttribute( "Offset", m_offset );
	e.setAttribute( "Length", m_length );
	e.setAttribute( "Motion", bool_str );
	QDomElement cameraTag = m_camera -> createXML( doc );
	e.appendChild( cameraTag );
	QDomElement drawingTag = m_drawing -> createXML( doc );
	e.appendChild( drawingTag );
	return e;
}
