/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#include "scene.h"

//-------------- CONSTRUCTOR ---------------

Scene::Scene()
{
	m_name = "";
//     Layer *def_layer = new Layer();
//     def_layer -> setIndexLayer( 1 );
//     def_layer -> setNameLayer( QObject::tr( "Layer" ) + QString( "1" ) );
//     layers.append( def_layer );
}

//------------- DESTRUCTOR ------------------

Scene::~Scene()
{
//     layers.setAutoDelete( true );
	qDeleteAll(m_layers);
//     layers.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void Scene::setNameScene( const QString & name )
{
	m_name = name;
}

void Scene::setLayers( QList<Layer*> layers )
{
	m_layers = layers;
}

QString Scene::nameScene() const
{
	return m_name;
}

QList<Layer*> Scene::getLayers()
{
	return m_layers;
}

QDomElement Scene::createXML( QDomDocument &doc )
{
	QDomElement e = doc.createElement( "Scene" );

	e.setAttribute( "Name", m_name );

	QDomElement lTag = doc.createElement( "Layers" );
	e.appendChild( lTag );
	
	for(int i = 0; i < m_layers.count() ; i++)
	{
		Layer *lIt = m_layers[i];
		QDomElement layerTag = lIt -> createXML( doc );
		lTag.appendChild( layerTag );
	}
	return e;
}
