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

#ifndef KTPACKAGEHANDLER_H
#define KTPACKAGEHANDLER_H

#include <QString>

class QuaZip;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTPackageHandler
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
		QString m_importedProjectPath;
};

#endif
