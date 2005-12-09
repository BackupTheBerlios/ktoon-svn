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

#include "ageometrictoolplugin.h"

#include <QKeySequence>
#include <QDebug>
#include <QImage>
#include <QPaintDevice>

#include "ktdebug.h"

QStringList AGeometricToolPlugin::keys() const
{
	return QStringList() << tr("Rectangle") << tr("Ellipse");
}

QRect AGeometricToolPlugin::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos)
{
	m_path = QPainterPath();
	m_path.moveTo(pos);
	
// 	painter.drawEllipse(pos.x(), pos.y(), 2,2);
	
	m_rect.setTopLeft(pos);
	
	return move(brush, painter, form, pos, pos);
}

QRect AGeometricToolPlugin::move(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();
	
	int rad = painter.pen().width() / 2;
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
// 		path.addRect(m_rect);
	}
	else if ( brush == tr("Ellipse"))
	{
// 		path.addEllipse(m_rect);
	}
// 	painter.setPen(QPen(color, 0.1, Qt::DotLine));
	painter.drawPath(path);
	
	boundingRect = path.boundingRect().toRect();
	
	painter.restore();
	return boundingRect;
}

QRect AGeometricToolPlugin::release(const QString &  brush ,QPainter &  painter , const QPainterPath &form, const QPoint &  pos )
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
	
	QRect rect = m_path.boundingRect().toRect().normalized().adjusted(-rad, -rad, +rad, +rad);
	
	applyGradient( &painter, rect); 
	
	painter.drawPath(m_path);
	
	return rect;
// 	return QRect(0, 0, 0, 0);
}

QPainterPath AGeometricToolPlugin::path() const
{
	return m_path;
}

QHash<QString, QAction *> AGeometricToolPlugin::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *circle = new QAction( tr("Rectangle"), this);
// 	circle->setShortcut( QKeySequence(tr("R")) );
	
	hash.insert( tr("Rectangle"), circle );
	
	
	
	QAction *rectangle = new QAction( tr("Ellipse"), this);
// 	rectangle->setShortcut( QKeySequence(tr("C")) );
	hash.insert(tr("Ellipse"), rectangle);

	return hash;
}

void AGeometricToolPlugin::applyGradient(QPainter *painter, const QRect &rect)
{
	if ( painter && rect.isValid() && !rect.isNull() )
	{
		const QGradient *gradient = painter->brush().gradient();
		if( gradient )
		{
			QGradient newGradient;
			newGradient.setSpread(gradient->spread());
			switch( gradient->type() )
			{
				case  QGradient::LinearGradient:
				{
					newGradient = QLinearGradient(rect.topLeft(), rect.topRight());
					break;
				}
				case QGradient::RadialGradient:
				{
					
					QPointF focal= static_cast<const QRadialGradient*>(gradient)->focalPoint ();
// 					focal = focal * (QMatrix().scale( rect.width()/ 100 ,   rect.height()/100  ));
// 					ktDebug() << focal.x() << ", " << focal.y();
					newGradient = QRadialGradient(rect.center(), (rect.left()-rect.right())/2, rect.center() );
					break;
				}
				case QGradient::ConicalGradient:
				{
					double angle = static_cast<const QConicalGradient*>(gradient)->angle();
					ktDebug() << angle;
					newGradient = QConicalGradient(rect.center(), angle);
					break;
				}
			}
			newGradient.setStops(gradient->stops());
			
			painter->setBrush(QBrush(newGradient));
		}
	}
}

Q_EXPORT_PLUGIN( AGeometricToolPlugin )

