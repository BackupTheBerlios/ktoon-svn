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
		APaintArea(QWidget *parent = 0);
		~APaintArea();
		QSize sizeHint() const;
		QSize minimumSizeHint () const;
		QPoint paintDevicePosition() const;
		QImage paintDevice() const;
		void setPaintDevice(const QImage &image);
		void setZeroAt(int zero);
		
		virtual void redrawAll();
		
	public slots:
		void setKeyFrame(int index);
		void setLayer(int index);
		void setScene(KTScene *scene);
		
	private:
		QImage m_paintDevice;
		AGrid m_grid;
		QPainterPath m_path;
		int m_xpos, m_ypos;
		int m_zero;
		bool m_drawGrid;
		
		// <FIXME>
		KTBrush *m_currentBrush;
		AToolInterface *m_currentTool;
		QString m_currentKeyTool;
		
		QPoint m_lastPosition;
		
		QColor m_brushColor, m_penColor; // TODO: move to KTBrush
		
		QList<AGraphicComponent *> m_undoComponents;
		
		AGraphicComponent *m_currentGraphic;
		bool m_redrawAll;

		KTKeyFrame *m_currentFrame;
		KTLayer *m_layer;
		KTScene *m_scene;

	public:
		void setTool( AToolInterface *toolIface, const QString &tool);
		QPainterPath translatePath(const QPainterPath &path, const QPoint &pos);
		
	public slots:
		void setBrush( KTBrush *brush );
		void setPenColor( const QColor&); // TODO: move to KTBrush
		void setBrushColor( const QColor&); // TODO: move to KTBrush
		void undo();
		void redo();
		
	private:
		void setupPainter(QPainter &painter); // TODO: move to KTBrush
		
		// </FIXME>
	protected:
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent ( QMouseEvent * e );
		void mouseReleaseEvent(QMouseEvent *e);
		void paintEvent(QPaintEvent *);
		virtual void draw(QPainter *p);
		void resizeEvent(QResizeEvent * event );
		
		
		
	signals:
		void mousePos(const QPoint& p);
};

#endif
