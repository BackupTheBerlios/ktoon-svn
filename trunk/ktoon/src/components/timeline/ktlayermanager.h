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

#ifndef KTLAYERMANAGER_H
#define KTLAYERMANAGER_H

#include <QPushButton>
#include <QToolTip>
#include <QLabel>
#include <QScrollBar>
#include <QButtonGroup>

#include <dgui/dimagebutton.h>

#include <QTableWidget>


/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLayerManager : public QTableWidget
{
	Q_OBJECT;
	
	friend class KTLayerManagerItemDelegate;
	
	public:
		enum Actions
		{
			NoAction = 0,
			ShowOutlines,
			LockLayers,
			ToggleLayerView,
			InsertLayer,
			RemoveLayer,
			MoveLayerUp,
			MoveLayerDown
		};
		
		/**
		 * Default constructor
		 * @param parent 
		 * @return 
		 */
		KTLayerManager(QWidget *parent = 0);
		~KTLayerManager();
		
		void insertLayer(int position, const QString &name);
		void insertSoundLayer(int position, const QString &name);
		void removeLayer(int position);
		void renameLayer(int position, const QString &name);
		
		void moveLayer(int position, int newPosition);
		
		void lockLayer(int position, bool locked);
		
		void setRowHeight(int rowHeight);
		
	protected:
		void resizeEvent(QResizeEvent *e);
		virtual void fixSize();
		
	protected slots:
		void commitData ( QWidget * editor );
		
	signals:
		void requestRenameEvent(int layerPosition, const QString &newName);
		
	private:
		struct Private;
		Private *const d;
};

#endif
