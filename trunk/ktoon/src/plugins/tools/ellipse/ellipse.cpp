/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
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
#include "ellipse.h"

#include <daction.h>
#include <dglobal.h>

#include <QPointF>
#include <QKeySequence>
#include <QMatrix>

#include "ktscene.h"

// #include "node.h"

Ellipse::Ellipse()
{
	setupActions();
}


Ellipse::~Ellipse()
{
}

void Ellipse::init(QGraphicsView *view)
{
	view->setDragMode ( QGraphicsView::NoDrag );
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		item->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
}

QStringList Ellipse::keys() const
{
	return QStringList() << "Ellipse";
}

void Ellipse::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if(input->buttons () == Qt::LeftButton)
	{
		m_ellipse = scene->addEllipse(QRectF(QPointF(0,0), QSizeF(0.0,0.0)));
		m_ellipse->setPos(input->pos());
	}
}

void Ellipse::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if(input->buttons () == Qt::LeftButton)
	{
		if(m_ellipse)
		{
			QRectF rect = m_ellipse->rect();
			rect.setBottomRight(m_ellipse->mapFromScene(input->pos() ));
			m_ellipse->setRect( rect);
		}
	}
}

void Ellipse::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{	
// 	if(m_ellipse)
// 	{
// 		QPainterPath path = m_ellipse->shape();
// 		QPolygonF poly = path.toFillPolygon();
// 		Node *startAngle = new Node(Node::StartAngle, poly.first(), m_ellipse, scene);
// 		Node *spanAngle = new Node(Node::SpanAngle, poly.last(), m_ellipse, scene);
// 	}
	
}

QString Ellipse::toolToXml() const
{
	return QString();
}

QPainterPath Ellipse::path() const
{
	
}

void Ellipse::setupActions()
{
	DAction *ellipse = new DAction( QIcon(), tr("Ellipse"), this);
	ellipse->setShortcut( QKeySequence(tr("Ctrl+E")) );
	QPixmap pix(THEME_DIR+"/cursors/circle.png");
	ellipse->setCursor( QCursor(pix, 0, 0) );
	m_actions.insert( tr("Ellipse"), ellipse );
}

QMap<QString, DAction *>Ellipse::actions() const
{
	return m_actions;
}

int Ellipse::toolType() const
{
	return KTToolInterface::Brush;
}

QWidget *Ellipse::configurator()
{
	return new QWidget();
}

bool Ellipse::isComplete() const
{
	return true;
}

void Ellipse::aboutToChangeTool()
{
}

Q_EXPORT_PLUGIN2(kt_ellipse,Ellipse  )
