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

#include "ktcamerabar.h"
#include <QBoxLayout>

#include <dcore/ddebug.h>
#include <dcore/dglobal.h>

KTCameraBar::KTCameraBar(QWidget *parent)
 : QFrame(parent)
{
	DINIT;
	setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
	setMidLineWidth(2);
	setLineWidth (1);
	
	QBoxLayout *m_mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, parent);
	m_mainLayout->addStretch(1);
	
	m_mainLayout->setSpacing(0);
	m_mainLayout->setMargin(0);
	
	m_rew = new DImageButton(QPixmap(THEME_DIR+"/icons/rw.png"), 33,this, true);
	m_mainLayout->addWidget(m_rew);
	connect(m_rew, SIGNAL(clicked()), this, SIGNAL(rew()));
	
	m_play = new DImageButton(QPixmap(THEME_DIR+"/icons/play.png"), 33,this, true);
	m_mainLayout->addWidget(m_play);
	connect(m_play, SIGNAL(clicked()), this, SIGNAL(play()));
	
	m_stop = new DImageButton(QPixmap(THEME_DIR+"/icons/stop.png"), 33,this, true);
	m_mainLayout->addWidget(m_stop);
	connect(m_stop, SIGNAL(clicked()), this, SIGNAL(stop()));
	
	m_ff = new DImageButton(QPixmap(THEME_DIR+"/icons/ff.png"), 33,this, true);
	m_mainLayout->addWidget(m_ff);
	connect(m_ff, SIGNAL(clicked()), this, SIGNAL(ff()));
	
	setLayout(m_mainLayout);
}


KTCameraBar::~KTCameraBar()
{
	DEND;
}

void KTCameraBar::setPalette(const QPalette &)
{
}
