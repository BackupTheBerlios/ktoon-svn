/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
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

#include "ktsoundlayer.h"

#include <QFileInfo>

#include <ksound/kaudioplayer.h>

#include "ktlibrary.h"
#include "ktproject.h"
#include "ktlibraryobject.h"

struct KTSoundLayer::Private
{
    QString filePath, symbolName;
    int playerId;
};

KTSoundLayer::KTSoundLayer(KTScene *parent)
 : KTLayer(parent), k(new Private)
{
}

KTSoundLayer::~KTSoundLayer()
{
    delete k;
}

void KTSoundLayer::fromSymbol(const QString &symbolName)
{
    KTLibrary *library = project()->library();
    
    if (KTLibraryObject *object = library->findObject(symbolName)) {
        if (object->type() == KTLibraryObject::Sound) {
            k->symbolName = symbolName;
            k->filePath = object->dataPath();
            k->playerId = KAudioPlayer::instance()->load(k->filePath);
        }
    }
}

QString KTSoundLayer::filePath() const
{
    return k->filePath;
}

void KTSoundLayer::play()
{
    KAudioPlayer::instance()->setCurrentPlayer(k->playerId);
    KAudioPlayer::instance()->play();
}

void KTSoundLayer::stop()
{
    KAudioPlayer::instance()->setCurrentPlayer(k->playerId);
    KAudioPlayer::instance()->stop();
}

void KTSoundLayer::fromXml(const QString &xml)
{
    QDomDocument document;
    
    if (! document.setContent(xml))
        return;
    
    QDomElement root = document.documentElement();
    setLayerName(root.attribute("name", layerName()));
    
    fromSymbol(root.attribute("symbol"));
}

QDomElement KTSoundLayer::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("soundlayer");
    root.setAttribute("name", layerName());
    root.setAttribute("symbol", k->symbolName);
    
    return root;
}
