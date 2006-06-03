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
#include "ashapebrushplugin.h"

#include <QKeySequence>
#include <QMatrix>

#include "dglobal.h"

AShapeBrushPlugin::AShapeBrushPlugin()
{
	m_configurator = new ShapeConfigurator;
}


AShapeBrushPlugin::~AShapeBrushPlugin()
{
	if (m_configurator) delete m_configurator;
}

QStringList AShapeBrushPlugin::keys() const
{
	return QStringList() << tr("Shape brush");
}

QRect AShapeBrushPlugin::press(const QString &brush, QPainter &painter,const QPoint &pos, KTKeyFrame *currentFrame)
{
	m_path = QPainterPath();
	m_path.moveTo(pos);
	
	return move(brush, painter, pos, pos);
}

QRect AShapeBrushPlugin::move(const QString &brush, QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
	painter.save();
	
	QPainterPath aPath = m_configurator->shape();
	
	QMatrix matrix;
	matrix.translate(newPos.x() - aPath.boundingRect().center().x(), newPos.y() - aPath.boundingRect().center().y()  );
	
	QPainterPath form = matrix.map(aPath);

	int rad = painter.pen().width()/2 + 1;
	QRect boundingRect = form.boundingRect().toRect().normalized().adjusted(-rad, -rad, +rad, +rad);
	
	QColor color = painter.pen().color();
	int thickness = painter.pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);

	QPainterPath path;
	path.setFillRule( Qt::WindingFill );
	
	if (brush == tr("Shape brush") )
	{
		path.addPath(form);
	}
	
	m_path.addPath(path);
	m_path.setFillRule( Qt::WindingFill );
	
	painter.drawPath(path);
	
// 	boundingRect = path.boundingRect().toRect();
	
	painter.restore();
	return boundingRect;
}

QRect AShapeBrushPlugin::release(const QString &  brush ,QPainter &  painter , const QPoint &  pos )
{
	return QRect(0, 0, 0, 0);
}

QPainterPath AShapeBrushPlugin::path() const
{
	return m_path;
}

QHash<QString, DAction *> AShapeBrushPlugin::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *act = new DAction( QPixmap(THEME_DIR+"/icons/node_eraser.png"), tr("Shape brush"),this);
	act->setShortcut( QKeySequence(tr("Ctrl+Alt+S")) );
	hash.insert( tr("Shape brush"), act );
	
	return hash;
}

int AShapeBrushPlugin::type() const
{
	return Brush;
}

QWidget * AShapeBrushPlugin::configurator()
{
	return m_configurator;
}
		
bool AShapeBrushPlugin::isComplete() const
{
	return true;
}

void AShapeBrushPlugin::aboutToChangeTool()
{
}

Q_EXPORT_PLUGIN( AShapeBrushPlugin )

