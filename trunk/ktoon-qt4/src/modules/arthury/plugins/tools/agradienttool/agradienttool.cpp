/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "agradienttool.h"
#include "ktapplication.h"

#include "ktalgorithm.h"

AGradientTool::AGradientTool()
{
}


AGradientTool::~AGradientTool()
{
}


QHash< QString, KTAction * > AGradientTool::actions()
{
	QHash<QString, KTAction *> hash;
	
	KTAction *action1 = new KTAction( QIcon(QPixmap(KTOON_THEME_DIR+"/icons/fill.png")), tr("Random Gradient"), this);
// 	action1->setShortcut( QKeySequence( tr("F") ) );
	
	hash.insert( tr("Random Gradient"), action1 );

	
	return hash;
}

QPainterPath AGradientTool::path() const
{
	return QPainterPath();
}

QRect AGradientTool::move(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& oldPos, const QPoint& newPos)
{
	QPainterPath path;
	path.moveTo(m_initialPoint);
	path.lineTo(newPos);
	
	emit toDrawGhostGraphic( path );
	return QRect(0,0,0,0);
}

QRect AGradientTool::press(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos, KTKeyFrame* currentFrame)
{
	m_initialPoint = pos;
	
	m_currentFrame = currentFrame;
	
	m_rect = painter.window();
	
	return QRect(0,0,0,0);
}

QRect AGradientTool::release(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos)
{	
	AGraphicComponent *component = new AGraphicComponent;
	QPainterPath path;
	path.addRect(m_rect);
	
	if ( brush == "Random Gradient" )
	{
		QLinearGradient gradient(m_initialPoint, pos);
		gradient.setColorAt(0, KTAlgorithm::randomColor());
		gradient.setColorAt(0.5, KTAlgorithm::randomColor());
		gradient.setColorAt(1, KTAlgorithm::randomColor());
		
		component->addGraphic( path, Qt::NoPen, gradient);
	}
	
	m_currentFrame->addComponent( component );
	
	emit requestRedraw();
	
	return QRect(0,0,0,0);
}

QStringList AGradientTool::keys() const
{
	return QStringList() << "Random Gradient";
}

QWidget* AGradientTool::configurator()
{
	return 0; // TODO: Create a gradient configurator
}

bool AGradientTool::isComplete() const
{
	return false;
}

int AGradientTool::type() const
{
	return Brush;
}

void AGradientTool::aboutToChangeTool()
{
}


Q_EXPORT_PLUGIN( AGradientTool );
