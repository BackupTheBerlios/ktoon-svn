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

#include "ktpackagehandler.h"

#include "ddebug.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "dglobal.h"

KTPackageHandler::KTPackageHandler()
{
}


KTPackageHandler::~KTPackageHandler()
{
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
		dError() << "Error while create package: " << zip.getZipError();
		return false;
	}
	
	if ( ! compress(&zip, projectPath ))
	{
		dError() << "Error while compress project" << zip.getZipError();
		return false;
	}
	
	zip.close();
	if(zip.getZipError() != 0)
	{
		dError() << "Error: " << zip.getZipError();
		return false;
	}
	
	return true;
}


bool KTPackageHandler::compress(QuaZip *zip, const QString &path)
{
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
		
		inFile.setFileName(filePath);
		if(!inFile.open(QIODevice::ReadOnly)) 
		{
			dError() << "Error opening file " << inFile.fileName() << " : " << inFile.errorString();
			return false;
		}
		
		if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(stripRepositoryFromPath(filePath), stripRepositoryFromPath(filePath) ))) 
		{
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
	path.remove(REPOSITORY);
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
		dDebug() << "Error while open package: " << zip.getZipError();
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
			dError() << "Can't get current file: " << zip.getZipError();
			return false;
		}
		
		if(!file.open(QIODevice::ReadOnly)) 
		{
			dError() << "Can't open file "<< file.getZipError();
			return false;
		}
		
		name = REPOSITORY+"/"+file.getActualFileName();
		
		if( name.endsWith(".ktp") )
		{
			m_importedProjectPath = name;
		}
		
		if(file.getZipError()!=UNZ_OK)
		{
			dError() << "Error while open package " << file.getZipError();
			return false;
		}
		
		createPath( name );
		out.setFileName(name);
		
		if ( ! out.open(QIODevice::WriteOnly) )
		{
			dError() << "Error while open file: " << out.fileName() << ", error was: " << out.errorString();
		}
		
		while(file.getChar(&c)) out.putChar(c);
		out.close();
		if(file.getZipError()!=UNZ_OK)
		{
			dError() << "Error while open package " << file.getZipError();
			return false;
		}
		if(!file.atEnd()) 
		{
			dError() << "Not EOF";
			return false;
		}
		file.close();
		if(file.getZipError()!=UNZ_OK) 
		{
			dError() << "Error while open package " << file.getZipError();
			return false;
		}
			
		next = zip.goToNextFile();
	}
	
	zip.close();
	if(zip.getZipError() != UNZ_OK)
	{
		dError() << "Error while open package " << file.getZipError();
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
	return m_importedProjectPath;
}

