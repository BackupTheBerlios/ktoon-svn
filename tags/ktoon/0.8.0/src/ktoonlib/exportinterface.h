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

#ifndef EXPORTINTERFACE_H
#define EXPORTINTERFACE_H

#include <QStringList>
#include <QDir>

#include "ktexportpluginobject.h"

#include "ktscene.h"

#include "qplugin.h" // Q_EXPORT_PLUGIN

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class ExportInterface
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
		
		Q_DECLARE_FLAGS(Formats, Format);
		
		
		virtual ~ExportInterface() {};
		virtual QString key() const = 0;
		virtual Formats availableFormats() = 0;
		
		virtual void exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format, const QSize &size, float sx = 1, float sy = 1) = 0;
		
	private:
		virtual QStringList createImages(const QList<KTScene *> &scenes, const QDir &dir, float sx = 1, float sy = 1, const char *format = "PNG") = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ExportInterface::Formats);

Q_DECLARE_INTERFACE( ExportInterface, "com.toonka.ktoon.ExportInterface/0.1" );

#endif
