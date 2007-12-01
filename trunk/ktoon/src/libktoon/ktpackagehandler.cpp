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

#include "ktpackagehandler.h"

#include <kcore/kdebug.h>

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "kcore/kglobal.h"
#include "kcore/kapplicationproperties.h"

#include "quazip.h"
#include "quazipfile.h"

struct KTPackageHandler::Private
{
	QString importedProjectPath;
};

KTPackageHandler::KTPackageHandler() : k(new Private)
{
}


KTPackageHandler::~KTPackageHandler()
{
	delete k;
}

bool KTPackageHandler::makePackage(const QString &projectPath, const QString &packagePath)
{
	if ( !QFile::exists(projectPath))
	{
		qWarning("Project not saved!");
		
		return false;
	}
	
	QFileInfo packageInfo(packagePath);
	
	QuaZip zip(packagePath);
	if(!zip.open(QuaZip::mdCreate))
	{
		kError() << "Error while create package: " << zip.getZipError();
		return false;
	}
	
	if ( ! compress(&zip, projectPath ))
	{
		kError() << "Error while compress project" << zip.getZipError();
		return false;
	}
	
	zip.close();
	if(zip.getZipError() != 0)
	{
		kError() << "Error: " << zip.getZipError();
		return false;
	}
	
	return true;
}


bool KTPackageHandler::compress(QuaZip *zip, const QString &path)
{
	kDebug() << "COMPRESSING: " << path;
	QFile inFile;
	QuaZipFile outFile(zip);
	char c;
	
	QFileInfoList files= QDir(path).entryInfoList();
	
	foreach(QFileInfo file, files)
	{
		QString filePath = path+"/"+file.fileName();
		
		if ( file.fileName().startsWith(".") ) continue;
		
		if ( file.isDir() )
		{
			compress(zip, file.path()+"/"+file.fileName());
			continue;
		}
		
		if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(stripRepositoryFromPath(filePath), stripRepositoryFromPath(filePath) ))) 
		{
			return false;
		}
		
		inFile.setFileName(filePath);
		if(!inFile.open(QIODevice::ReadOnly))
		{
			kError() << "Error opening file " << inFile.fileName() << " : " << inFile.errorString();
			return false;
		}
		while(inFile.getChar(&c) && outFile.putChar(c));
		
		if(outFile.getZipError()!=UNZ_OK)
		{
			return false;
		}
		outFile.close();
		if(outFile.getZipError()!=UNZ_OK)
		{
			return false;
		}
		inFile.close();
	}
	
	return true;
}

QString KTPackageHandler::stripRepositoryFromPath(QString path)
{
	path.remove(CACHE_DIR);
	if ( path[0] == QDir::separator() )
	{
		path.remove(0, 1);
	}
	return path;
}

bool KTPackageHandler::importPackage(const QString &packagePath)
{
	QuaZip zip(packagePath);
	
	if(!zip.open(QuaZip::mdUnzip))
	{
		kDebug() << "Error while open package: " << zip.getZipError();
		return false;
	}
	zip.setFileNameCodec("IBM866"); // ###: ?
	
	QuaZipFile file(&zip);
	QFile out;
	QString name;
	char c;
	
	QuaZipFileInfo info;

	bool next = zip.goToFirstFile();
	while( next )
	{
		if(!zip.getCurrentFileInfo(&info))
		{
			kError() << "Can't get current file: " << zip.getZipError();
			return false;
		}
		
		if(!file.open(QIODevice::ReadOnly)) 
		{
			kError() << "Can't open file "<< file.getZipError();
			return false;
		}
		
		name = CACHE_DIR+"/"+file.getActualFileName();
		if( name.endsWith( QDir::separator() ) )
		{
			name.remove(name.count()-1, 1);
		}
		
// 		if( name.endsWith(".ktp") )
		{
			k->importedProjectPath = QFileInfo(name).path();
		}
		
		if(file.getZipError() != UNZ_OK)
		{
			kError() << "Error while open package " << file.getZipError();
			return false;
		}
		
		createPath( name );
		out.setFileName(name);
		
		if ( ! out.open(QIODevice::WriteOnly) )
		{
			kError() << "Error while open file: " << out.fileName() << ", error was: " << out.errorString();
		}
		
		while(file.getChar(&c)) out.putChar(c);
		out.close();
		if(file.getZipError()!=UNZ_OK)
		{
			kError() << "Error while open package " << file.getZipError();
			return false;
		}
		if(!file.atEnd()) 
		{
			kError() << "Not EOF";
			return false;
		}
		file.close();
		if(file.getZipError()!=UNZ_OK) 
		{
			kError() << "Error while open package " << file.getZipError();
			return false;
		}
			
		next = zip.goToNextFile();
	}
	
	zip.close();
	if(zip.getZipError() != UNZ_OK)
	{
		kError() << "Error while open package " << file.getZipError();
		return false;
	}
	
	return true;
}

bool KTPackageHandler::createPath(const QString &filePath)
{
	QFileInfo info(filePath);
	QDir path = info.dir();
	
	if ( !path.exists() )
	{
		return path.mkpath(path.path());
	}
	
	return false;
}


QString KTPackageHandler::importedProjectPath() const
{
	return k->importedProjectPath;
}

