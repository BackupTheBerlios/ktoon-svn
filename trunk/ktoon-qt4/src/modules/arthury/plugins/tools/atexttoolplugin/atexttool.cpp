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

#include <dglobal.h>

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
	QStringList textList = m_configurator->text().split('\n');
	
	QPoint tpos = m_position;
	
	QFontMetrics fm(m_configurator->textFont());
	
	foreach(QString text, textList)
	{
		m_path.addText(tpos, m_configurator->textFont(), text);
		tpos.setY(tpos.y() + fm.size(Qt::TextSingleLine, text).height() );
	}
	
	painter.drawPath(m_path);
	
	QRect boundingRect = m_path.boundingRect().toRect().normalized().adjusted(-2,-2,2,2);
	
	return boundingRect;
}

QPainterPath ATextTool::path() const
{
	return m_path;
}

QHash<QString, DAction *> ATextTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *pencil = new DAction( QIcon(THEME_DIR+"/icons/text.png"), tr("Text"), this);
	pencil->setShortcut( QKeySequence(tr("T")) );
	
	hash.insert( tr("Text"), pencil );
	
	return hash;
}

int ATextTool::type() const
{
	return Brush;
}
		
QWidget *ATextTool::configurator()
{
	return m_configurator;
}

bool ATextTool::isComplete() const
{
	return true;
}

void ATextTool::aboutToChangeTool() 
{
// 	m_configurator->hide();
}


Q_EXPORT_PLUGIN( ATextTool );



