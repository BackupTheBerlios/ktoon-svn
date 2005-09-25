/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#ifndef CONFIGWIZARD_H
#define CONFIGWIZARD_H

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDialog>

#include "ktvhbox.h"

#include "q3wizard.h"

class CWFirstPage;
class CWSecondPage;

/**
This class helps to config KToon!

@author David Cuadrado
*/
class ConfigWizard : public Q3Wizard
{
	Q_OBJECT
	public:
    		ConfigWizard();
    		~ConfigWizard();
		
		void setInitialData(const QString &home, const QString &repos);
		QString getHome();
		QString getRepos();
		
	private slots:
		void next();
		void finished();

	private:
		CWFirstPage *m_firstPage;
		CWSecondPage *m_secondPage;
		
		QString kthome;
		QString ktrepos;
};

class CWFirstPage : public KTVHBox
{
	Q_OBJECT
	public:
		CWFirstPage(QWidget *parent = 0, const char *name = 0);
		~CWFirstPage();
};

class CWSecondPage : public KTVHBox
{
	Q_OBJECT
	public:
		CWSecondPage(QWidget *parent = 0, const char *name = 0);
		~CWSecondPage();
		void setData(const QString &home, const QString &repos);
		QString getHome();
		QString getRepos();
		
	private:
		QLineEdit *m_kthome;
		QLineEdit *m_ktrepos;
};

#endif
