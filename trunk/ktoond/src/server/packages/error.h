/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#ifndef PACKAGESERROR_H
#define PACKAGESERROR_H

#include <QDomDocument>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
namespace Packages {

class Error : public QDomDocument
{
	public:
		enum Level{ None = -1, Info, Warning, Err, Fatal };
		Error(const QString &message, Level level );
		~Error();
		void setMessage(const QString &message);
		void setLevel(int level);
		
	private:
		QDomElement m_message;
		QDomText m_text;
};

}

#endif
