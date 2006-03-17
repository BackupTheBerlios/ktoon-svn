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
#ifndef ADRAWCOMMAND_H
#define ADRAWCOMMAND_H

#include <dcommand.h>

#include "agraphiccomponent.h"
#include "ktkeyframe.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class AAddGraphicCommand : public DCommand
{
	public:
		AAddGraphicCommand(KTKeyFrame *frame, AGraphicComponent *added );
		~AAddGraphicCommand();
		
		void execute();
		void unexecute();
		QString name() const;
		
	private:
		AGraphicComponent *m_added;
		KTKeyFrame *m_frame;
};


class ADrawCommand : public DCommand
{
	public:
		ADrawCommand(KTKeyFrame *frame);
		~ADrawCommand();
		
		void execute();
		void unexecute();
		QString name() const;
		
	private:
		QList<AGraphicComponent *> m_copies;
		QList<AGraphicComponent *> m_originals;
		KTKeyFrame *m_frame;
};

#endif
