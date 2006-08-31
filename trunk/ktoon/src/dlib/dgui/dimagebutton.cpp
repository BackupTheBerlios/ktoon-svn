/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dimagebutton.h"

#include <QPainter>
#include <QPixmap>
#include <QLayout>
#include <QBitmap>

#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "ddebug.h"

class DImageButton::Animation
{
	public:
		Animation(int initialSize) : aSize(initialSize), aBeginning(true), m_interval(80)
		{
			aTimer = new QTimer;
		}
		~Animation() {};
		
		void begin()
		{
			aTimer->start(m_interval);
		}
		void end()
		{
			aTimer->stop();
		}
		
		QTimer *aTimer;
		int aSize;
		bool aBeginning;
		
	private:
		int m_interval;
};

DImageButton::DImageButton(const QIcon &icon, int size, QWidget *parent, bool animate ) : QPushButton(parent), m_imageSize(size), m_animator(0), m_isAnimated(animate)
{
	setup();
	setImage(icon);
}

DImageButton::DImageButton(const QIcon &icon, int size, QObject *reciever, const char *method, QWidget *parent, bool animate ) : QPushButton(parent), m_imageSize(size), m_animator(0), m_isAnimated(animate)
{
	setup();
	setImage(icon);
	
	connect(this, SIGNAL(clicked()), reciever, method);
}


DImageButton::~DImageButton()
{
	if ( m_isAnimated )
	{
		delete m_animator;
	}
}

void DImageButton::setup()
{
	setFlat( true );
	setAutoDefault( false );
	setIconSize( QSize(m_imageSize, m_imageSize) );
	setMaximumSize(m_imageSize, m_imageSize);
	setMinimumSize(m_imageSize, m_imageSize);
	m_animator = new Animation(m_imageSize);
	connect(m_animator->aTimer, SIGNAL(timeout()), this, SLOT(animate()));
	
	setFocusPolicy( Qt::NoFocus );
}

void DImageButton::enterEvent(QEvent *)
{
// 	setIconSize( QSize(m_imageSize-10,m_imageSize-10) );
	
	if ( m_isAnimated )
	{
		m_animator->begin();
		m_animator->aBeginning = true;
		
		if ( m_animator->aSize >= m_imageSize + 10 )
		{
			m_animator->aSize = m_imageSize;
		}
	}
	else
	{
		setFlat(false);
	}
}

void DImageButton::leaveEvent(QEvent *)
{
	if ( m_isAnimated && !isDown() )
	{
		m_animator->aBeginning = false;
		m_animator->aSize = m_imageSize;
		m_animator->end();
		
		setIconSize( QSize(m_imageSize,m_imageSize));
	}
	else
	{
		setFlat(true);
	}
}

void DImageButton::animate()
{
	if ( m_isAnimated )
	{
		if ( isDown() )
		{
			m_animator->end();
		}
		
		if ( m_animator->aBeginning ) // Icon grow up
		{
			m_animator->aSize-=1;
			
			setIconSize( QSize(m_animator->aSize,m_animator->aSize));
		}
		else
		{
			m_animator->aSize+=1;
			
			setIconSize( QSize(m_animator->aSize,m_animator->aSize));
		}
		
		if ( m_animator->aSize > m_imageSize+4 || m_animator->aSize < m_imageSize-4 )
		{
			m_animator->aBeginning = !m_animator->aBeginning;
		}
	}
}

// void DImageButton::resizeEvent(QResizeEvent *e)
// {
// 	QPainter painter( &m_drawer );
// 
// 	painter.setBackgroundColor( paletteBackgroundColor() );
// 	painter.eraseRect( m_drawer.rect() );
// 
// 	painter.end();
// 	
// 
// 	QPixmap buffer = getPixmap();
// 
// 	bitBlt( &m_drawer, size().width(), size().height(), &buffer, 0, 0, buffer.width(), buffer.height(), Qt::CopyROP );
// 
// 	repaint( false );
// }


void DImageButton::setImage ( const QIcon & icon)
{
	setIconSize( QSize(m_imageSize,m_imageSize) );
	QPushButton::setIcon(icon);
}

// void DImageButton::paintEvent(QPaintEvent *e)
// {
// 	QPushButton::paintEvent(e);
// 	
// 	QStyleOptionButton opt;
// 	opt.init(this);
// 	opt.iconSize = QSize(m_imageSize,m_imageSize);
// 	
// 	QStylePainter p(this);
// 	p.drawControl(QStyle::CE_PushButton, opt);
// }


void DImageButton::setAnimated(bool anim)
{
	m_isAnimated = anim;
}

