/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#include "cliphelper.h"

#include "qpathclipper_p.h"


QPainterPath ClipHelper::unite(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return path1.isEmpty() ? path2 : path1;
	
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolOr);
}

QPainterPath ClipHelper::intersect(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return QPainterPath();
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolAnd);
}

QPainterPath ClipHelper::subtract(const QPainterPath &path1, const QPainterPath &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return path1;
	QPathClipper clipper(path1, path2);
	return clipper.clip(QPathClipper::BoolSub);
}

