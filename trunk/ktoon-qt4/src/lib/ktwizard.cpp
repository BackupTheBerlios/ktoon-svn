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
#include "ktwizard.h"

#include "ktvhbox.h"
#include "kseparator.h"
#include "ktdebug.h"

// Qt
#include <QLabel>
#include <QBitmap>

KTWizard::KTWizard(QWidget *parent) : QDialog(parent)
{
	m_cancelButton = new QPushButton(tr("Cancel"));
	m_backButton = new QPushButton(tr("< &Back"));
	m_nextButton = new QPushButton(tr("Next >"));
	m_finishButton = new QPushButton(tr("&Finish"));

	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_backButton, SIGNAL(clicked()), this, SLOT(back()));
	connect(m_nextButton, SIGNAL(clicked()), this, SLOT(next()));
	connect(m_finishButton, SIGNAL(clicked()), this, SLOT(accept()));

	m_buttonLayout = new QHBoxLayout;
	m_buttonLayout->addStretch(1);
	m_buttonLayout->addWidget(m_cancelButton);
	m_buttonLayout->addWidget(m_backButton);
	m_buttonLayout->addWidget(m_nextButton);
	m_buttonLayout->addWidget(m_finishButton);

	m_mainLayout = new QVBoxLayout;
	m_mainLayout->addWidget(&m_history);
	m_mainLayout->addLayout(m_buttonLayout);
	setLayout(m_mainLayout);
}


KTWizard::~KTWizard()
{
}

KTWizardPage *KTWizard::addPage(KTWizardPage *newPage)
{
	newPage->setParent(&m_history);
	newPage->show();
	
	m_history.addWidget(newPage);
	
	if( m_history.count() == 1) // First Page
	{
		newPage->setFocus();
		m_backButton->setEnabled(false);
		m_nextButton->setDefault(true);
// 		m_finishButton->setEnabled(false);
	}
	
// 	ktDebug() << "ENABLE: " << newPage->isComplete();
// 	m_nextButton->setEnabled( newPage->isComplete() );
	
	connect(newPage, SIGNAL(completed()), this, SLOT(pageCompleted()));
	
	return newPage;
}

void KTWizard::showPage(KTWizardPage *page)
{
	m_history.setCurrentWidget(page);
}

void KTWizard::showPage(int index)
{
	m_history.setCurrentIndex(index);
}

void KTWizard::back()
{
	m_history.setCurrentIndex(m_history.currentIndex()-1);
	
	if ( m_history.currentIndex() == 0 )
	{
		m_nextButton->setEnabled(true);
		m_finishButton->setEnabled(false);
		m_backButton->setEnabled(false);
		
		m_nextButton->setDefault(true);
	} else 
	{
		m_nextButton->setDefault(true);
		m_finishButton->setEnabled(false);
	}
}

void KTWizard::next()
{
	m_history.setCurrentIndex(m_history.currentIndex()+1);
	
	ktDebug() << m_history.currentIndex() << " " << m_history.count() << endl;
	
	if ( m_history.currentIndex() == m_history.count()-1 )
	{
		m_nextButton->setEnabled(false);
// 		m_finishButton->setEnabled(true);
		m_backButton->setEnabled(true);
		
		m_finishButton->setDefault(true);
	} else 
	{
		m_finishButton->setEnabled(false);
	}
	
	pageCompleted();
}

void KTWizard::pageCompleted()
{
	KTWizardPage *current = qobject_cast<KTWizardPage *>(m_history.currentWidget());
	
	if ( m_history.currentIndex() == m_history.count()-1 )
	{
		m_finishButton->setEnabled(current->isComplete());
	}
	else
	{
		m_nextButton->setEnabled(current->isComplete());
	}
}

KTWizardPage::KTWizardPage(const QString &title, QWidget *parent) : KTVHBox(parent)
{
	KTVHBox *theTitle = new KTVHBox(this, Qt::Vertical);
	new QLabel(title, theTitle);
	new KSeparator(theTitle);
	boxLayout()->setAlignment(theTitle, Qt::AlignTop);
	
	m_container = new QFrame(this);
	
	m_layout = new QGridLayout(m_container);
	
	m_image = new QLabel;
	m_layout->addWidget(m_image, 0, 0, Qt::AlignLeft);
	m_image->hide();
	
	new KSeparator(this);
	
	hide();
}

void KTWizardPage::setPixmap(const QPixmap &px)
{
	m_image->setPixmap(px);
// 	m_image->setMask(px.mask());
	m_image->show();
}

void KTWizardPage::setWidget(QWidget *w)
{
	m_layout->addWidget(w, 0, 1, Qt::AlignTop);
}

KTWizardPage::~KTWizardPage() {};
