/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTPALETTEIMPORTER_H
#define KTPALETTEIMPORTER_H

#include <QDomDocument>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTPaletteImporter : QDomDocument
{
	public:
		enum PaletteType
		{
			Gimp = 0
		};
		
		KTPaletteImporter();
		~KTPaletteImporter();
		
		void import(const QString &file, PaletteType pt);
		void saveFile(const QString &path);
		QString filePath() const;
		
	private:
		void importGimpPalette(const QString &file);
		
	private:
		QString m_paletteName;
		QString m_filePath;
};

#endif
