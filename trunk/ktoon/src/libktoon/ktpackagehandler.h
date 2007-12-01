/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTPACKAGEHANDLER_H
#define KTPACKAGEHANDLER_H

#include <QString>
#include "ktglobal.h"

class QuaZip;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTOON_EXPORT KTPackageHandler
{
	public:
		KTPackageHandler();
		~KTPackageHandler();
		
		bool makePackage(const QString &projectPath, const QString &packagePath);
		bool importPackage(const QString &packagePath);
		
		QString importedProjectPath() const;
		
	private:
		bool compress(QuaZip *zip, const QString &path);
		QString stripRepositoryFromPath(QString path);
		bool createPath(const QString &filePath);
		
	private:
		struct Private;
		Private *const k;
};

#endif
