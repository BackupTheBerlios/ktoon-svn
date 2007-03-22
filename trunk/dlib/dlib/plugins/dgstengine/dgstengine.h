/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#ifndef DGSTENGINE_H
#define DGSTENGINE_H

#include <QObject>
#include <QMap>

#include <dsound/daudioengineiface.h>

#ifdef HAVE_GST10
#include <gst/gst.h>
#endif

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class DGstEngine : public QObject, public DAudioEngineIface
{
	Q_OBJECT;
	Q_INTERFACES(DAudioEngineIface);
	
	public:
		DGstEngine();
		~DGstEngine();
		virtual QString key() const;
		
		virtual int load( const QUrl &url, int id = -1 );
		
		virtual bool init();
		virtual bool play(int offset = 0);
		
		virtual void stop();
		virtual void pause();
		virtual void seek( uint ms );
		
		bool setCurrentPlayer(int id);
		
		void setVolume(int percent);
		
		static DGstEngine *instance();

	private:
		static DGstEngine *s_instance;
		
		struct PlayInfo
		{
			GstElement *player;
			QUrl url;
			int id;
		};
		
		QMap<int, PlayInfo> m_players;
		
		int m_currentPlayer;
		
	private:
		static GstBusSyncReply bus_cb( GstBus*, GstMessage*, gpointer );
		void destroyPlayInfo(const PlayInfo *playInfo);
		
	protected:
		PlayInfo createPlayInfo( const QUrl &url );
};

#endif

