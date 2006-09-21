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
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QIntValidator>

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
	update();
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
	
	QWidget *rotContainer = new QWidget;
	QHBoxLayout *rotLayout = new QHBoxLayout(rotContainer);
	rotLayout->setSpacing(3);
	rotLayout->setMargin(1);
	
	rotLayout->addWidget(new QLabel(tr("Rotate")));
	
	m_rotation = new QComboBox();
	m_rotation->setDuplicatesEnabled(false);
	m_rotation->setEditable(true);
	
	
	for(int i = 0; i < 360; i+=30)
	{
		m_rotation->addItem(QString::number(i), i);
	}
	
	m_rotation->setValidator(new QIntValidator(0, 360,this));
	
	rotLayout->addWidget( m_rotation);
	
	addPermanentWidget(rotContainer);
	
	connect(m_rotation, SIGNAL(activated(const QString &)), this, SLOT(applyRotationFromItem(const QString &)));
	
	
	
	
	
	///////
	
	m_antialiasHint = new QPushButton;
	m_antialiasHint->setFocusPolicy( Qt::NoFocus);
	
	m_antialiasHint->setText(tr("Antialiasing"));
	m_antialiasHint->setCheckable(true);
	
	m_antialiasHint->setChecked( parent->renderHints() & QPainter::Antialiasing );
	
	addPermanentWidget(m_antialiasHint/*,1*/);
	
	
	m_renderer = new QComboBox;
#ifdef QT_OPENGL_LIB
	m_renderer->addItem(tr("OpenGL"), KToon::OpenGL );
#endif
	m_renderer->addItem(tr("Native"), KToon::Native );
	
	m_renderer->setCurrentIndex(1);
	
	addPermanentWidget(m_renderer/*,1*/);
	
	
	m_brushStatus = new BrushStatus;
	addPermanentWidget(m_brushStatus);
	
	connect(m_antialiasHint, SIGNAL(toggled(bool)), this, SLOT(selectAntialiasingHint(bool) ));
	connect(m_renderer, SIGNAL(activated(int)), this, SLOT(selectRenderer(int)));
	
	m_brushStatus->setBackground( m_viewDocument->brushManager()->brush() );
	m_brushStatus->setForeground( m_viewDocument->brushManager()->pen() );
}


KTPaintAreaStatus::~KTPaintAreaStatus()
{
}


void KTPaintAreaStatus::selectAntialiasingHint(bool use)
{
	m_viewDocument->setAntialiasing( use ); 
}

void KTPaintAreaStatus::selectRenderer(int id)
{
	KToon::RenderType type = KToon::RenderType(m_renderer->itemData(id ).toInt());
	
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


void KTPaintAreaStatus::applyRotationFromItem(const QString & text)
{
	int angle = text.toInt();
	
	m_viewDocument->setRotationAngle(angle);
}


