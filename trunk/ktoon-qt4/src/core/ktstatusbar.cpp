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

KTStatusBar::KTStatusBar(QWidget *parent)
 : QStatusBar(parent)
{
	m_progressBar = new QProgressBar(this);
	m_progressBar->setMaximumSize(100, m_progressBar->height()/2);
	m_progressBar->setMinimumWidth(100);
	
	m_progressBar->setMaximum(10);
	
	m_status = new QLabel( "<b></b>",this );
	m_status->setIndent(10);
// 	m_status->setMaximumWidth(180);
	
	addPermanentWidget(m_status, 1);
	addPermanentWidget(m_progressBar, 0);
}


KTStatusBar::~KTStatusBar()
{
}

void KTStatusBar::setStatus(const QString &status)
{
	m_status->setText("<b>"+status+"</b>");
}

void KTStatusBar::addWidget ( QWidget *widget, int stretch, bool permanent)
{
	QStatusBar::addWidget(widget,stretch,permanent);

	if(widget->sizeHint().height() + 4 > height())
	{
		setFixedHeight(widget->sizeHint().height() + 4);
	}
}

void KTStatusBar::advance(int step)
{
	m_progressBar->setValue(step);
}
