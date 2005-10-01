//
// C++ Implementation: agraphiccomponent
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "agraphiccomponent.h"

AGraphicComponent::AGraphicComponent(QObject *parent) : QObject(parent), m_path(), m_color(Qt::black), m_brush(), m_pen(m_color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
	
}


AGraphicComponent::~AGraphicComponent()
{
	
}

 
QRectF AGraphicComponent::boundingRect() const
{
	return m_path.boundingRect();
}

QPainterPath AGraphicComponent::path() const
{
	return m_path;
}

QBrush AGraphicComponent::brush() const
{
	m_brush;
}

QPen AGraphicComponent::pen() const
{
	return m_pen;
}

QColor AGraphicComponent::color() const
{
	return m_color;
}

void AGraphicComponent::setPath(const QPainterPath &path )
{
	m_path = path;
}

void AGraphicComponent::setBrush(const QBrush &brush)
{
	m_brush = brush;
}

void AGraphicComponent::setPen(const QPen &pen)
{
	m_pen = pen;
}

void AGraphicComponent::setColor(const QColor &color)
{
	m_color = color;
}
