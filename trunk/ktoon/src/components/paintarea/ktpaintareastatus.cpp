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

#include <kgui/kseparator.h>

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

struct KTPaintAreaStatus::Private
{
	KTViewDocument *viewDocument;
	QPushButton *antialiasHint;
	QComboBox *renderer;
	BrushStatus *brushStatus;
	QComboBox *rotation;
};

KTPaintAreaStatus::KTPaintAreaStatus(KTViewDocument *parent) : QStatusBar(parent), d( new Private)
{
	setSizeGripEnabled(false);
	d->viewDocument = parent;
	QWidget *rotContainer = new QWidget;
	QHBoxLayout *rotLayout = new QHBoxLayout(rotContainer);
	rotLayout->setSpacing(3);
	rotLayout->setMargin(1);
	
	rotLayout->addWidget(new QLabel(tr("Rotate")));
	
	d->rotation = new QComboBox();
	d->rotation->setDuplicatesEnabled(false);
	d->rotation->setEditable(true);
	
	
	for(int i = 0; i < 360; i+=30)
	{
		d->rotation->addItem(QString::number(i), i);
	}
	
	d->rotation->setValidator(new QIntValidator(-360, 360,this));
	
	rotLayout->addWidget( d->rotation);
	
	addPermanentWidget(rotContainer);
	
	connect(d->rotation, SIGNAL(activated(const QString &)), this, SLOT(applyRotationFromItem(const QString &)));
	
	
	
	
	
	///////
	
	d->antialiasHint = new QPushButton;
	d->antialiasHint->setFocusPolicy( Qt::NoFocus);
	
	d->antialiasHint->setText(tr("Antialiasing"));
	d->antialiasHint->setCheckable(true);
	
	d->antialiasHint->setChecked( parent->renderHints() & QPainter::Antialiasing );
	
	addPermanentWidget(d->antialiasHint/*,1*/);
	
	
	d->renderer = new QComboBox;
#ifdef QT_OPENGL_LIB
	d->renderer->addItem(tr("OpenGL"), KToon::OpenGL );
#endif
	d->renderer->addItem(tr("Native"), KToon::Native );
	
	d->renderer->setCurrentIndex(1);
	
	addPermanentWidget(d->renderer/*,1*/);
	
	
	d->brushStatus = new BrushStatus;
	addPermanentWidget(d->brushStatus);
	
	connect(d->antialiasHint, SIGNAL(toggled(bool)), this, SLOT(selectAntialiasingHint(bool) ));
	connect(d->renderer, SIGNAL(activated(int)), this, SLOT(selectRenderer(int)));
	
	d->brushStatus->setBackground( d->viewDocument->brushManager()->brush() );
	d->brushStatus->setForeground( d->viewDocument->brushManager()->pen() );
}


KTPaintAreaStatus::~KTPaintAreaStatus()
{
	delete d;
}


void KTPaintAreaStatus::selectAntialiasingHint(bool use)
{
	d->viewDocument->setAntialiasing( use ); 
}

void KTPaintAreaStatus::selectRenderer(int id)
{
	KToon::RenderType type = KToon::RenderType(d->renderer->itemData(id ).toInt());
	
	if ( type == KToon::OpenGL )
	{
		d->viewDocument->setOpenGL( true );
	}
	else
	{
		d->viewDocument->setOpenGL( false );
	}
}


void KTPaintAreaStatus::setBrush(const QBrush &brush)
{
	d->brushStatus->setBackground( brush );
}

void KTPaintAreaStatus::setPen(const QPen &pen)
{
	d->brushStatus->setForeground( pen );
}


void KTPaintAreaStatus::applyRotationFromItem(const QString & text)
{
	int angle = text.toInt();
	
	if ( angle < 0 )
	{
		angle += 360;
	}
	
	d->viewDocument->setRotationAngle(angle);
}


