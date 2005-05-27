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
#include "configwizard.h"

ConfigWizard::ConfigWizard() : QWizard()
{	
	m_firstPage = new CWFirstPage(this);
	addPage(m_firstPage, tr("Welcome"));
	
	m_secondPage = new CWSecondPage(this);
	addPage(m_secondPage, tr("Configure KToon"));
	
	setFinishEnabled(m_secondPage, true);
	
	connect( finishButton (), SIGNAL(clicked()), this, SLOT(finished()));
}

ConfigWizard::~ConfigWizard()
{
}

void ConfigWizard::next()
{
	QWizard::next();
}

void ConfigWizard::finished()
{
}

void ConfigWizard::setInitialData(const QString &home, const QString &repos)
{
	m_secondPage->setData(home, repos);
}

QString ConfigWizard::getHome()
{
	return m_secondPage->getHome();
}

QString ConfigWizard::getRepos()
{
	return m_secondPage->getRepos();
}

// CWFirstPage
CWFirstPage::CWFirstPage(QWidget *parent, const char *name) : QVBox(parent, name)
{
	new QLabel(tr("<h3>KToon</h3>"
			"In this wizard you need set a values for proper configuration of ktoon<br><br>"
			"<em>--The KToon Team</em>"), this);
}

CWFirstPage::~ CWFirstPage()
{
}


// CWFSecondPage
CWSecondPage::CWSecondPage(QWidget *parent, const char *name) : QVBox(parent, name)
{
	new QLabel(tr("<h3>Step 1<h3>"), this);
	
	new QLabel(tr("Choose your KToon install directory"), this);
	
	QHBox *hbox1 = new QHBox(this);
	m_kthome = new QLineEdit(hbox1);
	QPushButton *button = new QPushButton(tr("browse..."), hbox1);
	QFileDialog *fd = new QFileDialog(hbox1);
	connect(fd, SIGNAL(dirEntered( const QString & )), m_kthome, SLOT(setText(const QString &)));
	fd->setMode(QFileDialog::Directory);
	fd->setModal(true);
	fd->hide();
	connect(button, SIGNAL(clicked()), fd, SLOT(show()));
	
	new QLabel(tr("Choose your project directory"), this);
	
	QHBox *hbox2 = new QHBox(this);
	m_ktrepos = new QLineEdit(hbox2);
	QFileDialog *fd2 = new QFileDialog(hbox2);
	connect(fd2, SIGNAL(dirEntered( const QString & )), m_ktrepos, SLOT(setText(const QString &)));
	
	fd2->hide();
	fd2->setMode(QFileDialog::Directory);
	fd2->setModal(true);
	QPushButton *button2 = new QPushButton(tr("browse..."), hbox2);
	connect(button2, SIGNAL(clicked()), fd2, SLOT(show()));
}

CWSecondPage::~ CWSecondPage()
{
}

void CWSecondPage::setData(const QString &home, const QString &repos)
{
	m_kthome->setText(home);
	m_ktrepos->setText(repos);
}

QString CWSecondPage::getHome()
{
	return m_kthome->text();
}

QString CWSecondPage::getRepos()
{
	return m_ktrepos->text();
}
