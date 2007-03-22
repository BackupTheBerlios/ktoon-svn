/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dalgorithm.h"

#include <cstdlib>
#include <cstdio>
#include <ctime>

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#endif

// DAlgorithm::DAlgorithm()
// {
// }
// 
// 
// DAlgorithm::~DAlgorithm()
// {
// }

int DAlgorithm::random()
{
	static bool init = false;
	if (!init)
	{
		unsigned int seed;
#ifdef Q_OS_LINUX
		init = true;
		int fd = open("/dev/urandom", O_RDONLY);
		if (fd < 0 || ::read(fd, &seed, sizeof(seed)) != sizeof(seed))
		{
			srand(getpid());
			seed = rand()+time(0);
		}
		if (fd >= 0) close(fd);
#else
		seed = ::time(0);
#endif
		srand(seed);
	}
	return rand();
}

QString DAlgorithm::randomString(int length)
{
	if (length <=0 ) return QString();

	QString str; str.resize( length );
	int i = 0;
	while (length--)
	{
		int r=random() % 62;
		r+=48;
		if (r>57) r+=7;
		if (r>90) r+=6;
		str[i++] =  char(r);
	}
	return str;
}

QColor DAlgorithm::randomColor(bool withAlpha)
{
	QColor c(random() % 255, random() % 255, random() % 255 );
	
	if ( withAlpha )
	{
		c.setAlpha(random() % 255);
	}
	
	return c;
}

