/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "kttweenerstep.h"
#include <QVector>

struct KTTweenerStep::Private
{
	QPointF position;
	double rotation;
	
	struct PairF {
		double x;
		double y;
	} translation, shear, scale;
	
	int flags;
	int n;
};

KTTweenerStep::KTTweenerStep(int n)
 : KTAbstractSerializable(), k(new Private)
{
	k->n = n;
	k->flags = None;
}

KTTweenerStep::~KTTweenerStep()
{
	delete k;
}

void KTTweenerStep::setPosition(const QPointF &pos)
{
	k->position = pos;
	k->flags |= Position;
}

void KTTweenerStep::setTranslation(double dx, double dy)
{
	k->translation.x = dx;
	k->translation.y = dy;
	k->flags |= Translation;
}

void KTTweenerStep::setRotation(double angle)
{
	k->rotation = angle;
	k->flags |= Rotation;
}

void KTTweenerStep::setShear(double sh, double sv)
{
	k->shear.x = sh;
	k->shear.y = sv;
	k->flags |= Shear;
}

void KTTweenerStep::setScale(double sx, double sy)
{
	k->scale.x = sx;
	k->scale.y = sy;
	k->flags |= Scale;
}


bool KTTweenerStep::has(Type type) const
{
	return k->flags & type;
}

int KTTweenerStep::n() const
{
	return k->n;
}

QPointF KTTweenerStep::position() const
{
	return k->position;
}

double KTTweenerStep::horizontalScale() const
{
	return k->scale.x;
}

double KTTweenerStep::verticalScale() const
{
	return k->scale.y;
}

double KTTweenerStep::horizontalShear() const
{
	return k->shear.x;
}

double KTTweenerStep::verticalShear() const
{
	return k->shear.y;
}

double KTTweenerStep::rotation() const
{
	return k->rotation;
}

double KTTweenerStep::xTranslation() const
{
	return k->translation.x;
}

double KTTweenerStep::yTranslation() const
{
	return k->translation.y;
}


QDomElement KTTweenerStep::toXml(QDomDocument& doc) const
{
	QDomElement step = doc.createElement("step");
	step.setAttribute("value", k->n);
	
	if(this->has(KTTweenerStep::Position) )
	{
		QDomElement e = doc.createElement("position");
		e.setAttribute("x", k->position.x());
		e.setAttribute("y", k->position.y());
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Scale) )
	{
		QDomElement e = doc.createElement("scale");
		e.setAttribute("sx", k->scale.x);
		e.setAttribute("sy", k->scale.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Translation) )
	{
		QDomElement e = doc.createElement("translation");
		e.setAttribute("dx", k->translation.x);
		e.setAttribute("dy", k->translation.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Shear) )
	{
		QDomElement e = doc.createElement("shear");
		e.setAttribute("sh", k->shear.x);
		e.setAttribute("sv", k->shear.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Rotation) )
	{
		QDomElement e = doc.createElement("rotation");
		e.setAttribute("angle", k->rotation);
		
		step.appendChild(e);
	}
	
	return step;
}


void KTTweenerStep::fromXml(const QString& xml)
{
	QDomDocument doc;
	
	if( doc.setContent(xml ) )
	{
		QDomElement root = doc.documentElement();
		
		QDomNode n = root.firstChild();
		
		k->n = root.attribute("value").toInt();
		
		while(!n.isNull())
		{
			QDomElement e = n.toElement();
			if(!e.isNull())
			{
				if( e.tagName() == "rotation" )
				{
					setRotation(e.attribute("angle").toDouble());
				}
				else if( e.tagName() == "shear" )
				{
					setShear(e.attribute("sh").toDouble(), e.attribute("sv").toDouble());
				}
				else if( e.tagName() == "position" )
				{
					setPosition(QPointF(e.attribute("x").toDouble(), e.attribute("y").toDouble()));
				}
				else if( e.tagName() == "scale" )
				{
					setScale(e.attribute("sx").toDouble(), e.attribute("sy").toDouble());
				}
				else if( e.tagName() == "translation" )
				{
					setTranslation(e.attribute("dx").toDouble(), e.attribute("dy").toDouble());
				}
				
			}
			n = n.nextSibling();
		}
	}
}

QDomDocument KTTweenerStep::createXml(int frames, const QVector<KTTweenerStep *> &steps)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("tweening");
	root.setAttribute("frames", frames);
	
	foreach(KTTweenerStep *step, steps)
	{
		root.appendChild(step->toXml(doc));
	}
	
	doc.appendChild(root);
	
	return doc;
}

