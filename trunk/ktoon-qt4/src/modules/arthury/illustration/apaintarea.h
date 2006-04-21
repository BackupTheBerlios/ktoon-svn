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
#include "ktpaintareaproperties.h"

#include <dcommand.h>

#include "ktglobal.h"

// Devices
#include "aimagedevicewidget.h"
#include "agldevice.h"

#define IMAGE_DEVICE qobject_cast<AImageDeviceWidget *>(m_paintDevice)->device
#define OPENGL_DEVICE qobject_cast<AGLDevice *>(m_paintDevice)
#define NATIVE_DEVICE m_paintDevice

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class APaintArea : public QWidget
{
	Q_OBJECT
	public:
		APaintArea(const QSize& size, KToon::RenderType type,  QWidget *parent = 0);
		~APaintArea();
		QSize sizeHint() const;
		QSize minimumSizeHint () const;
		QPoint paintDevicePosition() const;
		QWidget *paintDevice() const;
		void setOffset(const QPoint& zero);
		
		AGraphicComponent *currentGraphic();
		KTKeyFrame *currentFrame() const;
		
		void setHistory(DCommandHistory *history);
		void addCommand(DCommand *command, bool execute = true);
		
	public slots:
		void redrawAll();
		void aUpdate(const QRect &rect);
		void drawGhostGraphic(const QPainterPath &path);
		void setKeyFrame(int index);
		void setLayer(int index);
		void setScene(KTScene *scene);
		
		void setPreviousFrames(int n);
		void setNextFrames(int n);
		
	private:
		KToon::RenderType m_renderType;
		QWidget *m_paintDevice;
		AGrid m_grid;
		QPainterPath m_path;
		int m_xpos, m_ypos;
		QPoint m_offset;
		bool m_drawGrid;
		
		// <FIXME>
		KTBrush *m_currentBrush;
		AToolInterface *m_currentTool;
		QString m_currentKeyTool;
		
		QPoint m_lastPosition;
		
		AGraphicComponent *m_currentGraphic;
		QList<AGraphicComponent *> m_copiedGraphics;
		bool m_redrawAll;

		KTKeyFrame *m_currentFrame;
		KTLayer *m_layer;
		KTScene *m_scene;
		
		int m_previousFramesNumber, m_nextFramesNumber;
		float m_zoomFactor;
		QImage m_overBuffer;
		QRect m_overBufferRect;
		
		QSize m_size;
		
		KTPaintAreaProperties m_properties;
		
		DCommandHistory *m_history;

	public:
		void setTool( AToolInterface *toolIface, const QString &tool);
		QPainterPath translatePath(const QPainterPath &path, const QPoint &pos);
		void importImage(const QPixmap &image);
		
		void setPen(const QPen &pen);
		void setColors(const QBrush &foreground, const QBrush &background);
		
		void setProperties(const KTPaintAreaProperties &properties);
		
	private:
		void loadProperties();
		
	public slots:
		void copy();
		void paste();
		void cut();
		void group();
		void ungroup();
		void removeSelectsGraphics();
		void selectAll();
		
		void bringToFromSelected();
		void sendToBackSelected();
		
		void oneStepForwardSelected();
		void oneStepBackwardSelected();
		
		void setZoomFactor( double f);
		
		void zoomIn();
		void zoomOut();
		// </FIXME>
	protected:
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent ( QMouseEvent * e );
		void mouseReleaseEvent(QMouseEvent *e);
		void wheelEvent( QWheelEvent *event );
		
		void keyPressEvent(QKeyEvent *event);
		
		void paintEvent(QPaintEvent *);
		virtual void draw(QPainter *p);
		virtual void drawFrame(const KTKeyFrame *frame, QPainter *painter, float intensitive = 1);
		virtual void drawGraphic(const AGraphicComponent *graphic, QPainter *painter, float intensitive = 1);
		void resizeEvent(QResizeEvent * event );
		
		inline void fillDevice(const QBrush &brush)
		{
			switch(m_renderType)
			{
				case KToon::Image:
				{
					qobject_cast<AImageDeviceWidget *>(m_paintDevice)->setBackgroundColor( brush.color() );
				}
				break;
				case KToon::OpenGL:
				case KToon::Native:
				{
					QPainter painter(m_paintDevice);
					painter.fillRect(m_paintDevice->rect(), brush);
				}
			}
		}
		
		
	signals:
		void mousePos(const QPoint& p);
		void changedZoomFactor(double f);
};


#endif
