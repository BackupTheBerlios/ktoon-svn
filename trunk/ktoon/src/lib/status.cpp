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

#include "status.h"

#include <iostream>
#include <qworkspace.h>
#include <qcursor.h>

//-------------- CONSTRUCTOR ---------------

Status::Status() : m_currentDrawingArea(0)
{
	qDebug("[Initializing Status]");
	current_scene = NULL;
	current_layer = NULL;
	current_keyframe = NULL;
	current_timeline_keyframe = NULL;
	current_soundclip = NULL;
	current_previous_onion_skin = 0;
	current_next_onion_skin = 0;
	current_frame_rate = 25;
	current_camera_width = 320;
	current_camera_height = 240;
	current_background_color = QColor( 255, 255, 255 );
	current_grid_color = QColor( 210, 210, 210 );
	current_ntsc_color = QColor( 255, 0, 0 );
    
	m_document = new Document();
}

void Status::init()
{
	if ( m_document )
	{
		delete m_document;
		m_document = 0;
	}
	m_document->init();
	m_document->setNameDocument(tr("Document"));
}

void Status::setupDrawingArea(QWorkspace *ws)
{
	qDebug("Setup drawing area...");
	KTStatus -> setCurrentScene( (m_document-> getAnimation()->getScenes() ).first() );
	KTStatus -> setCurrentLayer( ( KTStatus -> currentScene() -> getLayers() ).first() );
	setCurrentKeyFrame( NULL );
	QPtrList<KeyFrame> empty;
	KTStatus -> setRenderKeyframes( empty );
	
// 	if ( m_currentDrawingArea )
// 	{
// 		delete m_currentDrawingArea;
// 	}
	
#ifndef NO_OPENGL
	m_currentDrawingArea = new DrawingArea( ws, tr( "Document" ) + QString( "1" ) );
	m_currentDrawingArea -> setCursor( QCursor( Qt::ForbiddenCursor ) );
	m_currentDrawingArea -> show();
	m_currentDrawingArea -> setActiveWindow();
#else
	m_currentDrawingArea = new DrawingAreaQt( ws, tr( "Document" ) + QString( "1" ) );
	m_currentDrawingArea -> resize( 641, 481 );
	m_currentDrawingArea -> move( 146, 8 );
	m_currentDrawingArea -> show();

	m_currentDrawingArea -> setMinimumSize( 321, 241 );
	m_currentDrawingArea -> setMaximumSize( 321, 241 );
#endif
}

//------------- DESTRUCTOR ------------------

Status::~Status()
{
	qDebug("[Destroying Status]");
	render_camera_frames.clear();
	if(current_outline_color == current_fill_color)
	{
		std::cerr << "\ncurrent_outline_color == current_fill_color!\n";
	}
	else
	{
		delete current_outline_color;
	}

	delete current_fill_color;
	delete current_scene;
	delete current_layer;
	delete current_keyframe;
	delete current_timeline_keyframe;
	delete current_soundclip;
}

//------------ PUBLIC MEMBERS ---------------

Status *Status::instance()
{
	static Status *status = new Status();
	
	return status;
}

#ifndef NO_OPENGL
DrawingArea *
#else
DrawingAreaQt *
#endif
Status::currentDrawingArea()
{
	return m_currentDrawingArea;
}


bool Status::closeCurrent()
{
	bool yes = false;
	if ( m_currentDrawingArea )
	{
		yes = m_currentDrawingArea->close(true);
// 		delete m_currentDrawingArea; // this produce a crash
// 		m_currentDrawingArea = 0;
	}
	
	
	return yes;
}


Document *Status::currentDocument()
{
	return m_document;
}

void Status::setCurrentPreviousOnionSkin( const int &previous_onion )
{
    current_previous_onion_skin = previous_onion;
}

void Status::setCurrentNextOnionSkin( const int &next_onion )
{
    current_next_onion_skin = next_onion;
}

void Status::setCurrentCursor( const int &cursor )
{
    current_cursor = cursor;
}

void Status::setCurrentOutlineColor( KTColor *color )
{
    Q_CHECK_PTR( color );
    current_outline_color = color;
}

void Status::setCurrentFillColor( KTColor *color )
{
    Q_CHECK_PTR( color );
    current_fill_color = color;
}

void Status::setCurrentBrush( Brush *brush )
{
    Q_CHECK_PTR( brush );
    current_brush = brush;
}

void Status::setCurrentScene( Scene *scene )
{
    current_scene = scene;
}

void Status::setCurrentLayer( Layer *layer )
{
    current_layer = layer;
}

void Status::setCurrentILayer(ILayer *ilayer)
{
	m_currentILayer = ilayer;
}

void Status::setCurrentKeyFrame( KeyFrame *keyframe )
{
    current_keyframe = keyframe;
}

void Status::setCurrentTimelineKeyFrame( KeyFrame *t_keyframe )
{
    Q_CHECK_PTR( t_keyframe );
    current_timeline_keyframe = t_keyframe;
}

void Status::setCurrentSoundClip( SoundClip *soundclip )
{
    Q_CHECK_PTR( soundclip );
    current_soundclip = soundclip;
}

void Status::setCurrentFrameRate( int rate )
{
    current_frame_rate = rate;
}

void Status::setCurrentCameraWidth( int width )
{
    current_camera_width = width;
}

void Status::setCurrentCameraHeight( int height )
{
    current_camera_height = height;
}

void Status::setRenderKeyframes( QPtrList<KeyFrame> frames )
{
    render_camera_frames = frames;
}

void Status::setCurrentBackgroundColor( QColor color )
{
    current_background_color = color;
}

void Status::setCurrentGridColor( QColor color )
{
    current_grid_color = color;
}

void Status::setCurrentNTSCColor( QColor color )
{
    current_ntsc_color = color;
}

int Status::currentPreviousOnionSkin() const
{
    return current_previous_onion_skin;
}

int Status::currentNextOnionSkin() const
{
    return current_next_onion_skin;
}

int Status::currentCursor() const
{
    return current_cursor;
}

KTColor* Status::currentOutlineColor()
{
    return current_outline_color;
}

KTColor* Status::currentFillColor()
{
    return current_fill_color;
}

Brush* Status::currentBrush()
{
    return current_brush;
}

Scene* Status::currentScene()
{
    return current_scene;
}

Layer* Status::currentLayer()
{
    return current_layer;
}

ILayer *Status::currentILayer()
{
	return m_currentILayer;
}

KeyFrame* Status::currentKeyFrame()
{
    return current_keyframe;
}

KeyFrame* Status::currentTimelineKeyFrame()
{
    return current_timeline_keyframe;
}

SoundClip* Status::currentSoundClip()
{
    return current_soundclip;
}

int Status::currentFrameRate() const
{
    return current_frame_rate;
}

int Status::currentCameraWidth() const
{
    return current_camera_width;
}

int Status::currentCameraHeight() const
{
    return current_camera_height;
}

QPtrList<KeyFrame> Status::renderKeyframes() const
{
    return render_camera_frames;
}

QColor Status::currentBackgroundColor()
{
    return current_background_color;
}

QColor Status::currentGridColor()
{
    return current_grid_color;
}

QColor Status::currentNTSCColor()
{
    return current_ntsc_color;
}
