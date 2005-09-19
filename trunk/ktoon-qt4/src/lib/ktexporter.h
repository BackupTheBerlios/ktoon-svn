/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@toonka.com   						   *
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

#ifndef KTEXPORTER_H
#define KTEXPORTER_H

#include <qobject.h>

#ifdef USE_MING
#include "mingpp.h"
#endif

/**
 * This class is used for export animations to others formats like SVG, PNG, SWF, etc...
 * @author David Cuadrado
*/

class KTExporter : public QObject
{
	Q_OBJECT
	public:
		enum Format { PNG = 0, SWF, SVG, JPEG  };
		
		KTExporter(QObject *parent = 0, const char *name = 0);
		~KTExporter();
		
		bool exportAnimation(const QString &filename, Format format);
		
#ifdef USE_MING	
		SWFMovie *doMovie( int width, int height );
		SWFDisplayItem *doAnimation( SWFMovie *movie, const QStringList &images );
#endif

	private:
		QString m_filename;
};

#endif
