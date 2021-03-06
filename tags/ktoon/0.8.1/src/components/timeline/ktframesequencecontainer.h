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

#ifndef KTFRAMESEQUENCECONTAINER_H
#define KTFRAMESEQUENCECONTAINER_H

#include <dvhbox.h>

#include <QMap>

#include "tframestable.h"
#include "kttlruler.h"


/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTFrameSequenceContainer : public DVHBox
{
	Q_OBJECT
	public:
		KTFrameSequenceContainer(QWidget *parent = 0);
		~KTFrameSequenceContainer();
		
		TFramesTable const *manager();
		
		void addLayer();
		void addFrameToLayer(int layerId);
		void removeCurrentLayer();
		void removeLayer(int pos);
		
		void selectLayer(int pos);
		void selectCell(int layer, int frame);
		
		void moveCurrentLayer(bool up);
		
	private slots:
		void selectFrameFromItem(TFramesTableItem *item);
		void selectCurrentFrame();
		
	signals:
		void frameSelected(int layer, int frame);
		
	private:
		TFramesTable *m_frameTable;

};

#endif
