/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktsoundlayer.h"

#include <QFileInfo>

#include <dsound/daudioplayer.h>

#include "ktlibrary.h"
#include "ktproject.h"
#include "ktlibraryobject.h"

struct KTSoundLayer::Private
{
	QString filePath, symbolName;
	int playerId;
};

KTSoundLayer::KTSoundLayer(KTScene *parent)
 : KTLayer(parent), d(new Private)
{
}


KTSoundLayer::~KTSoundLayer()
{
	delete d;
}

void KTSoundLayer::fromSymbol(const QString &symbolName)
{
	KTLibrary *library = project()->library();
	
	if( KTLibraryObject *object = library->findObject(symbolName) )
	{
		if( object->type() == KTLibraryObject::Sound)
		{
			d->symbolName = symbolName;
			d->filePath = object->dataPath();
			d->playerId = DAudioPlayer::instance()->load(d->filePath);
		}
	}
}

QString KTSoundLayer::filePath() const
{
	return d->filePath;
}

void KTSoundLayer::play()
{
	DAudioPlayer::instance()->setCurrentPlayer(d->playerId);
	DAudioPlayer::instance()->play();
}

void KTSoundLayer::stop()
{
	DAudioPlayer::instance()->setCurrentPlayer(d->playerId);
	DAudioPlayer::instance()->stop();
}


void KTSoundLayer::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	setLayerName( root.attribute( "name", layerName() ) );
	
	fromSymbol(root.attribute("symbol"));
}

QDomElement KTSoundLayer::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("soundlayer");
	root.setAttribute("name", layerName() );
	
	root.setAttribute("symbol", d->symbolName);
	
	
	return root;
}



