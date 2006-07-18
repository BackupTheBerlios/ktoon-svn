/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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


class QGraphicsRectItem;
class KTBrushManager;

/**
 * Esta clase provee un area para hacer dibujos.
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTPaintArea : public QGraphicsView
{
	Q_OBJECT;
	public:
		KTPaintArea(QWidget * parent = 0);
		~KTPaintArea();
		void setPhotogram(QGraphicsScene *photogram);
		
		void setUseOpenGL(bool opengl);
		
		void setTool(KTToolPlugin *tool);
		
	protected:
		void mousePressEvent ( QMouseEvent * event  );
		void mouseMoveEvent ( QMouseEvent * event );
		void mouseReleaseEvent(QMouseEvent *event );
		void resizeEvent ( QResizeEvent * event );
			
	signals:
		void cursorPosition(const QPointF &pos);
		
		
	private:
		QMouseEvent *mapMouseEvent(QMouseEvent *event) const;
		
	private:
		QGraphicsRectItem *m_grid;
		KTToolPlugin *m_tool;
		
		bool m_isDrawing;
		
		KTBrushManager *m_brushManager;
		

};

#endif
