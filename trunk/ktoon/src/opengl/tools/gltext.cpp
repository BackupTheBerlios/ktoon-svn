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

#include "gltext.h"
#include <qgl.h>

GLText::GLText(const QString &text, QGLWidget *parent, const QPoint & origin, const KTColor & color, const Brush & width) : GLGraphicComponent(parent, origin, color, width), m_text(text)
{
	setKindGraphic( GC_TEXT );
	id_graphic_component = glGenLists( 1 );
	stipple_factor = 1;
	stipple_pattern = 0xFFFF;
	z = 0.0;
	rotation_angle = 0.0;
	translate.setX( 0 );
	translate.setY( 0 );
	local_selection_name = selection_name;
	buildList();
	selection_name++;
	
	setZ(0);
}


GLText::~GLText()
{
}

void GLText::buildList()
{
	glNewList( id_graphic_component, GL_COMPILE );
#if 0
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
	glRasterPos3d(100, 100, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glCallLists(m_text.length(), GL_UNSIGNED_BYTE, m_text.utf8());
	glPopAttrib();
#endif
	glEndList();
}

void GLText::calculateTopLeft()
{
}

void GLText::calculateBottomRight()
{
}

QString GLText::clipboardGraphic()
{
}

QDomElement GLText::createXML(QDomDocument &)
{
}
