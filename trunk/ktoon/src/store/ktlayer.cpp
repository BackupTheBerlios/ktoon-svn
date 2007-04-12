/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktlayer.h"
#include "ktscene.h"

#include <dcore/ddebug.h>

#include "ktprojectloader.h"

struct KTLayer::Private
{
	Frames frames;
	bool isVisible;
	QString name;
	int framesCount;
	bool isLocked;
};

KTLayer::KTLayer(KTScene *parent) : QObject(parent), d(new Private)
{
	d->isVisible = true;
	d->name = tr("Layer");
	d->framesCount = 0;
	d->isLocked = false;
}

KTLayer::~KTLayer()
{
	qDeleteAll(d->frames);
	d->frames.clear();
	
	delete d;
}

Frames KTLayer::frames()
{
	return d->frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	d->frames = frames;
	d->framesCount = frames.count();
}

void KTLayer::setLayerName(const QString &name)
{
	d->name = name;
}

void KTLayer::setLocked(bool isLocked)
{
	d->isLocked = isLocked;
}

bool KTLayer::isLocked() const
{
	return d->isLocked;
}

void KTLayer::setVisible(bool isVisible)
{
	d->isVisible = isVisible;
// 	emit visibilityChanged(isVisible);
}

QString KTLayer::layerName() const
{
	return d->name;
}

bool KTLayer::isVisible() const
{
	return d->isVisible;
}

KTFrame *KTLayer::createFrame(int position, bool loaded)
{
	if ( position < 0 || position > d->frames.count() )
	{
		return 0;
	}
	
	KTFrame *frame = new KTFrame(this);
	
	d->framesCount++;
	
	frame->setFrameName(tr("Drawing %1").arg(d->framesCount));
	
	d->frames.insert(position, frame);
	
	if ( loaded )
	{
		KTProjectLoader::createFrame( scene()->index(), index(), position, frame->frameName(), project());
	}
	
	return frame;
}

bool KTLayer::removeFrame(int position)
{
	KTFrame *toRemove = frame(position);
	if ( toRemove )
	{
		d->frames.removeAt(position);
		delete toRemove;
		
		return true;
	}
	
	return false;
}

bool KTLayer::moveFrame(int from, int to)
{
	if ( from < 0 || from >= d->frames.count() || to < 0 || to >= d->frames.count() )
	{
		return false;
	}
	
	KTFrame *frame = d->frames.takeAt(from);
	
	d->frames.insert(to, frame);
	
	return true;
}



KTFrame *KTLayer::frame(int position)
{
	if ( position < 0 || position >= d->frames.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound";
		return 0;
	}
	
	return d->frames[position];
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
				KTFrame *frame = createFrame( d->frames.count(), true );
				
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
	root.setAttribute("name", d->name );
	doc.appendChild(root);
	
	Frames::ConstIterator iterator = d->frames.begin();
	
	while ( iterator != d->frames.end() )
	{
		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
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

int KTLayer::indexOf(KTFrame *frame) const
{
	return d->frames.indexOf(frame);
}

int KTLayer::index() const
{
	return scene()->indexOf(const_cast<KTLayer *>(this));
}
