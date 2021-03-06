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
#include "dglobal.h"

#include "dalgorithm.h"

AGradientTool::AGradientTool()
{
	m_configurator = new GradientConfigurator;
}


AGradientTool::~AGradientTool()
{
}


QHash< QString, DAction * > AGradientTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *action1 = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/random_gradient.png")), tr("Random Gradient"), this);
	action1->setShortcut( QKeySequence(tr("Shift+D")));
	action1->setCursor( QCursor(THEME_DIR+"/cursors/gradient.png") );
	
	hash.insert( tr("Random Gradient"), action1 );
	
	DAction *action2 = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/gradient.png")), tr("Gradient"), this);
	action2->setShortcut( QKeySequence( tr("Ctrl+D") ) );
	action2->setCursor( QCursor(THEME_DIR+"/cursors/gradient.png") );
	hash.insert( tr("Gradient"), action2 );

	
	return hash;
}

QPainterPath AGradientTool::path() const
{
	return QPainterPath();
}

QRect AGradientTool::move(const QString& brush, QPainter& painter, const QPoint& oldPos, const QPoint& newPos)
{
	QPainterPath path;
	path.moveTo(m_initialPoint);
	path.lineTo(newPos);
	
	emit toDrawGhostGraphic( path );
	return QRect(0,0,0,0);
}

QRect AGradientTool::press(const QString& brush, QPainter& painter, const QPoint& pos, KTKeyFrame* currentFrame)
{
	m_initialPoint = pos;
	
	m_currentFrame = currentFrame;
	
	m_rect = painter.window();
	
	return QRect(0,0,0,0);
}

QRect AGradientTool::release(const QString& brush, QPainter& painter, const QPoint& pos)
{	
	AGraphicComponent *component = new AGraphicComponent;
	QPainterPath path;
	path.addRect(m_rect);
	
	if ( brush == tr("Gradient") )
	{
		const QGradient *gradient = m_configurator->gradient();

		component->addGraphic( path, Qt::NoPen, DGradientAdjuster::adjustGradient(gradient,m_rect ));
	}
	else if ( brush == tr("Random Gradient") )
	{
		QLinearGradient gradient(m_initialPoint, pos);
		gradient.setColorAt(0, DAlgorithm::randomColor());
		gradient.setColorAt(0.5, DAlgorithm::randomColor());
		gradient.setColorAt(1, DAlgorithm::randomColor());
		
		component->addGraphic( path, Qt::NoPen, gradient);
	}
	
	m_currentFrame->addComponent( component );
	
	emit requestRedraw();
	
	return QRect(0,0,0,0);
}

QStringList AGradientTool::keys() const
{
	return QStringList() << tr("Gradient") << tr("Random Gradient");
}

QWidget * AGradientTool::configurator()
{
	return m_configurator;
}

bool AGradientTool::isComplete() const
{
	return false;
}

int AGradientTool::type() const
{
	return Fill;
}

void AGradientTool::aboutToChangeTool()
{
}


Q_EXPORT_PLUGIN( AGradientTool );

