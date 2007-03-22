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

#include "daudioplayer.h"

#include <QDir>
#include <QTimer>
#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

DAudioPlayer *DAudioPlayer::s_instance = 0;


DAudioPlayer::DAudioPlayer() : QObject(), m_engine(0)
{
}


DAudioPlayer::~DAudioPlayer()
{
}

DAudioPlayer *DAudioPlayer::instance()
{
	if ( !s_instance )
	{
		s_instance = new DAudioPlayer;
	}
	
	return s_instance;
}

void DAudioPlayer::loadEngine(const QString &engineKey)
{
	dDebug("audio engine") << "Loading engine: " << engineKey;
	
	QDir m_pluginDirectory = QDir(HOME_DIR+"/plugins/");
	
	foreach (QString fileName, m_pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(m_pluginDirectory.absoluteFilePath(fileName));
		QObject *plugin = qobject_cast<QObject*>(loader.instance());
		
		if (plugin)
		{
			dDebug("audio engine") << "******FILE: " << fileName;
			DAudioEngineIface *engine = qobject_cast<DAudioEngineIface *>(plugin);
			
			if ( engine )
			{
				if ( engine->key() == engineKey )
				{
					dDebug("audio engine") << "Loaded!";
					m_engine = engine;
					m_engine->init();
					break;
				}
			}
		}
	}
}


int DAudioPlayer::load( const QUrl &url, int id )
{
	if ( m_engine )
	{
		return m_engine->load(url, id);
	}
	return -1;
}

void DAudioPlayer::play(int offset)
{
	if ( m_engine )
	{
		m_engine->play( offset);
	}
}

void DAudioPlayer::pause()
{
	if ( m_engine )
	{
		m_engine->pause();
	}
}

void DAudioPlayer::setCurrentPlayer(int id)
{
	if ( m_engine )
	{
		m_engine->setCurrentPlayer( id );
	}
}

void DAudioPlayer::stop()
{
	if ( m_engine )
	{
		m_engine->stop();
	}
}

void DAudioPlayer::seek( uint ms )
{
	if ( m_engine )
	{
		m_engine->seek(ms);
	}
}

void DAudioPlayer::setVolume(int percent)
{
	if ( m_engine )
	{
		m_engine->setVolume( percent );
	}
}

