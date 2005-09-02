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

#ifndef KTFRAMESEQUENCE_H
#define KTFRAMESEQUENCE_H

#include <qwidget.h>
#include <qhbox.h>

#include "tlframe.h"

typedef QPtrList<TLFrame> ListOfTimeLineFrames;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTFrameSequence : public QHBox
{
	Q_OBJECT
	public:
		KTFrameSequence(QWidget *parent);
		~KTFrameSequence();
		
	public slots:
		void selectFrame(int position);
		
	private:
		ListOfTimeLineFrames m_frames;

	private:
		void createFrames();

};

#endif