//
// C++ Interface: ktstatusbar
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTSTATUSBAR_H
#define KTSTATUSBAR_H

#include <q3progressbar.h>
#include <qstatusbar.h>
#include <qlabel.h>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTStatusBar : public QStatusBar
{
	Q_OBJECT
	public:
		KTStatusBar(QWidget *parent);
		~KTStatusBar();
		void addWidget ( QWidget *widget, int stretch = 0, bool permanent = FALSE );
		
	public slots:
		void setStatus(const QString &status);
		void advance(int step);
		
	private:
		QLabel *m_status;
		Q3ProgressBar *m_progressBar;

};

#endif
