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
//Added by qt3to4:
#include <QLabel>

KTStatusBar::KTStatusBar(QWidget *parent)
 : QStatusBar(parent)
{
	m_progressBar = new Q3ProgressBar(this);
	m_progressBar->setMaximumSize(100, m_progressBar->height()/2);
	
	m_progressBar->setTotalSteps(10);
	
	m_status = new QLabel( "<b></b>",this );
	m_status->setMaximumWidth(180);
	
	addWidget(m_status, 1, true);
	addWidget(m_progressBar, 0, true);
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
	m_progressBar->setProgress(step);
}
