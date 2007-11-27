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

#include "ktipdatabase.h"

#include "kalgorithm.h"

#include <QDomDocument>
#include <QFile>

#include "kdebug.h"

struct KTipDatabase::Private
{
	QList<KTip> tips;
	int currentTipIndex;
};

KTipDatabase::KTipDatabase(const QString &file) : k(new Private)
{
	loadTips( file );
	
	if ( !k->tips.isEmpty() )
	{
		k->currentTipIndex = KAlgorithm::random() % k->tips.count();
	}
}


KTipDatabase::~KTipDatabase()
{
	delete k;
}

KTip KTipDatabase::tip() const
{
	if (k->currentTipIndex >= 0 && k->currentTipIndex < k->tips.count() )
		return k->tips[k->currentTipIndex];
	return KTip();
}

void KTipDatabase::nextTip()
{
	if (k->tips.isEmpty())
		return ;
	k->currentTipIndex += 1;
	if (k->currentTipIndex >= (int) k->tips.count())
	{
		k->currentTipIndex = 0;
	}
}

void KTipDatabase::prevTip()
{
	if (k->tips.isEmpty())
		return ;
	k->currentTipIndex -= 1;
	if (k->currentTipIndex < 0)
	{
		k->currentTipIndex = k->tips.count() - 1;
	}
}

void KTipDatabase::loadTips(const QString &filePath)
{
	QDomDocument doc;
	QFile file(filePath);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement();
		if(!e.isNull())
		{
			if ( e.tagName() == "tip" )
			{
				KTip tip;
				tip.text = e.text();
				k->tips << tip;
			}
		}
		n = n.nextSibling();
	}
}


