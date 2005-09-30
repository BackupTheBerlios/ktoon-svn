//
// C++ Interface: kttabdialog
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTTABDIALOG_H
#define KTTABDIALOG_H

#include <QDialog>
#include <QTabWidget>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTTabDialog : public QDialog
{
	Q_OBJECT
	public:
		KTTabDialog(QWidget *parent = 0);
		~KTTabDialog();
		
		void addTab ( QWidget * child, const QString & label );
		void addTab ( QWidget * child, const QIcon & iconset, const QString & label );
		
	private:
		QTabWidget *m_tabWidget;
};

#endif
