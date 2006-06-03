/**************************************************************************
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

#include "ageometrictoolplugin.h"

#include <QKeySequence>
#include <QDebug>
#include <QImage>
#include <QPaintDevice>


#include "dglobal.h"
#include "dgradientadjuster.h"
#include "ddebug.h"

QStringList AGeometricToolPlugin::keys() const
{
	return QStringList() << tr("Rectangle") << tr("Ellipse") << tr("Line");
}

QRect AGeometricToolPlugin::press(const QString &brush, QPainter &painter,const QPoint &pos, KTKeyFrame *currentFrame)
{
	m_path = QPainterPath();
	m_path.moveTo(pos);
	
// 	painter.drawEllipse(pos.x(), pos.y(), 2,2);
	
	m_rect.setTopLeft(pos);
	
	return move(brush, painter, pos, pos);
}

QRect AGeometricToolPlugin::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();
	
	int rad = painter.pen().width()/2 + 1;
	QRect boundingRect = QRect(oldPos, newPos).normalized().adjusted(-rad, -rad, +rad, +rad);
	
	QColor color = painter.pen().color();
	int thickness = painter.pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);

	QPainterPath path;
// 	path.setFillRule( Qt::WindingFill );
	m_path.setFillRule( Qt::WindingFill );
	
	m_rect.setBottomRight(newPos);
	
	if (brush == tr("Rectangle") )
	{
		path.addRect(m_rect);
	}
	else if ( brush == tr("Ellipse"))
	{
		path.addEllipse(m_rect);
	}
	else if( brush == tr("Line"))
	{
		path.moveTo(m_rect.topLeft());
		path.lineTo( m_rect.bottomRight());
	}
	
// 	painter.setPen(QPen(color, 1, Qt::DashLine));
// 	painter.drawPath(path);
	painter.restore();
	
// 	boundingRect = path.boundingRect().toRect();
	
	emit toDrawGhostGraphic( path ); 
	return QRect(0, 0, 0, 0);
}

QRect AGeometricToolPlugin::release(const QString &  brush ,QPainter &  painter , const QPoint &  pos )
{
	int rad = painter.pen().width();
	
	m_rect.setBottomRight(pos);
	
	if (brush == tr("Rectangle") )
	{
		m_path.addRect(m_rect);
	}
	else if ( brush == tr("Ellipse"))
	{
		m_path.addEllipse(m_rect);
	}
	else if( brush == tr("Line"))
	{
		m_path.moveTo(m_rect.topLeft());
		m_path.lineTo( m_rect.bottomRight());
	}
	
	QRect rect = m_path.boundingRect().toRect().normalized().adjusted(-rad, -rad, +rad, +rad);
	
	if ( painter.brush().gradient() )
	{
		painter.setBrush(DGradientAdjuster::adjustGradient(painter.brush().gradient(), rect));
	}
	
	painter.drawPath(m_path);
	
	return rect;
// 	return QRect(0, 0, 0, 0);
}

QPainterPath AGeometricToolPlugin::path() const
{
	return m_path;
}

QHash<QString, DAction *> AGeometricToolPlugin::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *action1 = new DAction( QIcon(THEME_DIR+"/icons/square.png"), tr("Rectangle"), this);
	action1->setShortcut( QKeySequence(tr("Ctrl+R")) );
	action1->setCursor( QCursor(THEME_DIR+"/cursors/square.png") );
	
	hash.insert( tr("Rectangle"), action1 );
	
	DAction *action2 = new DAction(QIcon(THEME_DIR+"/icons/ellipse.png"), tr("Ellipse"), this);
	action2->setShortcut( QKeySequence(tr("Ctrl+E")) );
	action2->setCursor( QCursor(THEME_DIR+"/cursors/circle.png") );
	
	hash.insert(tr("Ellipse"), action2);
	
	
	DAction *action3 = new DAction( QIcon(THEME_DIR+"/icons/line.png"), tr("Line"), this);
	action3->setShortcut( QKeySequence(tr("Ctrl+L")) );
	hash.insert(tr("Line"), action3);
	
	return hash;
}

int AGeometricToolPlugin::type() const
{
	return Brush;
}
		
QWidget  *AGeometricToolPlugin::configurator()
{
	return  0;
}

bool AGeometricToolPlugin::isComplete() const
{
	return true;
}

void AGeometricToolPlugin::aboutToChangeTool() 
{

}


Q_EXPORT_PLUGIN( AGeometricToolPlugin )

