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

#include "kaudioplayer.h"

#include <QDir>
#include <QTimer>
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#define PLUGIN_DIR QString(KLIB_PREFIX)+"/lib/klib/plugins"

KAudioPlayer *KAudioPlayer::s_instance = 0;

KAudioPlayer::KAudioPlayer() : QObject(), m_engine(0)
{
}

KAudioPlayer::~KAudioPlayer()
{
}

KAudioPlayer *KAudioPlayer::instance()
{
    if (!s_instance)
        s_instance = new KAudioPlayer;
    
    return s_instance;
}

void KAudioPlayer::loadEngine(const QString &engineKey)
{
    kDebug("audio engine") << "Loading engine: " << engineKey << " from: " << PLUGIN_DIR;
    
    QDir m_pluginDirectory = QDir(PLUGIN_DIR);
    
    foreach (QString fileName, m_pluginDirectory.entryList(QDir::Files)) {
        QPluginLoader loader(m_pluginDirectory.absoluteFilePath(fileName));
        QObject *plugin = qobject_cast<QObject*>(loader.instance());
        
        if (plugin) {
            kDebug("audio engine") << "******FILE: " << fileName;
            KAudioEngineIface *engine = qobject_cast<KAudioEngineIface *>(plugin);
            
            if (engine) {
                if (engine->key() == engineKey) {
                    kDebug("audio engine") << "Loaded!";
                    m_engine = engine;
                    m_engine->init();
                    break;
                }
            }
        }
    }
}

int KAudioPlayer::load(const QUrl &url, int id)
{
    if (m_engine)
        return m_engine->load(url, id);

    return -1;
}

void KAudioPlayer::play(int offset)
{
    if (m_engine)
        m_engine->play( offset);
}

void KAudioPlayer::pause()
{
    if (m_engine)
        m_engine->pause();
}

void KAudioPlayer::setCurrentPlayer(int id)
{
    if (m_engine)
        m_engine->setCurrentPlayer(id);
}

void KAudioPlayer::stop()
{
    if (m_engine)
        m_engine->stop();
}

void KAudioPlayer::seek(uint ms)
{
    if (m_engine)
        m_engine->seek(ms);
}

void KAudioPlayer::setVolume(int percent)
{
    if (m_engine) {
        m_engine->setVolume(percent);
    }
}

