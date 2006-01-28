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

#include "afilltool.h"
#include "ktapplication.h"

AFillTool::AFillTool()
{
}


AFillTool::~AFillTool()
{
}


QStringList AFillTool::keys() const
{
	return QStringList() << "Fill";
}


QHash< QString, QAction * > AFillTool::actions()
{
	QHash<QString, QAction *> hash;
	
	QAction *fillAction = new QAction( QIcon(QPixmap(KTOON_THEME_DIR+"/icons/fill.png")), tr("Fill"), this);
	fillAction->setShortcut( QKeySequence(tr("F")) );
	
	hash.insert( tr("Fill"), fillAction );
	
	return hash;
}

QPainterPath AFillTool::path() const
{
	return QPainterPath();
}

QRect AFillTool::press(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& pos, KTKeyFrame* currentFrame)
{
	if( currentFrame->selectedComponents().count() > 0 )
	{
		AGraphicComponent *component = currentFrame->selectedComponents()[0];
		
		foreach(AGraphic *graphic, component->graphics())
		{
			if ( graphic->path.contains(pos) )
			{
				graphic->brush = painter.pen().brush();
			}
		}
		
		emit requestRedraw();
		
	}
	
	return QRect(0,0,0,0);
}

QRect AFillTool::move(const QString& brush, QPainter& painter, const QPainterPath& form, const QPoint& oldPos, const QPoint& newPos)
{
	return QRect(0,0,0,0);
}

QRect AFillTool::release(const QString &brush, QPainter &painter,const QPainterPath &form,const QPoint &pos)
{
	
	return QRect(0,0,0,0);
}

QWidget* AFillTool::configurator()
{
	return 0;
}

int AFillTool::type() const
{
	return Fill;
}

Q_EXPORT_PLUGIN( AFillTool );
