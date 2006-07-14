/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#ifndef KTEXPOSURETABLE_H
#define KTEXPOSURETABLE_H

#include<QTableWidget>
#include <QList>

class KTExposureHeader;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTExposureTable : public QTableWidget
{
	Q_OBJECT;
	
	friend class KTExposureItemDelegate;
	
	public:
		enum Attribute
		{
			IsUsed = 1000,
			IsLocked
		};
		
		KTExposureTable(QWidget * parent = 0);
		~KTExposureTable();
		
		int currentLayer() const;
		int currentFrame() const;
		void insertLayer(int index, const QString & name);
		void setUseFrame(int indexLayer, int indexFrame, const QString & name);
		void removeLayer(int indexLayer );
		void removeFrame(int indexLayer, int indexFrame);
		void moveFrame( int oldPosLayer, int oldPosFrame, int newPosLayer, int newPosFrame );
		void moveLayer( int oldPosLayer, int newPosLayer );
		
		void setLockFrame(int indexLayer, int indexFrame, bool locked);
		int numUsed() const;
		
		void setName(int indexLayer, int indexFrame,const QString & name);
		void setLayerName(int indexLayer, const QString & name);
		
		bool frameIsLocked(int indexLayer, int indexFrame);
		
	private:
		KTExposureHeader *m_header;
		
	private slots:
		void emitRequestSetUsedFrame(int indexFrame,  int indexLayer);
		void emitRequestRenameFrame( QTableWidgetItem * item );
		void emitRequestMoveLayer( int logicalIndex, int oldVisualIndex, int newVisualIndex  );
		
	protected:
		bool edit ( const QModelIndex & index, EditTrigger trigger, QEvent * event );
		
	signals:
		void requestSetUsedFrame(int indexLayer, int indexFrame);
		void requestRenameFrame(int indexLayer, int indexFrame,const QString & name);
		void requestRenameLayer(int indexLayer, const QString & name);
		void requestMoveLayer( int oldIndex, int newIndex  );
};

#endif
