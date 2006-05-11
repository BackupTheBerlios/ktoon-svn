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

#include "ktkeyframe.h"
#include "ddebug.h"

KTKeyFrame::KTKeyFrame(const KTKeyFrame &kf) : KTSerializableObject(kf.parent()), m_name(kf.m_name), m_isLocked(kf.m_isLocked), m_nClones(kf.m_nClones)
{
	QList<AGraphicComponent *>::const_iterator it;
	for(it = kf.m_components.begin(); it != kf.m_components.end(); ++it )
	{
		addComponent( new AGraphicComponent(*(*it)));
	}
}



KTKeyFrame::KTKeyFrame(QObject *parent) : KTSerializableObject(parent), m_name("Frame"), m_isLocked(false), m_nClones(0)
{
	
}

KTKeyFrame::KTKeyFrame(const QString &frameName, QObject * parent) : KTSerializableObject(parent), m_name(frameName), m_isLocked(false), m_nClones(0)
{	
	
}

// KTKeyFrame::KTKeyFrame(const KTKeyFrame &kf) : QObject(kf.parent()), m_name(kf.m_name), m_isLocked(kf.m_isLocked)
// {
// 	kf.m_components.count() << endl;
// 	m_components += kf.m_components;
// }

KTKeyFrame::~KTKeyFrame()
{
	clear(true);
}

void KTKeyFrame::setComponents(const QList<AGraphicComponent *> &components)
{
	clear();
	m_components = components;
}

void KTKeyFrame::addComponent(AGraphicComponent *comp) 
{
	m_components << comp;
}

void KTKeyFrame::insertComponent(int pos, AGraphicComponent *comp)
{
	m_components.insert(pos, comp);
}

void KTKeyFrame::removeComponent(AGraphicComponent *comp)
{
	deSelectedComponent( comp);
        m_components.removeAll(comp);
	delete comp;
}


AGraphicComponent *KTKeyFrame::takeLastComponent()
{
	if ( m_components.count() == 0 )
	{
		return 0;
	}
	
	return m_components.takeLast();
}

QList<AGraphicComponent *> KTKeyFrame::components() const
{
	return m_components;
}


void KTKeyFrame::setFrameName(const QString &name)
{
	m_name = name;
}

void KTKeyFrame::setLocked(bool isLocked)
{
	m_isLocked = isLocked;
}

QString KTKeyFrame::frameName() const
{
	return m_name;
}

bool KTKeyFrame::isLocked()
{
	return m_isLocked;
}

QDomElement KTKeyFrame::createXML( QDomDocument &doc )
{
	QDomElement frame = doc.createElement("Frame");
	frame.setAttribute ( "name", m_name);
	frame.setAttribute ( "nClones", m_nClones);
	
	QList<AGraphicComponent *>::ConstIterator iterator = m_components.begin();
	
	while( iterator != m_components.end() )
	{
		frame.appendChild((*iterator)->createXML(doc));
		
		++iterator;
	}
	
	return frame;
}

void KTKeyFrame::addSelectedComponent(AGraphicComponent *toSelect)
{
	if(toSelect && !m_selectedComponents.contains(toSelect))
	{
		m_selectedComponents << toSelect;
	}
}

void KTKeyFrame::deSelectedComponent(AGraphicComponent *toDeSelect)
{
	toDeSelect->removeControlPoints();
	m_selectedComponents.removeAll ( toDeSelect );
}

void KTKeyFrame::clearSelections()
{
	foreach( AGraphicComponent *component, m_selectedComponents)
	{
		deSelectedComponent(component);
	}
}

QList<AGraphicComponent *> KTKeyFrame::selectedComponents()
{
	return m_selectedComponents;
}

void KTKeyFrame::scale(int sX, int sY)
{
	foreach(AGraphicComponent *comp, m_components)
	{
		comp->scale( sX, sY);
	}
}

void KTKeyFrame::addComponents(QList<AGraphicComponent *> comps)
{
	m_components += comps;
}

void KTKeyFrame::removeSelections()
{
	foreach(AGraphicComponent *comp, m_selectedComponents)
	{
		deSelectedComponent(comp);
		m_components.removeAll(comp);
		delete comp;
		comp=0;
	}
}

void KTKeyFrame::selecteAllComponents()
{
	m_selectedComponents = m_components;
}

void KTKeyFrame::selectContains (const QRect & rect)
{
	clearSelections();
	foreach(AGraphicComponent *comp, m_components)
	{
		if(rect.intersects(comp->boundingRect().toRect().adjusted(1,0,0,-1)))
		{
			m_selectedComponents << comp;
		}
	}
}

bool KTKeyFrame::hasSelections() const
{
	return !m_selectedComponents.isEmpty();
}

void KTKeyFrame::sendToBackSelected()
{
	if(m_selectedComponents.count() == 1)
	{
		m_components.removeAll ( m_selectedComponents[0] );
		m_components.push_front( m_selectedComponents[0] );
	}
}

void KTKeyFrame::bringToFromSelected()
{
	if(m_selectedComponents.count() == 1)
	{
		m_components.removeAll ( m_selectedComponents[0] );
		m_components.push_back( m_selectedComponents[0] );
	}
}

void KTKeyFrame::oneStepForwardSelected()
{
	if(m_selectedComponents.count() == 1  )
	{
		if(! (m_selectedComponents[0] == m_components.last()))
		{
			int index = m_components.indexOf(m_selectedComponents[0]);
			if(index != -1 )
			{
				m_components.swap(index, index +1 );
			} 
		}
	}
}
void KTKeyFrame::oneStepBackwardSelected()
{
	if(m_selectedComponents.count() == 1  )
	{
		if(! (m_selectedComponents[0] == m_components.first()))
		{
			int index = m_components.indexOf(m_selectedComponents[0]);
			if(index != -1 )
			{
				m_components.swap(index, index -1 );
			}
		}
	}
}	


void KTKeyFrame::replace(AGraphicComponent *orig, AGraphicComponent *newComponent)
{
	int index = m_components.indexOf(orig);
	
	if( index >= 0)
	{
		m_components.replace(index, newComponent);
	}
}


void KTKeyFrame::setClonesNumber(int nClones)
{
	m_nClones = nClones;
}

void KTKeyFrame::clear(bool alsoDelete )
{
	if ( alsoDelete )
	{
		qDeleteAll(m_components.begin(), m_components.end());
	}
	m_components.clear();
}

int KTKeyFrame::clonesNumber()
{
	return m_nClones;
}



