//
// C++ Implementation: apaintarea
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "apaintarea.h"
#include "ktdebug.h"

#include <QPalette>
#include <QPainter>



APaintArea::APaintArea(QWidget *parent) : QWidget(parent), m_xpos(0), m_ypos(0), m_zero(0)
{
	KTINIT;
	setAttribute(Qt::WA_StaticContents);
	
	m_paintDevice = QImage(520, 340, QImage::Format_RGB32);
	m_paintDevice.fill(qRgb(255, 255, 255));
	
	setMouseTracking(true);
}


APaintArea::~APaintArea()
{
	KTEND;
}

void APaintArea::mouseMoveEvent(QMouseEvent *e)
{
	emit mousePos(e->pos());
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	if(event->buttons() == Qt::LeftButton)
	{
		m_path.lineTo(event->pos());
	}
	
	QPainter painter(&m_paintDevice);
	
	painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawPath(m_path);
	painter.end();
	
// 	m_path = QPainterPath();
	repaint();
// 	qDebug() << "APaintArea " << m_path.elementCount () << endl ;
}

QSize APaintArea::sizeHint() const
{
	return m_paintDevice.size()+ QSize(m_zero,m_zero);
}

QSize APaintArea::minimumSizeHint () const
{
	return QSize(m_zero,m_zero);
}

void APaintArea::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	
	painter.drawImage(QPoint(m_xpos, m_ypos), m_paintDevice);
	painter.end();
}

void APaintArea::resizeEvent ( QResizeEvent * event )
{
	m_xpos = width()/2 -m_paintDevice.width()/2;
	m_ypos = height()/2-m_paintDevice.height()/2;
	repaint();
	QWidget::resizeEvent(event);
	
}

QPoint APaintArea::paintDevicePosition() const
{
	return QPoint(m_xpos, m_ypos);
}

void APaintArea::setZeroAt(int zero)
{
	m_zero = zero*2;
	resize(sizeHint());
	repaint();
}

QImage APaintArea::paintDevice() const
{
	return m_paintDevice;
}

void APaintArea::mousePressEvent ( QMouseEvent * e )
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	if(event->button() == Qt::LeftButton)
	{
		m_path.moveTo(event->pos());
	}
	
}
