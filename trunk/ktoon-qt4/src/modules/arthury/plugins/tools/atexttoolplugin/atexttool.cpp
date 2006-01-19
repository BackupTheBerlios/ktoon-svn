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
 
#include "atexttool.h"

#include <QPointF>
#include <QFontMetrics>
#include <QKeySequence>

QStringList ATextTool::keys() const
{
	return QStringList() << tr("Text");
}

QRect ATextTool::press(const QString &brush, QPainter &painter, const QPainterPath &form,const QPoint &pos, KTKeyFrame *currentFrame)
{
	m_path = QPainterPath();
	m_path.moveTo(pos);
	
	m_position = pos;
	
	return QRect();
}

QRect ATextTool::move(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &oldPos, const QPoint &newPos)
{
	return QRect();
}

QRect ATextTool::release(const QString &  brush ,QPainter &  painter ,const QPainterPath &/*form*/, const QPoint &  pos )
{
	m_path.addText(m_position, painter.font(), m_configurator->text());
	
	painter.drawPath(m_path);
	
	QRect boundingRect = m_path.boundingRect().toRect().normalized().adjusted(-2,-2,2,2);
	
	return boundingRect;
}

QPainterPath ATextTool::path() const
{
	return m_path;
}

QHash<QString, QAction *> ATextTool::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *pencil = new QAction( QIcon(), tr("Text"), this);
	pencil->setShortcut( QKeySequence(tr("T")) );
	
	hash.insert( tr("Text"), pencil );
	
	return hash;
}

Q_EXPORT_PLUGIN( ATextTool );


