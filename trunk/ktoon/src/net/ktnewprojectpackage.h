/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2007 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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


#ifndef KTNEWPROJECTPACKAGE_H
#define KTNEWPROJECTPACKAGE_H

#include <QDomDocument>

/**
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
*/
class KTNewProjectPackage : public QDomDocument
{
	public:
		KTNewProjectPackage(const QString & name, const QString & author, const QString & description );
		~KTNewProjectPackage();
		
		void setName(const QString & name);
		void setAuthor(const QString & author);
		void setDescription(const QString & description);
		
	private:
		struct Private;
		Private *const k;
		
};

#endif
