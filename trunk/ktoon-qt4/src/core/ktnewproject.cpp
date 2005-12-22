/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#include "ktnewproject.h"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>

KTNewProject::KTNewProject(QWidget *parent)
 : KTWizard(parent)
{
	setModal(true);
	m_firstPage = new NPFirstPage;
	connect( m_firstPage, SIGNAL(sendToOSD( const QString& )), this, SIGNAL(sendToOSD( const QString& )));
	addPage(m_firstPage);
}


KTNewProject::~KTNewProject()
{
}

QString KTNewProject::projectName() const
{
	return m_firstPage->projectName();
}

QSize KTNewProject::dimension() const
{
	return m_firstPage->dimension();
}

// NPFirstPage

NPFirstPage::NPFirstPage(QWidget *parent) : KTWizardPage(tr("New KToon Project"), parent)
{
	QFrame *container = new QFrame(this);
	QGridLayout *layout = new QGridLayout(container);
	
	QLabel *labelProjectName = new QLabel(tr("Project Name"), container);
	layout->addWidget(labelProjectName, 0, 0);
	
	m_projectName = new QLineEdit( container );
	layout->addWidget(m_projectName, 0, 1);
	
	QLabel *labelAuthorName = new QLabel(tr("Author"), container);
	layout->addWidget(labelAuthorName, 1, 0);
	
	m_authorName = new QLineEdit( container );
	layout->addWidget(m_authorName, 1, 1);
	
	m_size = new KTXYSpinBox("Dimension", container);
	m_size->setMaximum( 1000);
	m_size->setModifyTogether(true);;
	m_size->setX( 520);
	m_size->setY( 340);
	layout->addWidget(m_size, 2, 0);
	
	setWidget(container);
	
}

NPFirstPage::~NPFirstPage()
{
	
}

bool NPFirstPage::isComplete()
{
	bool ok = true;
	
	if(m_projectName->text().isEmpty())
	{
		emit sendToOSD(tr("field project name is empty"));
		ok = false;
	}
	return ok;
}

void NPFirstPage::reset()
{
	
}

QString NPFirstPage::projectName() const
{
	return m_projectName->text();
}

QSize NPFirstPage::dimension() const
{
	QSize size( m_size->x(), m_size->y() );
	return size;
}
