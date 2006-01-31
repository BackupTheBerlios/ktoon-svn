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

#ifndef KTLAYERSEQUENCE_H
#define KTLAYERSEQUENCE_H

#include "kttimelinelayer.h"

#include "ktwidgetlistview.h"

#include <QResizeEvent>
#include "ktvhbox.h"

typedef QList<KTTimeLineLayer *>  ListOfTLLayers;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLayerSequence : public KTWidgetListView
{
	Q_OBJECT
	public:
		KTLayerSequence(QWidget *parent);
		~KTLayerSequence();
		ListOfTLLayers layers();
		void setPalette(const QPalette &);
		
	private slots:
		void displayMenu(KTTimeLineLayer *ly,const QPoint &pos);
		
	public slots:
		KTTimeLineLayer *createNewLayer(const QString &name, bool toEnd = true);
		void removeLayer();
		
		void selectLayer(KTTimeLineLayer *);
		
		void moveLayerUp();
		void moveLayerDown();
		
	private:
		KTTimeLineLayer *m_defaultLayer;
		ListOfTLLayers m_layers;
		int m_layerCount;
	
	protected:
// 		void resizeEvent(QResizeEvent *e);

};

#endif
