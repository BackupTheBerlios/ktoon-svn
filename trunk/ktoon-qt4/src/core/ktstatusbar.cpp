//
// C++ Implementation: ktstatusbar
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
