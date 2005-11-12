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

AGraphicComponent::AGraphicComponent() : QObject(), m_pPath(), m_pColor(Qt::black), m_pPen(m_pColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
}

AGraphicComponent::AGraphicComponent(const AGraphicComponent &toCopy) : QObject(toCopy.parent()), m_pPath(toCopy.m_pPath), m_pColor(toCopy.m_pColor), m_pPen(toCopy.m_pPen)
{
}

AGraphicComponent::~AGraphicComponent()
{
}

 
QRectF AGraphicComponent::boundingRect() const
{
	return m_pPath.boundingRect();
}

QPainterPath AGraphicComponent::path() const
{
	return m_pPath;
}

QBrush AGraphicComponent::brush() const
{
	return m_pBrush;
}

QPen AGraphicComponent::pen() const
{
	return m_pPen;
}

QColor AGraphicComponent::color() const
{
	return m_pColor;
}

void AGraphicComponent::setPath(const QPainterPath &path )
{
	m_pPath = path;
}

void AGraphicComponent::setBrush(const QBrush &brush)
{
	m_pBrush = brush;
}

void AGraphicComponent::setPen(const QPen &pen)
{
	m_pPen = pen;
}

void AGraphicComponent::setPen(const QColor &color)
{
	m_pColor = color;
	m_pPen = QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

void AGraphicComponent::setColor(const QColor &color)
{
	m_pColor = color;
}
