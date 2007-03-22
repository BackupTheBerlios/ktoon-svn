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

KTLayer::KTLayer(KTScene *parent) : QObject(parent), m_isVisible(true), m_name(tr("Layer")), m_framesCount(0), m_isLocked(false)
{
}

KTLayer::~KTLayer()
{
	qDeleteAll(m_frames);
	m_frames.clear();
	
}

Frames KTLayer::frames()
{
	return m_frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	m_frames = frames;
	m_framesCount = frames.count();
}

void KTLayer::setLayerName(const QString &name)
{
	m_name = name;
}

void KTLayer::setLocked(bool isLocked)
{
	m_isLocked = isLocked;
}

bool KTLayer::isLocked() const
{
	return m_isLocked;
}

void KTLayer::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
// 	emit visibilityChanged(isVisible);
}

QString KTLayer::layerName() const
{
	return m_name;
}

bool KTLayer::isVisible() const
{
	return m_isVisible;
}

KTFrame *KTLayer::createFrame(int position, bool loaded)
{
	if ( position < 0 || position > m_frames.count() )
	{
		return 0;
	}
	
	KTFrame *frame = new KTFrame(this);
	
	m_framesCount++;
	
	frame->setFrameName(tr("Drawing %1").arg(m_framesCount));
	
	m_frames.insert(position, frame);
	
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
		m_frames.removeAt(position);
		delete toRemove;
		
		return true;
	}
	
	return false;
}

bool KTLayer::moveFrame(int from, int to)
{
	if ( from < 0 || from >= m_frames.count() || to < 0 || to >= m_frames.count() )
	{
		return false;
	}
	
	KTFrame *frame = m_frames.takeAt(from);
	
	m_frames.insert(to, frame);
	
	return true;
}



KTFrame *KTLayer::frame(int position)
{
	if ( position < 0 || position >= m_frames.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound";
		return 0;
	}
	
	return m_frames[position];
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
				KTFrame *frame = createFrame( m_frames.count(), true );
				
				if ( frame )
				{
					QDomDocument newDoc;
					newDoc.appendChild(newDoc.importNode(n, true ));
					frame->fromXml( newDoc.toString(0) );
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTLayer::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("layer");
	root.setAttribute("name", m_name );
	doc.appendChild(root);
	
	Frames::ConstIterator iterator = m_frames.begin();
	
	while ( iterator != m_frames.end() )
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
	return m_frames.indexOf(frame);
}

int KTLayer::index() const
{
	return scene()->indexOf(const_cast<KTLayer *>(this));
}
