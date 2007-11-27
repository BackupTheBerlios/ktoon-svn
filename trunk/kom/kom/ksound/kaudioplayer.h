/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KAUDIOPLAYER_H
#define KAUDIOPLAYER_H

#include <qobject.h>

#include "kaudioengineiface.h"
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KAudioPlayer : public QObject
{
	Q_OBJECT;
	public:
		KAudioPlayer();
		~KAudioPlayer();
		
		static KAudioPlayer *instance();
		
		void loadEngine(const QString &engine);
		
		int load( const QUrl &url, int id = -1 );
		void play(int offset = 0);
		void pause();
		
		void setCurrentPlayer(int id);
		
		void stop();
		void seek( uint ms );
		void setVolume(int percent);
		
	private:
		static KAudioPlayer *s_instance;
		
		KAudioEngineIface *m_engine;
};

namespace KPlayer
{
	inline int play(const QUrl &url, int seek = 0)
	{
		int id = KAudioPlayer::instance()->load(url);
		
		KAudioPlayer::instance()->play( seek );
		
		return id;
	}
};

#endif

