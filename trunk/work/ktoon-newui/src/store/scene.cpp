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
    name = "";
//     Layer *def_layer = new Layer();
//     def_layer -> setIndexLayer( 1 );
//     def_layer -> setNameLayer( QObject::tr( "Layer" ) + QString( "1" ) );
//     layers.append( def_layer );
}

//------------- DESTRUCTOR ------------------

Scene::~Scene()
{
    layers.setAutoDelete( true );
    layers.clear();
    layers.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void Scene::setNameScene( const QString & _name )
{
    name = _name;
}

void Scene::setLayers( QPtrList<Layer> _layers )
{
    layers = _layers;
}

QString Scene::nameScene() const
{
    return name;
}

QPtrList<Layer> Scene::getLayers()
{
    return layers;
}

QDomElement Scene::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Scene" );

    e.setAttribute( "Name", name );

    QDomElement l_tag = doc.createElement( "Layers" );
    e.appendChild( l_tag );
    Layer *l_it;
    for ( l_it = layers.first(); l_it; l_it = layers.next() )
    {
	QDomElement layer_tag = l_it -> createXML( doc );
	l_tag.appendChild( layer_tag );
    }

    return e;
}
