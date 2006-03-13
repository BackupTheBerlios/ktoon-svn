/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktpaintareacontainer.h"

#include <QScrollArea>
#include <QScrollBar>
#include <QGridLayout>
#include <QPushButton>

#include "ddebug.h"

KTPaintAreaContainer::KTPaintAreaContainer(const QSize& size, APaintArea::RenderType type,  QWidget *parent) : QWidget(parent), m_drawAreaDelta(50,50)
{
	DINIT;
// 	setMouseTracking(true);
	
	QGridLayout *grid = new QGridLayout(this);
	grid->setMargin(0);
	grid->setSpacing(0);
	
	m_HRuler = new KTDocumentRuler(Qt::Horizontal, 0);
	grid->addWidget (m_HRuler,0,1);
	
	m_VRuler = new KTDocumentRuler(Qt::Vertical, 0);
	grid->addWidget (m_VRuler,1,0);
	
	QScrollArea *m_scroller = new QScrollArea(this);
	
// 	m_scroller->setBackgroundRole(QPalette::Mid);
	
	m_HRuler->setZeroAt(m_drawAreaDelta.x());
	m_VRuler->setZeroAt(m_drawAreaDelta.y());
	
	m_drawArea = new APaintArea(size, type, m_scroller);
	m_drawArea->setOffset(m_drawAreaDelta);
	
	m_HRuler->setMinimumWidth(m_drawArea->width());
	m_VRuler->setMinimumHeight(m_drawArea->height());
	
	m_scroller->setWidget(m_drawArea);
	
	m_scroller->setWidgetResizable ( true );
	m_scroller->setFocus();
	
	QScrollBar *hBar = m_scroller->horizontalScrollBar();
	hBar->setSingleStep(10);
	connect(hBar, SIGNAL(sliderMoved(int)), m_HRuler, SLOT(slide(int)));
	
	QScrollBar *vBar = m_scroller->verticalScrollBar();
	vBar->setSingleStep(10);
	connect(vBar, SIGNAL(sliderMoved(int)), m_VRuler, SLOT(slide(int)));
	
	QPushButton *corner = new QPushButton(this);
	corner->setMaximumSize(m_VRuler->width(), m_HRuler->height() );
	
	grid->addWidget(corner, 0, 0, Qt::AlignCenter);
	
	grid->addWidget(m_scroller,1,1);
	
	connect(m_drawArea, SIGNAL(mousePos(const QPoint &)), this, SLOT(moveRulerPointers(const QPoint &)));
}

KTPaintAreaContainer::~KTPaintAreaContainer()
{
	DEND;
}


void KTPaintAreaContainer::moveRulerPointers(const QPoint &p)
{
	m_HRuler->movePointers(p);
	m_VRuler->movePointers(p);
}

APaintArea *KTPaintAreaContainer::drawArea() const
{
	return m_drawArea;
}

void KTPaintAreaContainer::resizeEvent ( QResizeEvent * e)
{
#if 0
	m_drawAreaDelta.setY( height()/2 - m_drawArea->paintDevice()->height()/2);
	m_drawAreaDelta.setX( width()/2 - m_drawArea->paintDevice()->width()/2 );
	if(m_drawAreaDelta.x() < 0 )
	{
		m_drawAreaDelta.setX(0);
	}
	if( m_drawAreaDelta.y() < 0 )
	{
		m_drawAreaDelta.setY(0);
	}
	m_HRuler->setZeroAt(m_drawAreaDelta.x());
	m_VRuler->setZeroAt(m_drawAreaDelta.y());
	m_drawArea->setOffset(m_drawAreaDelta);
#endif
	m_drawArea->resize(size());
	QWidget::resizeEvent (e);
}

