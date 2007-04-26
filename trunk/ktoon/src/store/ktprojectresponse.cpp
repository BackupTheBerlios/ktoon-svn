/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktprojectresponse.h"

class KTProjectResponse::Private
{
	public:
		Private(int part, int action) : part(part), action(action), isExternal(false) {}
		
		int part;
		int action;
		KTProjectRequestArgument arg;
		QByteArray data;
		Mode mode;
		
		bool isExternal;
};

KTProjectResponse::KTProjectResponse(int part, int action) : d(new Private(part, action))
{
}


KTProjectResponse::~KTProjectResponse()
{
	delete d;
}


int KTProjectResponse::part() const
{
	return d->part;
}

int KTProjectResponse::action() const
{
	if( d->mode == Undo )
	{
		switch(d->action)
		{
			case KTProjectRequest::Add:
			{
				return KTProjectRequest::Remove;
			}
			break;
			case KTProjectRequest::AddSymbolToProject:
			{
			}
			break;
			case KTProjectRequest::EditNodes:
			{
			}
			break;
			case KTProjectRequest::View:
			{
			}
			break;
			case KTProjectRequest::Select:
			{
			}
			break;
			case KTProjectRequest::Transform:
			{
			}
			break;
			case KTProjectRequest::Tweening:
			{
			}
			break;
			case KTProjectRequest::Lock:
			{
			}
			break;
			case KTProjectRequest::Ungroup:
			{
				return KTProjectRequest::Group;
			}
			break;
			case KTProjectRequest::Rename:
			{
			}
			break;
			case KTProjectRequest::Move:
			{
			}
			break;
			case KTProjectRequest::Convert:
			{
			}
			break;
			case KTProjectRequest::Remove:
			{
				return KTProjectRequest::Add;
			}
			break;
			case KTProjectRequest::Group:
			{
				return KTProjectRequest::Ungroup;
			}
			break;
			default:
			{
				qFatal("Unhandled action");
			}
			break;
		}
	}
	
	return d->action;
}

int KTProjectResponse::originalAction() const
{
	return d->action;
}

void KTProjectResponse::setMode(Mode mode)
{
	d->mode = mode;
}

void KTProjectResponse::setExternal(bool e)
{
	d->isExternal = e;
}

bool KTProjectResponse::external() const
{
	return d->isExternal;
}

KTProjectResponse::Mode KTProjectResponse::mode() const
{
	return d->mode;
}

void KTProjectResponse::setArg(const QString &value)
{
	d->arg = value;
}

void KTProjectResponse::setData(const QByteArray &data)
{
	d->data = data;
}

KTProjectRequestArgument KTProjectResponse::arg() const
{
	return d->arg;
}

QByteArray KTProjectResponse::data() const
{
	return d->data;
}




// SCENE

KTSceneResponse::KTSceneResponse(int part, int action) : KTProjectResponse(part, action), m_sceneIndex(-1)
{
}

KTSceneResponse::~KTSceneResponse()
{
}

int KTSceneResponse::sceneIndex() const
{
	return m_sceneIndex;
}

void KTSceneResponse::setSceneIndex(int index)
{
	m_sceneIndex = index;
}

void KTSceneResponse::setState(const QString &state)
{
	m_state = state;
}

QString KTSceneResponse::state() const
{
	return m_state;
}

// LAYER

KTLayerResponse::KTLayerResponse(int part, int action) : KTSceneResponse(part, action), m_layerIndex(-1)
{
}

KTLayerResponse::~KTLayerResponse()
{
}

int KTLayerResponse::layerIndex() const
{
	return m_layerIndex;
}

void KTLayerResponse::setLayerIndex(int index)
{
	m_layerIndex = index;
}


// FRAME

KTFrameResponse::KTFrameResponse(int part, int action) : KTLayerResponse(part, action), m_frameIndex(-1)
{
}

KTFrameResponse::~KTFrameResponse()
{
}

int KTFrameResponse::frameIndex() const
{
	return m_frameIndex;
}

void KTFrameResponse::setFrameIndex(int index)
{
	m_frameIndex = index;
}


// ITEM

KTItemResponse::KTItemResponse(int part, int action) : KTFrameResponse(part, action), m_itemIndex(-1)
{
}

KTItemResponse::~KTItemResponse()
{
}

int KTItemResponse::itemIndex() const
{
	return m_itemIndex;
}

void KTItemResponse::setItemIndex(int index)
{
	m_itemIndex = index;
}

KTLibraryResponse::KTLibraryResponse(int part, int action) : KTFrameResponse(part, action), m_symbolType(-1)
{
}

KTLibraryResponse::~KTLibraryResponse()
{
}

void KTLibraryResponse::setSymbolType(int symtype)
{
	m_symbolType = symtype;
}

int KTLibraryResponse::symbolType() const
{
	return m_symbolType;
}


KTProjectResponseFactory::KTProjectResponseFactory()
{
}

KTProjectResponseFactory::~KTProjectResponseFactory()
{
}

KTProjectResponse *KTProjectResponseFactory::create(int part, int action)
{
	switch(part)
	{
		case KTProjectRequest::Scene:
		{
			return new KTSceneResponse(part, action);
		}
		break;
		case KTProjectRequest::Layer:
		{
			return new KTLayerResponse(part, action);
		}
		break;
		case KTProjectRequest::Frame:
		{
			return new KTFrameResponse(part, action);
		}
		break;
		case KTProjectRequest::Item:
		{
			return new KTItemResponse(part, action);
		}
		break;
		case KTProjectRequest::Library:
		{
			return new KTLibraryResponse(part, action);
		}
		break;
		default:
		{
			qFatal("Unknown PART"); // TODO: REMOVE ME
		}
		break;
	}
	
	return new KTProjectResponse(part, action);
}






