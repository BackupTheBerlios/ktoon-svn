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

#ifndef KTFRAMESEQUENCEMANAGER_H
#define KTFRAMESEQUENCEMANAGER_H

#include "ktvhbox.h"
//Added by qt3to4:
#include <QList>

#include <QScrollArea>

#include "kttlruler.h"
#include "ktframesequence.h"
#include "ktwidgetlistview.h"

typedef QList<KTFrameSequence *> ListOfFrameSequences;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTFrameSequenceManager : public KTWidgetListView
{
	Q_OBJECT
	public:
		KTFrameSequenceManager(QWidget *parent);
		~KTFrameSequenceManager();
		
	public slots:
		void insertFrameSequence();
		void removeFrameSequence();
		void setCurrentFrame(TLFrame *);
			
	protected:
// 		QSize sizeHint() const;
		
	private:
		ListOfFrameSequences m_sequences;
		
// 		KTVHBox *m_vBox;
		KTFrameSequence *m_lastSequence, *m_currentFrameSequence;

		QScrollBar *m_scroll;
		
		TLFrame *m_currentFrame;

};

#endif
