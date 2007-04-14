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

#ifndef DMOVIEGENERATORINTERFACE_H
#define DMOVIEGENERATORINTERFACE_H

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class DMovieGeneratorInterface
{
	public:
		virtual ~DMovieGeneratorInterface() {}
		
		virtual void nextFrame() = 0;
		virtual void reset() = 0;
		
		void saveMovie(const QString &filename)
		{
			end();
			__saveMovie(filename);
		};
		
	protected:
		virtual void __saveMovie(const QString &filename) = 0;
		virtual bool begin() = 0;
		virtual void end() = 0;
};

#endif


