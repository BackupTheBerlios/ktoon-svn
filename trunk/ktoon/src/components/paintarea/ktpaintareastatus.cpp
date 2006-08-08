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

#include "ktpaintareastatus.h"

#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>

#include <dseparator.h>

#include "ktviewdocument.h"
#include "ktglobal.h"
#include "ktbrushmanager.h"


class ColorWidget : public QWidget
{
	public:
		ColorWidget() : m_brush(Qt::transparent) {};
		~ColorWidget() {};
		void setBrush(const QBrush &brush);
		
		QSize sizeHint() const;
		
	protected:
		void paintEvent(QPaintEvent *);
		
	private:
		QBrush m_brush;
};

QSize ColorWidget::sizeHint() const
{
	QSize size(40, -1);
	
	return size;
}


void ColorWidget::setBrush(const QBrush &brush)
{
	m_brush = brush;
}

void ColorWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	
	painter.fillRect(rect(), m_brush);
}

class BrushStatus : public QWidget
{
	public:
		BrushStatus();
		~BrushStatus();
		
		void setForeground(const QPen &pen);
		void setBackground(const QBrush &brush);
		
	private:
		ColorWidget *m_pen;
		ColorWidget *m_brush;
};

BrushStatus::BrushStatus()
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(2);
	layout->setSpacing(2);
	
	m_pen = new ColorWidget;
	
	m_brush = new ColorWidget;
	
	layout->addWidget(m_pen);
	layout->addWidget( new DSeparator(Qt::Vertical ) );
	layout->addWidget(m_brush);
}

BrushStatus::~BrushStatus()
{
}

void BrushStatus::setForeground(const QPen &pen)
{
	m_pen->setBrush( pen.brush() );
}

void BrushStatus::setBackground(const QBrush &brush)
{
	m_brush->setBrush(brush);
}

////////////////

KTPaintAreaStatus::KTPaintAreaStatus(KTViewDocument *parent) : QStatusBar(parent), m_viewDocument(parent)
{
	setSizeGripEnabled(false);
	
	m_renderHint = new QComboBox;
	m_renderHint->addItem(tr("Antialiasing"), true );
	m_renderHint->addItem(tr("No antialiasing"), false );
	
	m_renderHint->setCurrentIndex(1);
	
	addPermanentWidget(m_renderHint/*,1*/);
	
	
	m_renderer = new QComboBox;
	m_renderer->addItem(tr("OpenGL"), KToon::OpenGL );
	m_renderer->addItem(tr("Native"), KToon::Native );
	
	m_renderer->setCurrentIndex(1);
	
	addPermanentWidget(m_renderer/*,1*/);
	
	
	m_brushStatus = new BrushStatus;
	addPermanentWidget(m_brushStatus);
	
	connect(m_renderHint, SIGNAL(activated( int )), this, SLOT(selectRenderHint(int) ));
	connect(m_renderer, SIGNAL(activated(int)), this, SLOT(selectRenderer(int)));
	
	m_brushStatus->setBackground( m_viewDocument->brushManager()->brush() );
	m_brushStatus->setForeground( m_viewDocument->brushManager()->pen() );
}


KTPaintAreaStatus::~KTPaintAreaStatus()
{
}


void KTPaintAreaStatus::selectRenderHint(int id)
{
	m_viewDocument->setAntialiasing( m_renderHint->itemData(id ).toBool() ); 
}

void KTPaintAreaStatus::selectRenderer(int id)
{
	KToon::RenderType type = KToon::RenderType(m_renderHint->itemData(id ).toInt());
	
	if ( type == KToon::OpenGL )
	{
		m_viewDocument->setOpenGL( true );
	}
	else
	{
		m_viewDocument->setOpenGL( false );
	}
}


void KTPaintAreaStatus::setBrush(const QBrush &brush)
{
	m_brushStatus->setBackground( brush );
}

void KTPaintAreaStatus::setPen(const QPen &pen)
{
	m_brushStatus->setForeground( pen );
}


