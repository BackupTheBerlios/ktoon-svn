/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#ifndef KTXMLREADER_H
#define KTXMLREADER_H

#include <qxml.h>
#include "ktxmlparser.h"

/**
@author David Cuadrado
*/
class KTXmlReader : public QXmlSimpleReader
{
	public:
    		KTXmlReader();
    		~KTXmlReader();

		bool parseXml(const QXmlInputSource * input);
		
		XMLSingleResult getResult();
		XMLTotalResults getResults();

	private:
		KTXmlParser *m_parser;

};

#endif
