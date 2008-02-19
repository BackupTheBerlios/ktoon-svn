/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktlayer.h"
#include "ktscene.h"

#include <kcore/kdebug.h>

#include "ktprojectloader.h"

struct KTLayer::Private
{
	Frames frames;
	bool isVisible;
	QString name;
	int framesCount;
	bool isLocked;
};

KTLayer::KTLayer(KTScene *parent) : QObject(parent), k(new Private)
{
	k->isVisible = true;
	k->name = tr("Layer");
	k->framesCount = 0;
	k->isLocked = false;
}

KTLayer::~KTLayer()
{
	k->frames.clear(true);
	
	delete k;
}

Frames KTLayer::frames()
{
	return k->frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	k->frames = frames;
	k->framesCount = frames.count();
}

void KTLayer::setLayerName(const QString &name)
{
	k->name = name;
}

void KTLayer::setLocked(bool isLocked)
{
	k->isLocked = isLocked;
}

bool KTLayer::isLocked() const
{
	return k->isLocked;
}

void KTLayer::setVisible(bool isVisible)
{
	k->isVisible = isVisible;
// 	emit visibilityChanged(isVisible);
}

QString KTLayer::layerName() const
{
	return k->name;
}

bool KTLayer::isVisible() const
{
	return k->isVisible;
}

KTFrame *KTLayer::createFrame(int position, bool loaded)
{
	if ( position < 0 || position > k->frames.count() )
	{
		return 0;
	}
	
	KTFrame *frame = new KTFrame(this);
	
	k->framesCount++;
	
	frame->setFrameName(tr("Drawing %1").arg(k->framesCount));
	
	k->frames.insert(position, frame);
	
	if ( loaded )
	{
		KTProjectLoader::createFrame( scene()->visualIndex(), visualIndex(), position, frame->frameName(), project());
	}
	
	return frame;
}

bool KTLayer::removeFrame(int position)
{
	KTFrame *toRemove = frame(position);
	if ( toRemove )
	{
		k->frames.removeVisual(position);
		toRemove->setRepeat(toRemove->repeat()-1);
		
		if(toRemove->repeat() < 1)
		{
			delete toRemove;
		}
		return true;
	}
	
	return false;
}

bool KTLayer::moveFrame(int from, int to)
{
	if ( from < 0 || from >= k->frames.count() || to < 0 || to >= k->frames.count() )
	{
		return false;
	}
	
	k->frames.moveVisual(from, to);
	
	return true;
}


bool KTLayer::expandFrame(int position, int size)
{
	if ( position < 0 || position >= k->frames.count() )
	{
		return false;
	}
	
	KTFrame *toExpand = frame(position);
	
	if(toExpand)
	{
		for(int i = 0; i < size; i++)
		{
			k->frames.expandValue(position);
// 		k->frames.insert(position+i+1, toExpand);
		}
		toExpand->setRepeat(toExpand->repeat()+size);
		return true;
	}
	return false;
}


KTFrame *KTLayer::frame(int position) const
{
	if ( position < 0 || position >= k->frames.count() )
	{
		#ifdef K_DEBUG
			K_FUNCINFO << " FATAL ERROR: index out of bound";
		#endif
		return 0;
	}
	
	return k->frames.visualValue(position);
}


void KTLayer::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	
	setLayerName( root.attribute( "name", layerName() ) );
	
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if ( e.tagName() == "frame" )
			{
				KTFrame *frame = createFrame( k->frames.count(), true );
				
				if ( frame )
				{
					QString newDoc;
					
					{
						QTextStream ts(&newDoc);
						ts << n;
					}
					
					frame->fromXml( newDoc );
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTLayer::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("layer");
	root.setAttribute("name", k->name );
	doc.appendChild(root);
	
	foreach(KTFrame *frame, k->frames.visualValues() )
	{
		root.appendChild( frame->toXml(doc) );
	}
	
	return root;
}

KTScene *KTLayer::scene() const
{
	return static_cast<KTScene *>(parent());
}

KTProject *KTLayer::project() const
{
	return scene()->project();
}

int KTLayer::logicalIndexOf(KTFrame *frame) const
{
	return k->frames.logicalIndex(frame);
}

int KTLayer::visualIndexOf(KTFrame *frame) const
{
	return k->frames.visualIndex(frame);
}

int KTLayer::logicalIndex() const
{
	return scene()->logicalIndexOf(const_cast<KTLayer *>(this));
}

int KTLayer::visualIndex() const
{
	return scene()->visualIndexOf(const_cast<KTLayer *>(this));
}

