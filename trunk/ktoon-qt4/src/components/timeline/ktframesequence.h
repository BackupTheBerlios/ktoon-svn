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

#include "tlframe.h"
#include "kttimelineframe.h"

#include "ktwidgetlistview.h"
#include "ktwidgetlistitem.h"

#include <QHBoxLayout>

typedef QList<KTTimeLineFrame *> ListOfTimeLineFrames;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTFrameSequence : public KTWidgetListItem
{
	Q_OBJECT
	public:
		KTFrameSequence(int ID, int count = 100,  KTWidgetListView *parent = 0);
		~KTFrameSequence();
		void setPosition(int pos);
		int position();
		KTTimeLineFrame *frameAt(int pos);
		
	signals:
		void frameSelected(KTTimeLineFrame *);
		
	public slots:
		void selectFrame(int position);
		
	private:
		ListOfTimeLineFrames m_frames;
		int m_ID;

	private:
		void createFrames(int count);
		QHBoxLayout *m_layout;
		
		int m_selectedFrameIndex;

};

#endif
