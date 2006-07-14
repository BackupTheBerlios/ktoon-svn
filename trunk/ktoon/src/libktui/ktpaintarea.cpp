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

#include "ktpaintarea.h"

#include "ddebug.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>

#ifdef QT_OPENGL_LIB
#include <QGLWidget>
#endif

KTPaintArea::KTPaintArea(QWidget * parent) : QGraphicsView(parent)
{
	setMouseTracking(true);
	
	QGraphicsScene *sscene = new QGraphicsScene(QRect() , this);
	setScene(sscene);
	
	setBackgroundBrush (Qt::white);
 	
	m_grid =  scene()->addRect( QRect() , QPen(Qt::black, 3), QBrush() );
	
#ifdef QT_OPENGL_LIB
	setViewport(new QGLWidget());
#endif
}

KTPaintArea::~KTPaintArea()
{
	
}



void KTPaintArea::mousePressEvent ( QMouseEvent * event )
{
}

void KTPaintArea::mouseMoveEvent ( QMouseEvent * event )
{
	emit cursorPosition(event->pos()  );
}

void KTPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
}


void KTPaintArea::resizeEvent ( QResizeEvent * event )
{
	scene()->setSceneRect(rect().normalized().adjusted ( 0, 0, -25, -25 ) );
	QPoint zero(scene()->width() - 500, scene()->height() - 400);
	m_grid->setRect(QRectF(mapToScene(zero/2), QSizeF( 500, 400) ));
}
