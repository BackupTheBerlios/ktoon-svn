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

#include <QtDebug>

DAudioPlayer *DAudioPlayer::s_instance = 0;


DAudioPlayer::DAudioPlayer() : QObject()
{
}


DAudioPlayer::~DAudioPlayer()
{
	qDebug("~AudioPlayer");
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
	qDebug() << "Loading engine: " << engineKey;
	
// 	QDir m_pluginDirectory = QDir(HOME+"/plugins/");
	QDir m_pluginDirectory = QDir("dgstengine");

	foreach (QString fileName, m_pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(m_pluginDirectory.absoluteFilePath(fileName));
		QObject *plugin = qobject_cast<QObject*>(loader.instance());
		
		if (plugin)
		{
			qDebug() << "******FILE: " << fileName;
			DAudioEngineIface *engine = qobject_cast<DAudioEngineIface *>(plugin);
			
			if ( engine )
			{
				if ( engine->key() == engineKey )
				{
					qDebug() << "Loaded!";
					m_engine = engine;
					m_engine->init();
				}
			}
		}
	}
}


int DAudioPlayer::load( const QUrl &url, int id )
{
	return m_engine->load(url, id);
}

void DAudioPlayer::play(int offset)
{
	m_engine->play( offset);
}

void DAudioPlayer::pause()
{
	m_engine->pause();
}

void DAudioPlayer::setCurrentPlayer(int id)
{
	m_engine->setCurrentPlayer( id );
}

void DAudioPlayer::stop()
{
	m_engine->stop();
}

void DAudioPlayer::seek( uint ms )
{
	m_engine->seek(ms);
}

void DAudioPlayer::setVolume(int percent)
{
	m_engine->setVolume( percent );
}

