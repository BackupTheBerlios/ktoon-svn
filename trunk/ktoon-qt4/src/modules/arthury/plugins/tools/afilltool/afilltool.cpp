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
#include "dglobal.h"

#include "dbrushadjuster.h"

AFillTool::AFillTool()
{
}


AFillTool::~AFillTool()
{
}


QStringList AFillTool::keys() const
{
	return QStringList() << "Fill" << "Remove Fill" << "Countour Fill";
}


QHash< QString, DAction * > AFillTool::actions()
{
	QHash<QString, DAction *> hash;
	
	DAction *fillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/fill.png")), tr("Fill"), this);
	fillAction->setShortcut( QKeySequence( tr("F") ) );
	
	hash.insert( tr("Fill"), fillAction );
	
	DAction *removeFillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/removefill.png")), tr("Remove Fill"), this);
	removeFillAction->setShortcut( QKeySequence(tr("Shift+F") ));
	
	hash.insert( tr("Remove Fill"), removeFillAction );
	
	DAction *countourFillAction = new DAction( QIcon(QPixmap(THEME_DIR+"/icons/contour.png")), tr("Countour Fill"), this);
	countourFillAction->setShortcut( QKeySequence(tr("Control+F") ));
	
	hash.insert( tr("Countour Fill"), countourFillAction );
	
	return hash;
}

QPainterPath AFillTool::path() const
{
	return QPainterPath();
}

QRect AFillTool::press(const QString& brush, QPainter& painter, const QPoint& pos, KTKeyFrame* currentFrame)
{
	if( currentFrame->selectedComponents().count() > 0 )
	{
		AGraphicComponent *selected = currentFrame->selectedComponents()[0];
		
		QList<AGraphicComponent*> components = QList<AGraphicComponent*>() << selected << selected->allChilds();
		
		bool finded = false;
		
		foreach(AGraphicComponent *component, components)
		{
			foreach(AGraphic *graphic, component->graphics())
			{
				if ( graphic->path.contains(pos) )
				{
					if ( brush == "Fill" )
					{
						graphic->brush = DBrushAdjuster::adjustBrush(painter.pen().brush(), graphic->path.boundingRect().toRect());
					}
					else if ( brush == "Remove Fill" )
					{
						graphic->brush = Qt::transparent;
					}
					else if ( brush == "Countour Fill" )
					{
						graphic->pen.setBrush(painter.pen().brush());
					}
					finded = true;
					break;
				}
			}
			if(finded) break;
		}
		
		if ( finded)
		{
			emit requestRedraw();
		}
	}
	
	return QRect(0,0,0,0);
}

bool AFillTool::findChild(AGraphicComponent *component, const QString &brush, const QPoint &clickedPos)
{
}

QRect AFillTool::move(const QString& brush, QPainter& painter, const QPoint& oldPos, const QPoint& newPos)
{
	return QRect(0,0,0,0);
}

QRect AFillTool::release(const QString &brush, QPainter &painter,const QPoint &pos)
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

bool AFillTool::isComplete() const
{
	return false;
}
void AFillTool::aboutToChangeTool() 
{
}

Q_EXPORT_PLUGIN( AFillTool );
