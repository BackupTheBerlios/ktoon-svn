//
// C++ Interface: apaintarea
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef APAINTAREA_H
#define APAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include <QPainterPath>
#include "agrid.h"

/**
	@author David Cuadrado <krawek@toonka.com>
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
		void setZeroAt(int zero);
		
		
	private:
		QImage m_paintDevice;
		AGrid m_grid;
		QPainterPath m_path;
		int m_xpos, m_ypos;
		int m_zero;
		bool m_drawGrid;
		
	protected:
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent ( QMouseEvent * e );
		void paintEvent(QPaintEvent *);
		void resizeEvent(QResizeEvent * event );
		
	signals:
		void mousePos(const QPoint& p);
};

#endif
