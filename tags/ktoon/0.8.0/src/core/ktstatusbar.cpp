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

#include "ktstatusbar.h"
#include "ddebug.h"

KTStatusBar::KTStatusBar(QWidget *parent)
 : QStatusBar(parent)
{
	setObjectName("KTStatusBar_");
	
	m_progressBar = new QProgressBar(this);
	m_progressBar->setMaximumSize(100, m_progressBar->height()/2);
	m_progressBar->setMinimumWidth(100);
	
	m_progressBar->setMaximum(10);
	
	m_status = new QLabel( this );
// 	m_status->setFont(QFont("Times", 8));
	m_status->setIndent(10);
// 	m_status->setMaximumWidth(180);
	
	addPermanentWidget(m_status, 1);
	addPermanentWidget(m_progressBar, 0);
	
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(clear()));
}


KTStatusBar::~KTStatusBar()
{
}

void KTStatusBar::clear()
{
	advance(0);
	
	m_timer->stop();
}

void KTStatusBar::setStatus(const QString &status, int ms )
{
	m_status->setText(status);
	
	m_status->repaint();
	
	if ( ms > 0)
	{
		if ( m_timer->isActive() )
			m_timer->stop();
		m_timer->start(ms);
	}
}

void KTStatusBar::addWidget ( QWidget *widget, int stretch, bool permanent)
{
	if ( permanent )
	{
		QStatusBar::addPermanentWidget(widget,stretch);
	}
	else
	{
		QStatusBar::addWidget(widget,stretch);
	}
	
	if(widget->sizeHint().height() + 4 > height())
	{
// 		setFixedHeight(widget->sizeHint().height() + 4);
	}
}

void KTStatusBar::advance(int step, int totalSteps)
{
	if ( totalSteps != -1 )
	{
		m_progressBar->setMaximum(totalSteps);
	}
	
	m_progressBar->setValue(step);
}
