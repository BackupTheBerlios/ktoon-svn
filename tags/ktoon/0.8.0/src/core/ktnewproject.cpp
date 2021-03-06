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

KTNewProject::KTNewProject(QWidget *parent)
	: DTabDialog(parent)
{
	setWindowTitle(tr("Create a new project"));
	setModal(true);
	
	QFrame *container = new QFrame();
	QGridLayout *layout = new QGridLayout(container);
	
	QLabel *labelProjectName = new QLabel(tr("Project Name"), container);
	layout->addWidget(labelProjectName, 0, 0);
	
	m_projectName = new QLineEdit( container );
	layout->addWidget(m_projectName, 0, 1);
	
	QLabel *labelAuthorName = new QLabel(tr("Author"), container);
	layout->addWidget(labelAuthorName, 1, 0);
	
	m_authorName = new QLineEdit( container );
	layout->addWidget(m_authorName, 1, 1);
	
	m_size = new DXYSpinBox("Dimension", container);
	m_size->setMaximum( 1000);
	m_size->setModifyTogether(true);;
	
	m_size->setX( 520 );
	m_size->setY( 340);
	
	layout->addWidget(m_size, 2, 0);
	
	QGroupBox *renderAndFps= new QGroupBox();
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	renderAndFps->setLayout(subLayout);
	
	QBoxLayout *fpsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	QLabel *label = new QLabel(tr("FPS"));
	m_fps = new QSpinBox();
	m_fps->setValue(24);
	
	fpsLayout->addWidget(label);
	fpsLayout->addWidget(m_fps);
	subLayout->addLayout(fpsLayout);
	
	layout->addWidget(renderAndFps, 2, 1);
	addTab( container, tr("Project info"));
	
	
}
KTNewProject::~KTNewProject()
{
}

QString KTNewProject::projectName() const
{
	return m_projectName->text();
}

QSize KTNewProject::dimension() const
{
	QSize size( (int)m_size->x(), (int)m_size->y() );
	return size;
}

int  KTNewProject::fps() const
{
	return m_fps->value();
}

#if 0
#include <QLabel>
#include <QLineEdit>
#include <QLayout>

KTNewProject::KTNewProject(QWidget *parent)
 : DWizard(parent)
{
	setWindowTitle(tr("Create a new project"));
	setModal(true);
	m_firstPage = new NPFirstPage;
	connect( m_firstPage, SIGNAL(sendToOSD( const QString&,int )), this, SIGNAL(sendToOSD( const QString&, int )));
	addPage(m_firstPage);
}



QString KTNewProject::projectName() const
{
	return m_firstPage->projectName();
}

QSize KTNewProject::dimension() const
{
	return m_firstPage->dimension();
}

int KTNewProject::fps() const
{
	return m_firstPage->fps();
}

QString KTNewProject::renderType() const
{
	return m_firstPage->renderType();
}

// NPFirstPage

NPFirstPage::NPFirstPage(QWidget *parent) : DWizardPage(tr("New KToon Project"), parent)
{
	QFrame *container = new QFrame();
	QGridLayout *layout = new QGridLayout(container);
	
	QLabel *labelProjectName = new QLabel(tr("Project Name"), container);
	layout->addWidget(labelProjectName, 0, 0);
	
	m_projectName = new QLineEdit( container );
	layout->addWidget(m_projectName, 0, 1);
	
	QLabel *labelAuthorName = new QLabel(tr("Author"), container);
	layout->addWidget(labelAuthorName, 1, 0);
	
	m_authorName = new QLineEdit( container );
	layout->addWidget(m_authorName, 1, 1);
	
	m_size = new DXYSpinBox("Dimension", container);
	m_size->setMaximum( 1000);
	m_size->setModifyTogether(true);;
// 	m_size->setX( 520);
// 	m_size->setY( 340);
	
	m_size->setX( 520 );
	m_size->setY( 340);
	
	layout->addWidget(m_size, 2, 0);
	
	QGroupBox *renderAndFps= new QGroupBox();
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	renderAndFps->setLayout(subLayout);
	
	QBoxLayout *renderLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	QLabel *label = new QLabel(tr("Render type"));

	m_renderType = new QComboBox();
	m_renderType->addItems ( QStringList() << tr("Image") << tr("OpenGL") << tr("Native") );
	renderLayout->addWidget(label);
	renderLayout->addWidget(m_renderType);
	subLayout->addLayout(renderLayout);
	
	QBoxLayout *fpsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	label = new QLabel(tr("FPS"));
	m_fps = new QSpinBox();
	m_fps->setValue(24);
	
	fpsLayout->addWidget(label);
	fpsLayout->addWidget(m_fps);
	subLayout->addLayout(fpsLayout);
	
	layout->addWidget(renderAndFps, 2, 1);
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
		emit sendToOSD(tr("Field project name is empty"), 2);
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
	QSize size( (int)m_size->x(), (int)m_size->y() );
	return size;
}

int  NPFirstPage::fps() const
{
	return m_fps->value();
}

QString NPFirstPage::renderType() const
{
	return m_renderType->currentText();
}

#endif
