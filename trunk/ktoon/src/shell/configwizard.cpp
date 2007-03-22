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

#include <QLabel>
#include <QLayout>
#include <QHBoxLayout>
#include <QBitmap>
#include <QLineEdit>
#include <QFileDialog>

#include "wizard1.xpm"
#include "wizard2.xpm"

#include <dcore/ddebug.h>
#include <dcore/dimageeffect.h>

#include <dcore/dglobal.h>

ConfigWizard::ConfigWizard() : DWizard(0)
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

QString ConfigWizard::home() const
{
	return m_secondPage->home();
}

QString ConfigWizard::cache() const
{
	return m_secondPage->repository();
}

// CWFirstPage
CWFirstPage::CWFirstPage(QWidget *parent) : DWizardPage(tr("Welcome"), parent)
{
	QImage img(wizard1_xpm);
	
	setPixmap( QPixmap::fromImage(/* ::blend(img, 0.1f, palette().color(QPalette::Background), DImageEffect::DiagonalGradient, true)*/img) );

	QLabel *msg = new QLabel(tr("<h1>Welcome to the KToon Configuration wizard!</h1><br><br>"
			"<table border=0 align=right ><tr><td>- KToon is a 2D Animation Toolkit designed by animators for animators.</td></tr><tr><td>- KToon is free/open software and it is covered under the GNU GPL license terms.</td></tr><tr><td>- KToon is Software Project developed by Toonka Films (http://www.toonka.com)</td></tr></table><br><br>"

			"This is an \"easy to use tool\" to allow you to configure your KToon environment.<br><br>"

			"Please, choose the \"Next\" button to setting up some parameters<br> required to use KToon correctly or use the \"Cancel\" button<br> to close this dialog. Enjoy KToon!<br><br><br>"

			"<em>--The KToon Team</em><br><br>"));
	
	setWidget(msg);
}

CWFirstPage::~ CWFirstPage()
{
}


// CWFSecondPage
CWSecondPage::CWSecondPage(QWidget *parent) : DWizardPage(tr("Configure KToon"), parent)
{
	setPixmap( QPixmap(wizard2_xpm) );
	
	DVHBox *container = new DVHBox(0, Qt::Vertical);
	container->boxLayout()->setAlignment(Qt::AlignTop);
	
	new QLabel(tr("<h3>Step 1<h3>"), container);
	
	new QLabel(tr("Choose your KToon installation directory"), container);
	
	DVHBox *hbox1 = new DVHBox(container, Qt::Horizontal);
	
	m_kthome = new QLineEdit(QString::fromLocal8Bit(::getenv("KTOON_HOME")), hbox1);
	
	connect(m_kthome, SIGNAL(textChanged(const QString &)), this, SLOT(verify(const QString &)));

	m_kthome->setToolTip(tr("Choose the directory where KToon is installed"));
	
	QPushButton *button = new QPushButton(tr("Browse..."), hbox1);
	
	QFileDialog *fd = new QFileDialog(hbox1);
	connect(fd, SIGNAL(currentChanged ( const QString & )), m_kthome, SLOT(setText(const QString &)));
	fd->setFileMode(QFileDialog::Directory);
	fd->setModal(true);
	fd->hide();
	connect(button, SIGNAL(clicked()), fd, SLOT(show()));
	
	new QLabel(tr("Choose a temporal directory"), container);
	
	DVHBox *hbox2 = new DVHBox(container, Qt::Horizontal);
	
	m_ktrepos = new QLineEdit(QDir::tempPath(),hbox2);
	
	connect(m_ktrepos, SIGNAL(textChanged(const QString &)), this, SLOT(verify(const QString &)));
	
	m_ktrepos->setToolTip(tr("Choose the directory for temporal files"));
	
	QFileDialog *fd2 = new QFileDialog(hbox2);
	connect(fd2, SIGNAL(currentChanged ( const QString & )), m_ktrepos, SLOT(setText(const QString &)));
	
	fd2->hide();
	fd2->setFileMode(QFileDialog::Directory);
	fd2->setModal(true);
	QPushButton *button2 = new QPushButton(tr("Browse..."), hbox2);
	
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

QString CWSecondPage::home() const
{
	return m_kthome->text();
}

QString CWSecondPage::repository() const
{
	return m_ktrepos->text();
}

bool CWSecondPage::isComplete() const
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

