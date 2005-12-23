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

#ifndef APAINTAREA_H
#define APAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include <QPainterPath>
#include <QList>

#include "agrid.h"

#include "afilterinterface.h"
#include "atoolinterface.h"

#include "ktscene.h"

#include "ktbrush.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class APaintArea : public QWidget
{
	Q_OBJECT
	public:
		APaintArea(const QSize& size, QWidget *parent = 0);
		~APaintArea();
		QSize sizeHint() const;
		QSize minimumSizeHint () const;
		QPoint paintDevicePosition() const;
		QImage paintDevice() const;
		void setPaintDevice(const QImage &image);
		void setZeroAt(const QPoint& zero);
		
		virtual void redrawAll();
		
		AGraphicComponent *currentGraphic();
		AGraphicComponent *selectedGraphic();
		
		KTBrush *currentBrush();
		
		void drawSelected(QPainter *painter);
		
	public slots:
		void aUpdate(const QRectF &rect);
		void setKeyFrame(int index);
		void setLayer(int index);
		void setScene(KTScene *scene);
		
		void setPreviousFrames(int n);
		void setNextFrames(int n);
		
	private:
		QImage m_paintDevice;
		AGrid m_grid;
		QPainterPath m_path;
		int m_xpos, m_ypos;
		QPoint m_zero;
		bool m_drawGrid;
		
		// <FIXME>
		KTBrush *m_currentBrush;
		AToolInterface *m_currentTool;
		QString m_currentKeyTool;
		
		QPoint m_lastPosition;
		
		QList<AGraphicComponent *> m_undoComponents;
		
		AGraphicComponent *m_currentGraphic;
		AGraphicComponent *m_selectedGraphic;
		bool m_redrawAll; 

		KTKeyFrame *m_currentFrame;
		KTLayer *m_layer;
		KTScene *m_scene;
		
		int m_previousFramesNumber, m_nextFramesNumber;

	public:
		void setTool( AToolInterface *toolIface, const QString &tool);
		QPainterPath translatePath(const QPainterPath &path, const QPoint &pos);
		
	public slots:
		void setBrush( const KTBrush *brush );
		void undo();
		void redo();

		// </FIXME>
	protected:
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent ( QMouseEvent * e );
		void mouseReleaseEvent(QMouseEvent *e);
		void paintEvent(QPaintEvent *);
		virtual void draw(QPainter *p);
		virtual void drawFrame(const KTKeyFrame *frame, QPainter *painter, float intensitive = 1);
		virtual void drawGraphic(const AGraphicComponent *graphic, QPainter *painter, float intensitive = 1);
		void resizeEvent(QResizeEvent * event );
		
		
	signals:
		void mousePos(const QPoint& p);
};

#endif
