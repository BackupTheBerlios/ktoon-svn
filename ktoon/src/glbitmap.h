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

#ifndef GLBITMAP_H
#define GLBITMAP_H

/*!
 * \file glbitmap.h
 * \brief Include this file if you need the class GLBitmap
 */

#include <GL/gl.h>
#include <qgl.h>

#include "glgraphiccomponent.h"
#include "color.h"

/*!
 * \brief Class that represents the drawing area's bitmap object
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class is intended to represent an irregular fill object.
 */
class GLBitmap : public GLGraphicComponent
{

public:
        GLBitmap( QGLWidget *parent, const QPoint & _origin, const Color & _color, const Brush & _width, const QPoint & _end );
	~GLBitmap();

	//Set methods

	//Get methods

private:
 	QPoint end;
	void buildList();
};

#endif
