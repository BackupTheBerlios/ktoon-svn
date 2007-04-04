/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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
 : KTAbstractSerializable(), d(new Private)
{
	d->n = n;
	d->flags = None;
}

KTTweenerStep::~KTTweenerStep()
{
	delete d;
}

void KTTweenerStep::setPosition(const QPointF &pos)
{
	d->position = pos;
	d->flags |= Position;
}

void KTTweenerStep::setTranslation(double dx, double dy)
{
	d->translation.x = dx;
	d->translation.y = dy;
	d->flags |= Translation;
}

void KTTweenerStep::setRotation(double angle)
{
	d->rotation = angle;
	d->flags |= Rotation;
}

void KTTweenerStep::setShear(double sh, double sv)
{
	d->shear.x = sh;
	d->shear.y = sv;
	d->flags |= Shear;
}

void KTTweenerStep::setScale(double sx, double sy)
{
	d->scale.x = sx;
	d->scale.y = sy;
	d->flags |= Scale;
}


bool KTTweenerStep::has(Type type) const
{
	return d->flags & type;
}

int KTTweenerStep::n() const
{
	return d->n;
}

QPointF KTTweenerStep::position() const
{
	return d->position;
}

double KTTweenerStep::horizontalScale() const
{
	return d->scale.x;
}

double KTTweenerStep::verticalScale() const
{
	return d->scale.y;
}

double KTTweenerStep::horizontalShear() const
{
	return d->shear.x;
}

double KTTweenerStep::verticalShear() const
{
	return d->shear.y;
}

double KTTweenerStep::rotation() const
{
	return d->rotation;
}

double KTTweenerStep::xTranslation() const
{
	return d->translation.x;
}

double KTTweenerStep::yTranslation() const
{
	return d->translation.y;
}


QDomElement KTTweenerStep::toXml(QDomDocument& doc) const
{
	QDomElement step = doc.createElement("step");
	step.setAttribute("value", d->n);
	
	if(this->has(KTTweenerStep::Position) )
	{
		QDomElement e = doc.createElement("position");
		e.setAttribute("x", d->position.x());
		e.setAttribute("y", d->position.y());
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Scale) )
	{
		QDomElement e = doc.createElement("scale");
		e.setAttribute("sx", d->scale.x);
		e.setAttribute("sy", d->scale.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Translation) )
	{
		QDomElement e = doc.createElement("translation");
		e.setAttribute("dx", d->translation.x);
		e.setAttribute("dy", d->translation.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Shear) )
	{
		QDomElement e = doc.createElement("shear");
		e.setAttribute("sh", d->shear.x);
		e.setAttribute("sv", d->shear.y);
		
		step.appendChild(e);
	}
	
	if(this->has(KTTweenerStep::Rotation) )
	{
		QDomElement e = doc.createElement("rotation");
		e.setAttribute("angle", d->rotation);
		
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
		
		d->n = root.attribute("value").toInt();
		
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
	
	QDomElement root = doc.createElement("steps");
	root.setAttribute("frames", frames);
	
	foreach(KTTweenerStep *step, steps)
	{
		root.appendChild(step->toXml(doc));
	}
	
	doc.appendChild(root);
	
	return doc;
}

