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
	
	setObjectName("KTViewCamera_");
	
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
	
	connect(m_animationArea, SIGNAL(progressStep(int, int)), this, SIGNAL(sendProgress(int, int)));
	connect(m_animationArea, SIGNAL(toStatusBar(const QString &, int)), this, SIGNAL(sendMessage( const QString &, int)));
	
	layout->addWidget( animationAreaContainer, 0, Qt::AlignTop | Qt::AlignCenter );
	
#if 0
	KTCameraBar *m_bar = new KTCameraBar;
	layout->addWidget( m_bar, 0, Qt::AlignTop | Qt::AlignCenter );
	m_bar->show();
	
	connect(m_bar, SIGNAL(play()), m_animationArea, SLOT(play()));
	connect(m_bar, SIGNAL(stop()), m_animationArea, SLOT(stop()));
#else
	CCBar *m_bar = new CCBar(40,this);
	layout->addWidget(m_bar, 0, Qt::AlignTop | Qt::AlignCenter );
	m_bar->show();
	
	CCButton *rew = m_bar->addButton(QPixmap(KTOON_HOME+"/images/icons/rw.png" ));
	CCButton *play = m_bar->addButton(QPixmap(KTOON_HOME+"/images/icons/play.png" ));
	CCButton *stop = m_bar->addButton(QPixmap(KTOON_HOME+"/images/icons/stop.png" ));
	CCButton *ff = m_bar->addButton(QPixmap(KTOON_HOME+"/images/icons/ff.png" ));
	
	connect(play, SIGNAL(clicked()), m_animationArea, SLOT(play()));
	connect(stop, SIGNAL(clicked()), m_animationArea, SLOT(stop()));
#endif


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

AAnimationArea *KTViewCamera::animationArea()
{
	return m_animationArea;
}
