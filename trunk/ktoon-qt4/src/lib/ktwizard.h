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
#ifndef KTWIZARD_H
#define KTWIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "ktvhbox.h"

class KTWizardPage;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTWizard : public QDialog
{
	Q_OBJECT

	public:
		KTWizard(QWidget *parent = 0);
		~KTWizard();
		KTWizardPage *addPage(KTWizardPage *KTWizardPage);
		void showPage(int index);
		void showPage(KTWizardPage *page);
		
	private slots:
		void back();
		void next();
		void pageCompleted();
		
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

class KTWizardPage : public KTVHBox
{
	Q_OBJECT
	public:
		KTWizardPage(const QString &title, QWidget *parent );
		virtual ~KTWizardPage();
		
		virtual bool isComplete() = 0;
		virtual void reset() = 0;
		
		void setPixmap(const QPixmap &px);
		void setWidget(QWidget *w);
		
	private:
		QFrame *m_container;
		QGridLayout *m_layout;
		QLabel *m_image;

	signals:
		void completed();
};

#endif