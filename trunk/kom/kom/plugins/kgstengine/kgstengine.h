/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   License:                                                              *
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


#ifndef KGSTENGINE_H
#define KGSTENGINE_H

#include <QObject>
#include <QHash>

#include <ksound/kaudioengineiface.h>

#ifdef HAVE_GST10
#include <gst/gst.h>
#endif

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class KGstEngine : public QObject, public KAudioEngineIface
{
	Q_OBJECT;
	Q_INTERFACES(KAudioEngineIface);
	
	public:
		KGstEngine();
		~KGstEngine();
		virtual QString key() const;
		
		virtual int load( const QUrl &url, int id = -1 );
		
		virtual bool init();
		virtual bool play(int offset = 0);
		
		virtual void stop();
		virtual void pause();
		virtual void seek( uint ms );
		
		bool setCurrentPlayer(int id);
		
		void setVolume(int percent);
		
		static KGstEngine *instance();

	private:
		static KGstEngine *s_instance;
		
		struct PlayInfo
		{
			KGstEngine *engine;
			GstElement *player;
			QUrl url;
			int id;
			bool stopped;
		};
		
		QHash<int, PlayInfo> m_players;
		
		int m_currentPlayer;
		
	private:
		static GstBusSyncReply bus_cb( GstBus*, GstMessage*, gpointer );
		void destroyPlayInfo(const PlayInfo *playInfo);
		
	protected:
		PlayInfo createPlayInfo( const QUrl &url );
};

#endif

