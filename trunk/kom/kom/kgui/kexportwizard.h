/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KEXPORTWIZARD_H
#define KEXPORTWIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "kgui/kvhbox.h"
#include <kcore/kglobal.h>
#include <kgui/ktoolview.h>

class KExportWizardPage;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KExportWizard : public QDialog
{
	Q_OBJECT;
	
	public:
		KExportWizard(QWidget *parent = 0);
		~KExportWizard();
		KExportWizardPage *addPage(KExportWizardPage *page);
		void showPage(int index);
		void showPage(KExportWizardPage *page);
		
	private slots:
		void cancel();
		void back();
		void next();
		void pageCompleted();
	signals:
		void cancelled();
		void updateScenes();
	private:
		QStackedWidget m_history;
		QPushButton *m_cancelButton;
		QPushButton *m_backButton;
		QPushButton *m_nextButton;
		QHBoxLayout *m_buttonLayout;
		QVBoxLayout *m_mainLayout;


};

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class KExportWizardPage : public KVHBox
{
	Q_OBJECT
	public:
		KExportWizardPage(const QString &title, QWidget *parent = 0 );
		virtual ~KExportWizardPage();
		
		virtual bool isComplete() const = 0;
		virtual void reset() = 0;
		
		void setPixmap(const QPixmap &px);
		void setWidget(QWidget *w);
		
	public slots:
		virtual void aboutToNextPage() {}
		virtual void aboutToBackPage() {}
	signals:
		void completed();
	private:
		QFrame *m_container;
		QGridLayout *m_layout;
		QLabel *m_image;


};

#endif
