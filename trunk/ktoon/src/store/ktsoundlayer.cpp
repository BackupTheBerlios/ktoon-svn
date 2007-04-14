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

#include <dsound/daudioplayer.h>

struct KTSoundLayer::Private
{
	QString filePath;
	
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

void KTSoundLayer::setFilePath(const QString &filePath)
{
	d->filePath = filePath;
	
	d->playerId = DAudioPlayer::instance()->load(filePath);
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

