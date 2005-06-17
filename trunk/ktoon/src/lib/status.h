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

#ifndef STATUS_H
#define STATUS_H

/**
 * @file status.h
 * @brief Include this file if you need the class Status
 */

#include <qptrlist.h>
#include <qcolor.h>
#include <qguardedptr.h> 

#include "brush.h"
#include "ktcolor.h"
#include "scene.h"
#include "layer.h"
#include "ilayer.h"
#include "keyframe.h"
#include "soundclip.h"
#include "glgraphiccomponent.h"
#include "tools.h"

#include "document.h"

#ifndef NO_OPENGL
	#include "drawingarea.h"
#else
	#include "drawingareaqt.h"
#endif

class Status;
class QWorkspace;

/**
 * @brief Class that handles the application's general status
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains some useful information about the current status of the application, such as
 * current cursor, outline color, fill color, brush, etc. It is used in order to synchronize some actions
 * among application's widgets.
*/
class Status : public QObject
{
	public:
		/**
		* @brief Default Constructor
		*
		* Constructs a Status object.
		*/
		Status();
		/**
		* @brief Default Destructor
		*
		* Destroys the Status object.
		*/
		~Status();
			 
			 
		 void init();
		 
		 static Status *instance();
		 
		 Document *currentDocument();
		
		/**
		* @brief Sets a new current drawing area's previous onion skin
		*
		* @param previous_onion The new number of previous onion skins.
		*/
		void setCurrentPreviousOnionSkin( const int &previous_onion );
		/**
		* @brief Sets a new current drawing area's next onion skin
		*
		* @param next_onion The new number of next onion skins.
		*/
		void setCurrentNextOnionSkin( const int &next_onion );
		/**
		* @brief Sets a new current drawing area's cursor
		*
		* This cursor indicates the active Tool.
		* @param cursor The new current cursor. See Tools::ToolType.
		*/
		void setCurrentCursor( const int &cursor );
		/**
		* @brief Sets a new current outline color
		*
		* @param color The new current outline color
		*/
		void setCurrentOutlineColor( KTColor *color );
		/**
		* @brief Sets a new current fill color
		*
		* @param color The new current fill color
		*/
		void setCurrentFillColor( KTColor *color );
		/**
		* @brief Sets a new current brush
		*
		* @param brush The new current brush
		*/
		void setCurrentBrush( Brush *brush );
		/**
		* @brief Sets a new current scene
		*
		* @param scene The new current scene
		*/
		void setCurrentScene( Scene *scene );
		/**
		* @brief Sets a new current layer
		*
		* @param layer The new current layer
		*/
		void setCurrentLayer( Layer *layer );
			 
			 
		void setCurrentILayer (ILayer *ilayer); // FIXME
		/**
		* @brief Sets a new current keyframe
		*
		* @param keyframe The new current keyframe
		*/
		void setCurrentKeyFrame( KeyFrame *keyframe );
		/**
		* @brief Sets a new current timeline keyframe
		*
		* @param t_keyframe The new current timeline keyframe
		*/
		void setCurrentTimelineKeyFrame( KeyFrame *t_keyframe );
		/**
		* @brief Sets a new current sound clip
		*
		* @param soundclip The new current sound clip
		*/
		void setCurrentSoundClip( SoundClip *soundclip );
		/**
		* @brief Sets a new current frame rate
		*
		* @param rate The new current frame rate
		*/
		void setCurrentFrameRate( int rate );
		/**
		* @brief Sets a new current camera width
		*
		* @param width The new current camera width
		*/
		void setCurrentCameraWidth( int width );
		/**
		* @brief Sets a new current camera height
		*
		* @param height The new current camera height
		*/
		void setCurrentCameraHeight( int height );
		/**
		* @brief Sets the render camera keyframes
		*
		* These are the keyframes that are going to be displayed into the render camera preview dialog box.
		* @param frames The render camera keyframes
		*/
		void setRenderKeyframes( QPtrList<KeyFrame> frames );
		/**
		* @brief Sets a new current background color
		*
		* @param color The new current background color
		*/
		void setCurrentBackgroundColor( QColor color );
		/**
		* @brief Sets a new current grid color
		*
		* @param color The new current grid color
		*/
		void setCurrentGridColor( QColor color );
		/**
		* @brief Sets a new current NTSC color
		*
		* @param color The new current NTSC color
		*/
		void setCurrentNTSCColor( QColor color );
		
		/**
		* @brief Gets the current previous onion skin
		*
		* @return The current previous onion skin.
		*/
		int currentPreviousOnionSkin() const;
		/**
		* @brief Gets the current next onion skin
		*
		* @return The current next onion skin.
		*/
		int currentNextOnionSkin() const;
		/**
		* @brief Gets the current drawing area's cursor
		*
		* @return The current cursor. See Tools::ToolType
		*/
		int currentCursor() const;
		/**
		* @brief Gets the current outline color
		*
		* @return The current outline color
		*/
		KTColor *currentOutlineColor();
		/**
		* @brief Gets the current fill color
		*
		* @return The current fill color
		*/
		KTColor *currentFillColor();
		/**
		* @brief Gets the current brush
		*
		* @return The current brush
		*/
		Brush *currentBrush();
		/**
		* @brief Gets the current scene
		*
		* @return The current scene
		*/
		Scene *currentScene();
		/**
		* @brief Gets the current layer
		*
		* @return The current layer
		*/
		Layer *currentLayer();
			 
			 
		/**
		 *  Gets the current ilayer
		 * @return 
		 */
		ILayer *currentILayer();
		/**
		* @brief Gets the current keyframe
		*
		* @return The current keyframe
		*/
		KeyFrame *currentKeyFrame();
		/**
		* @brief Gets the current timeline keyframe
		*
		* @return The current timeline keyframe
		*/
		KeyFrame *currentTimelineKeyFrame();
		/**
		* @brief Gets the current sound clip
		*
		* @return The current sound clip
		*/
		SoundClip *currentSoundClip();
		/**
		* @brief Gets the current frame rate
		*
		* @return The current frame rate
		*/
		int currentFrameRate() const;
		/**
		* @brief Gets the current camera width
		*
		* @return The current camera width
		*/
		int currentCameraWidth() const;
		/**
		* @brief Gets the current camera height
		*
		* @return The current camera height
		*/
		int currentCameraHeight() const;
		/**
		* @brief Gets the render camera keyframes
		*
		* These are the keyframes that are going to be displayed into the render camera preview dialog box.
		* @return The render camera keyframes
		*/
		QPtrList<KeyFrame> renderKeyframes() const;
		/**
		* @brief Gets the current background color
		*
		* @return The current background color
		*/
		QColor currentBackgroundColor();
		/**
		* @brief Gets the current grid color
		*
		* @return The current grid color
		*/
		QColor currentGridColor();
		/**
		* @brief Gets the current NTSC color
		*
		* @return The current NTSC color
		*/
		QColor currentNTSCColor();
			 
#ifndef NO_OPENGL
		DrawingArea *currentDrawingArea();
#else
		DrawingAreaQt *currentDrawingArea();
#endif
		
		void setupDrawingArea(QWorkspace *ws);
		
		bool closeCurrent();
		
		bool isValid();

	private:
		short int current_tool;
		int current_cursor;
		int current_previous_onion_skin;
		int current_next_onion_skin;
		KTColor *current_outline_color;
		KTColor *current_fill_color;
		Brush *current_brush;
		Scene *current_scene;
		Layer *current_layer;
		ILayer *m_currentILayer;
		KeyFrame *current_keyframe;
		KeyFrame *current_timeline_keyframe;
		SoundClip *current_soundclip;
		int current_frame_rate;
		int current_camera_width;
		int current_camera_height;
		QPtrList<KeyFrame> render_camera_frames;
		QColor current_background_color, current_grid_color, current_ntsc_color;
		
		Document *m_document;
#ifndef NO_OPENGL
		QGuardedPtr<DrawingArea> m_currentDrawingArea;
#else
		QGuardedPtr<DrawingAreaQt> m_currentDrawingArea;
#endif
};

#define KTStatus static_cast<Status *>(Status::instance())

#endif
