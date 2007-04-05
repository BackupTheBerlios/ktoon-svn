/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
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
#include "stepsviewer.h"
#include <QGraphicsPathItem>

#include "kttweenerstep.h"

#include "spinboxdelegate.h"
#include <QDebug>
#include <cmath>

struct StepsViewer::Private
{
	QPolygonF points;
	QPolygonF stops;
	QList<int> frames;
};

StepsViewer::StepsViewer(QWidget *parent) : QTableWidget(parent), d(new Private)
{
	setColumnCount(2);
	setHorizontalHeaderLabels(QStringList() << tr("interval") << tr("frames"));
	setItemDelegate(new SpinBoxDelegate);
}

StepsViewer::~StepsViewer()
{
	delete d;
}

void StepsViewer::setPath(const QGraphicsPathItem *path)
{
	QPolygonF points = path->path().toFillPolygon();
	
	if(points.count() > 2)
	{
		d->points.clear();
		d->frames.clear();
		
		points.pop_back();
		d->points = points;
		points.pop_front();
		
		int count = 1;
		setRowCount(0);
		for(int i = 1; i < path->path().elementCount(); i++)
		{
			QPainterPath::Element e  = path->path().elementAt(i);
			
			if(e.type != QPainterPath::CurveToElement )
			{
				if(e.type  == QPainterPath::CurveToDataElement && path->path().elementAt(i-1).type  == QPainterPath::CurveToElement) continue;
				
				d->stops << e;
				
				int frames = 0;
				
				QPolygonF::iterator it = points.begin();
				
				while(it != points.end())
				{
					frames++;
					if(e == (*it))
					{
						break;
					}
					else
					{
						it = points.erase(it);
					}
				}
				d->frames << frames;
				setRowCount(rowCount()+1);
				QTableWidgetItem *item = new QTableWidgetItem(QString::number(count));
				item->setText(QString::number(count));
				item->setFlags(item->flags() & ~Qt::ItemIsEditable);
				QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(frames));
				item1->setText(QString::number(frames));
				
				setItem(count-1, 0, item );
				setItem(count-1, 1, item1 );
				count++;
			}
		}
	}
}

QVector<KTTweenerStep *> StepsViewer::steps()
{
	QVector<KTTweenerStep *> s;
	QVectorIterator<QPointF> point(d->points);
	int count = 0;
	
	KTTweenerStep *step = new KTTweenerStep(0);
	step->setPosition(point.next());
	s << step;
	
	for(int i = 0; i < rowCount(); i++)
	{
		for(int j = 0; j < d->frames[i]; j++)
		{
			int frames = item(i,1)->text().toInt();
			count += (int)::ceil(frames /d->frames[i]);
			KTTweenerStep *step = new KTTweenerStep(count);
			step->setPosition(point.next());
			s << step;
		}
	}
	return s;
}

int StepsViewer::totalSteps()
{
	int total = 0;
	for(int i = 0; i < rowCount(); i++)
	{
		total += item(i,1)->text().toInt();
	}
	return total;
}


