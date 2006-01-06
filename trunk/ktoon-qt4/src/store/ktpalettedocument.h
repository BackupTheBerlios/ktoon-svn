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

#ifndef KTPALETTEDOCUMENT_H
#define KTPALETTEDOCUMENT_H

#include <QDomDocument>
#include <QBrush>
#include <QList>

/**
 * @author David Cuadrado
*/

class KTPaletteDocument : public QDomDocument
{
	public:
		KTPaletteDocument(const QString &name, bool isEditable);
		~KTPaletteDocument();
		void addGradient(const QGradient &gradient);
		void addColor(const QColor &color);
		void setElements(const QList<QBrush > &colors);
};

#endif
