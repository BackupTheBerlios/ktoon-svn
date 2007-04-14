/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef DMOVIEGENERATOR_H
#define DMOVIEGENERATOR_H

#include <QImage>

#include "dgui/dmoviegeneratorinterface.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class DMovieGenerator : public QImage, public DMovieGeneratorInterface
{
	public:
		enum Format
		{
			NONE = 0,
			SWF = 1<<0,
			MPEG = 1<<1,
			AVI = 1<<2,
			RM = 1 << 3,
			ASF = 1 << 5,
			MOV = 1 << 6,
			GIF = 1 << 7,
			PNG = 1 << 8,
			JPEG = 1 << 9,
			SMIL = 1 << 10
		};
		DMovieGenerator( int width, int height);
		virtual ~DMovieGenerator();
		
		void nextFrame();
		virtual void reset();
		
	protected:
		virtual bool begin();
		virtual void end();
		
		virtual void handle(const QImage &image) = 0;
		
};

#endif


