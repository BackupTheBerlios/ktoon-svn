/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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
#ifndef DFFMPEGMOVIEGENERATOR_H
#define DFFMPEGMOVIEGENERATOR_H

#include <dgui/dmoviegenerator.h>

/**
	@author David Cuadrado <krawek@gmail.com>
*/
class DFFMpegMovieGenerator : public DMovieGenerator
{
	public:
		DFFMpegMovieGenerator(Format format, int width, int height, int fps = 24);
		~DFFMpegMovieGenerator();
		
	protected:
		void __saveMovie(const QString &fileName);
		virtual void handle(const QImage& image);
		
		virtual bool begin();
		virtual void end();
		
	private:
		struct Private;
		Private *const d;
};

#endif
