/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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
#ifndef KTSOUNDLAYER_H
#define KTSOUNDLAYER_H

#include <ktlayer.h>

/**
	@author David Cuadrado <krawek@gmail.com>
*/
class KTSoundLayer : public KTLayer
{
	public:
		KTSoundLayer(KTScene *parent);
		~KTSoundLayer();
		
		void fromSymbol(const QString &symbolName);
		QString filePath() const;
		
		void play();
		void stop();
		
		void fromXml(const QString &xml );
		QDomElement toXml(QDomDocument &doc) const;
		
	private:
		struct Private;
		Private *const d;

};

#endif
