/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                   *
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

//
#ifndef KTPAINTAREA_H
#define KTPAINTAREA_H

#include <QGraphicsView>
#include <kttoolplugin.h>

#include "ktabstractprojecteventhandler.h"


class QGraphicsRectItem;
class KTBrushManager;
class KTInputDeviceInformation;
class KTProject;
class KTPaintAreaRotator;
/**
 * Esta clase provee un area para hacer dibujos.
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\>
*/


class KTPaintArea : public QGraphicsView, public KTAbstractProjectEventHandler
{
	Q_OBJECT;
	public:
		KTPaintArea(KTProject *project, QWidget * parent = 0);
		~KTPaintArea();
		void setPhotogram(QGraphicsScene *photogram);
		
		void setAntialiasing(bool use);
		void setUseOpenGL(bool opengl);
		void setDrawGrid(bool draw);
		void setTool(KTToolPlugin *tool);
		
		bool drawGrid() const;
		
		void setCurrentScene(int index);
		
		void scaleView(qreal scaleFactor);
		
		void setRotationAngle(int angle);
		
		KTBrushManager *brushManager() const;
		
	private:
		void saveState();
		void restoreState();
		
	protected:
		void mousePressEvent ( QMouseEvent * event  );
		void mouseDoubleClickEvent( QMouseEvent *event);
		void mouseMoveEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent *event );
		void tabletEvent ( QTabletEvent * event );
		void wheelEvent( QWheelEvent *event );
		void keyPressEvent(QKeyEvent *event);
		
	signals:
		void cursorPosition(const QPointF &pos);
		void eventTriggered(const KTProjectEvent *event);
		
	private:
		QMouseEvent *mapMouseEvent(QMouseEvent *event) const;
		
	public slots:
		void centerDrawingArea();
		void deleteItems();
		void groupItems();
		void ungroupItems();
		void copyItems();
		void pasteItems();
		void cutItems();
		
	protected:
		void frameEvent(KTFrameEvent *event);
		void layerEvent(KTLayerEvent *event);
		void sceneEvent(KTSceneEvent *event);
		void projectEvent(KTProjectEvent *event);
		void itemEvent(KTItemEvent *event);
		void drawBackground(QPainter *painter, const QRectF &rect);
		
		
		
	private:
		QGraphicsRectItem *m_grid;
		KTToolPlugin *m_tool;
		
		bool m_isDrawing;
		
		KTBrushManager *m_brushManager;
		KTInputDeviceInformation *m_inputInformation;
		
		KTProject *m_project;
		
		QRectF m_drawingRect;
		
		int m_currentSceneIndex;
		
		bool m_drawGrid;
		double m_angle;
		
		KTPaintAreaRotator *m_rotator;
		
		QStringList m_copiesXml;
};

#endif
