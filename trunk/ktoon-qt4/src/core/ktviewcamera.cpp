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

#include "ktviewcamera.h"
#include "ktapplication.h"
#include "ktdebug.h"

KTViewCamera::KTViewCamera(QWidget *parent) : KTMdiWindow(parent)
{
	KTINIT;
	
	setWindowTitle(tr("Render Camera Preview"));
	setWindowIcon( QPixmap(KTOON_HOME+"/images/icons/camera_preview.xpm" ));
	
	m_container = new QFrame(this);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, m_container);
	
	QFrame *animationAreaContainer = new QFrame;
	
	animationAreaContainer->setMidLineWidth(2);
	animationAreaContainer->setLineWidth(2);
	animationAreaContainer->setFrameStyle(QFrame::Box | QFrame::Raised );
	
	QBoxLayout *animationAreaLayout = new QBoxLayout(QBoxLayout::TopToBottom, animationAreaContainer);
	animationAreaLayout->setMargin(0);
	m_animationArea = new AAnimationArea;
	animationAreaLayout->addWidget(m_animationArea);
	
	
	layout->addWidget( animationAreaContainer, 0, Qt::AlignTop | Qt::AlignCenter );
	
	m_bar = new KTCameraBar;
	layout->addWidget( m_bar, 0, Qt::AlignTop | Qt::AlignCenter );
	m_bar->show();
	
	connect(m_bar, SIGNAL(play()), m_animationArea, SLOT(play()));
	connect(m_bar, SIGNAL(stop()), m_animationArea, SLOT(stop()));
	
	m_container->setLayout(layout);
	
	setCentralWidget(m_container);
	
// 	QPalette pal = palette();
// 	pal.setColor(QPalette::Background, pal.color(QPalette::Foreground) );
// 	setPalette(pal);
}


KTViewCamera::~KTViewCamera()
{
	KTEND;
}

// QSize KTViewCamera::sizeHint() const
// {
// 	return m_container->sizeHint();
// }

AAnimationArea *KTViewCamera::animationArea()
{
	return m_animationArea;
}
