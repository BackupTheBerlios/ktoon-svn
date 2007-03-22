/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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
#ifndef DWIZARD_H
#define DWIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "dgui/dvhbox.h"
#include <dcore/dglobal.h>

class DWizardPage;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_GUI_EXPORT DWizard : public QDialog
{
	Q_OBJECT

	public:
		DWizard(QWidget *parent = 0);
		~DWizard();
		DWizardPage *addPage(DWizardPage *DWizardPage);
		void showPage(int index);
		void showPage(DWizardPage *page);
		
	private slots:
		void back();
		void next();
		void pageCompleted();
		void finish();
		
	private:
		QStackedWidget m_history;
		QPushButton *m_cancelButton;
		QPushButton *m_backButton;
		QPushButton *m_nextButton;
		QPushButton *m_finishButton;
		QHBoxLayout *m_buttonLayout;
		QVBoxLayout *m_mainLayout;


};

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class DWizardPage : public DVHBox
{
	Q_OBJECT
	public:
		DWizardPage(const QString &title, QWidget *parent );
		virtual ~DWizardPage();
		
		virtual bool isComplete() const = 0;
		virtual void reset() = 0;
		
		void setPixmap(const QPixmap &px);
		void setWidget(QWidget *w);
		
	public slots:
		virtual void aboutToFinish() {};
		
	private:
		QFrame *m_container;
		QGridLayout *m_layout;
		QLabel *m_image;

	signals:
		void completed();
};

#endif
