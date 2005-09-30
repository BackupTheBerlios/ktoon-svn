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
#include <QToolTip>
#include <QLabel>
#include <QLayout>
#include <QHBoxLayout>
#include <QBitmap>

#include "wizard1.xpm"
#include "wizard2.xpm"

#include "ktdebug.h"
#include "kimageeffect.h"

ConfigWizard::ConfigWizard() : KTWizard(0)
{
	setModal(true);
	
	m_firstPage = new CWFirstPage;
	addPage(m_firstPage);
	
	m_secondPage = new CWSecondPage;
	addPage(m_secondPage);
}

ConfigWizard::~ConfigWizard()
{
}

void ConfigWizard::setInitialData(const QString &home, const QString &repos)
{
	m_secondPage->setData(home, repos);
}

QString ConfigWizard::home()
{
	return m_secondPage->home();
}

QString ConfigWizard::repository()
{
	return m_secondPage->repository();
}

// CWFirstPage
CWFirstPage::CWFirstPage(QWidget *parent) : KTWizardPage(tr("Welcome"), parent)
{
	QImage img(wizard1_xpm);
	
	setPixmap( QPixmap(KImageEffect::blend(img, 0.1f, palette().color(QPalette::Background), KImageEffect::DiagonalGradient, true)) );

	QLabel *msg = new QLabel(tr("<h3>KToon</h3><br>"
			"In this wizard you need set a values for proper configuration of ktoon<br><br>"
			"<em>--The KToon Team</em>"));
	
	setWidget(msg);
}

CWFirstPage::~ CWFirstPage()
{
}


// CWFSecondPage
CWSecondPage::CWSecondPage(QWidget *parent) : KTWizardPage(tr("Configure KToon"), parent)
{
	setPixmap( QPixmap(wizard2_xpm) );
	
	KTVHBox *container = new KTVHBox(0, Qt::Vertical);
	container->boxLayout()->setAlignment(Qt::AlignTop);
	
	new QLabel(tr("<h3>Step 1<h3>"), container);
	
	new QLabel(tr("Choose your KToon install directory"), container);
	
	KTVHBox *hbox1 = new KTVHBox(container, Qt::Horizontal);
	
	m_kthome = new QLineEdit("", hbox1);
	connect(m_kthome, SIGNAL(textChanged(const QString &)), this, SLOT(verify(const QString &)));

	QToolTip::add(m_kthome, tr("Choose your KTOON_HOME directory"));
	
	QPushButton *button = new QPushButton(tr("browse..."), hbox1);
	
	QFileDialog *fd = new QFileDialog(hbox1);
	connect(fd, SIGNAL(currentChanged ( const QString & )), m_kthome, SLOT(setText(const QString &)));
	fd->setMode(QFileDialog::Directory);
	fd->setModal(true);
	fd->hide();
	connect(button, SIGNAL(clicked()), fd, SLOT(show()));
	
// 	new QLabel(tr("Choose your project directory"), container);
	
	KTVHBox *hbox2 = new KTVHBox(container, Qt::Horizontal);
	
	m_ktrepos = new QLineEdit("",hbox2);
	connect(m_ktrepos, SIGNAL(textChanged(const QString &)), this, SLOT(verify(const QString &)));
	
	QToolTip::add(m_ktrepos, tr("In this directory will be save your projects"));
	
	QFileDialog *fd2 = new QFileDialog(hbox2);
	connect(fd2, SIGNAL(currentChanged ( const QString & )), m_ktrepos, SLOT(setText(const QString &)));
	
	fd2->hide();
	fd2->setMode(QFileDialog::Directory);
	fd2->setModal(true);
	QPushButton *button2 = new QPushButton(tr("browse..."), hbox2);
	
	connect(button2, SIGNAL(clicked()), fd2, SLOT(show()));
	
	setWidget(container);
}

CWSecondPage::~ CWSecondPage()
{
}

void CWSecondPage::setData(const QString &home, const QString &repos)
{
	m_kthome->setText(home);
	m_ktrepos->setText(repos);
}

QString CWSecondPage::home()
{
	return m_kthome->text();
}

QString CWSecondPage::repository()
{
	return m_ktrepos->text();
}

bool CWSecondPage::isComplete()
{
	bool isOk = false;
	
	if ( m_kthome->text() != "" && m_ktrepos->text() != "" )
	{
		isOk= true;
	}
	return isOk;
};

void CWSecondPage::reset() 
{
	m_kthome->clear();
	m_ktrepos->clear();
}

void CWSecondPage::verify(const QString &)
{
	emit completed();
}

