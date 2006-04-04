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

#include "adrawcommand.h"

#include <ddebug.h>

AAddGraphicCommand::AAddGraphicCommand(KTKeyFrame *frame, AGraphicComponent *added) : DCommand(), m_frame(frame), m_added(added)
{
}


AAddGraphicCommand::~AAddGraphicCommand()
{
}

void AAddGraphicCommand::execute()
{
	m_frame->addComponent( m_added );
}

void AAddGraphicCommand::unexecute()
{
	m_frame->takeLastComponent();
}

QString AAddGraphicCommand::name() const
{
	return "";
}

////////////*//*************************************//////////////////////


ADrawCommand::ADrawCommand(KTKeyFrame *frame) : DCommand(), m_frame(frame)
{
	foreach(AGraphicComponent *component, frame->components()) // FIXME: lots of memory 
	{
		m_originals << new AGraphicComponent(*component);
		m_copies << new AGraphicComponent(*component);
	}
}


ADrawCommand::~ADrawCommand()
{
}

void ADrawCommand::execute()
{
	for(int i = 0; i < m_originals.count(); i++ )
	{
		AGraphicComponent *component = m_originals[i];
		AGraphicComponent *copy = m_copies[i];
		
		if ( component )
		{
			AGraphicComponent *tmp = new AGraphicComponent(*component);
			
			component->copyAttributes( copy );
			
			copy->copyAttributes(tmp);
		}
	}
	m_frame->setComponents(m_originals);
}

void ADrawCommand::unexecute()
{
	for(int i = 0; i < m_copies.count(); i++ )
	{
		AGraphicComponent *component = m_originals[i];
		AGraphicComponent *copy = m_copies[i];
		
		if ( copy )
		{
			AGraphicComponent *tmp = new AGraphicComponent(*component);
			
			component->copyAttributes( copy );
			
			copy->copyAttributes(tmp);
		}
	}
	m_frame->setComponents(m_copies);
}

QString ADrawCommand::name() const
{
	return "";
}

