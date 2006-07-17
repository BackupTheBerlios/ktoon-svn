/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include "dgstengine.h"

#include <QtDebug>
#include <QTimer>
#include <QFile>

#include <dcore/ddebug.h>

DGstEngine *DGstEngine::s_instance = 0;


GstBusSyncReply DGstEngine::bus_cb(GstBus*, GstMessage* msg, gpointer pinfo) // static
{
	PlayInfo *playInfo = static_cast<PlayInfo *>(pinfo);
	
	switch ( GST_MESSAGE_TYPE(msg))
	{
		case GST_MESSAGE_ERROR:
		{
			GError* error;
			gchar* debugs;
          
			gst_message_parse_error(msg,&error,&debugs);
			qDebug() << "ERROR RECEIVED IN BUS_CB <" << error->message << ">";
			
			instance()->destroyPlayInfo( playInfo );
			
		}
		break;
		case GST_MESSAGE_EOS:
		{
			qDebug() << "FINISHED ";
			
			instance()->destroyPlayInfo( playInfo );
		}
		break;
		default: ;
	}
	
	gst_message_unref( msg );
	return GST_BUS_DROP;
}


DGstEngine *DGstEngine::instance()
{
	if ( !s_instance )
	{
		s_instance = new DGstEngine;
	}
	
	return s_instance;
}

DGstEngine::DGstEngine() : m_currentPlayer(0)
{
}


DGstEngine::~DGstEngine()
{
	QMapIterator<int, PlayInfo> i(m_players);
	while (i.hasNext())
	{
		i.next();
		gst_object_unref (GST_OBJECT (i.value().player));
	}
	
	gst_deinit();
}


QString DGstEngine::key() const
{
	return "gstreamer";
}

int DGstEngine::load( const QUrl &url, int id )
{
	QUrl path = url;
	if ( path.scheme().isEmpty() )
	{
		path.setScheme("file");
	}
	
	qDebug() << "LOAD: " << path.toString();
	
	if ( !QFile::exists(url.path()) )
	{
		m_currentPlayer = -1;
		
		dError() << "Cannot load: " << path.toString();
		return -1;
	}
	
	if ( id < 0 )
	{
		id = m_players.count();
	}
	
	m_players.insert(id, createPlayInfo(path));
	
	m_currentPlayer = id;
	
	m_players[id].id = id;
	
	gst_bus_set_sync_handler(gst_pipeline_get_bus(GST_PIPELINE(m_players[id].player)), bus_cb, &m_players[id]);
	
	return id;
}

bool DGstEngine::init()
{
	qDebug() << "INIT";
	GError *err;
	if ( !gst_init_check( NULL, NULL, &err ) )
	{
		qDebug() << "Error gst_init_check";
		return false;
	}

	// Check if registry exists
	GstElement* dummy = gst_element_factory_make ( "fakesink", "fakesink" );
	if ( !dummy )
	{
		qDebug() << "gst-register";
		return false;
	}
	
	gst_object_unref( dummy );

	return true;
}

bool DGstEngine::play(int offset)
{
	qDebug() << "PLAY ";
	
	if ( m_currentPlayer < 0 )
	{
		return false;
	}
	
	g_object_set (G_OBJECT (m_players[m_currentPlayer].player), "uri", m_players[m_currentPlayer].url.toString().toLocal8Bit().data(), NULL);
	
	gst_element_set_state (m_players[m_currentPlayer].player, GST_STATE_PLAYING);
	
	if ( offset > 0 )
	{
		seek(offset);
	}
	
	return true;
}

void DGstEngine::stop()
{
	qDebug() << "STOP";
	gst_element_set_state (m_players[m_currentPlayer].player, GST_STATE_NULL);
}

void DGstEngine::pause()
{
	qDebug("PAUSE");
	gst_element_set_state (m_players[m_currentPlayer].player, GST_STATE_PAUSED);
}

void DGstEngine::seek( uint ms )
{
	qDebug() << "SEEKING "<< ms;
	
	gst_element_get_state(m_players[m_currentPlayer].player, NULL, NULL, 100*GST_MSECOND);
	
	if (!gst_element_seek (m_players[m_currentPlayer].player, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, ms*GST_MSECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
	{
		qDebug("Can't seek");
	}
	
// 	gst_element_get_state(m_players[m_currentPlayer].player, NULL, NULL, 100*GST_MSECOND);
}

DGstEngine::PlayInfo DGstEngine::createPlayInfo(const QUrl &url)
{
	qDebug("Create play Info");
	
	PlayInfo playInfo;
	playInfo.player = gst_element_factory_make("playbin", "play");
	playInfo.url = url;
	
	return playInfo;
}

bool DGstEngine::setCurrentPlayer(int id)
{
	if ( m_players.contains(id) )
	{
		m_currentPlayer = id;
		return true;
	}
	
	return false;
}

void DGstEngine::destroyPlayInfo(const PlayInfo *playInfo)
{
	qDebug() << "Destroy play info" << playInfo->id;
	
	m_players.remove(playInfo->id);
	gst_element_set_state( playInfo->player, GST_STATE_NULL );
	gst_object_unref( GST_OBJECT( playInfo->player ) );
	
}

void DGstEngine::setVolume(int percent)
{
	if ( m_currentPlayer < 0  )
		return;
	g_object_set( G_OBJECT(m_players[m_currentPlayer].player), "volume", percent*0.01, NULL );
}


Q_EXPORT_PLUGIN2( gst_engine, DGstEngine );


