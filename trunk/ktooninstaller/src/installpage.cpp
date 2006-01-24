/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                *
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

#include "installpage.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QProcess>

#include "ktimagebutton.h"
#include "application.h"
#include "ktdebug.h"

#include "fileopen.xpm"
#include "install.xpm"

InstallPage::InstallPage(QWidget *parent)
	: KTWizardPage(tr("Install"), parent), m_isComplete(false)
{
	QWidget *container = new QWidget;
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	
	layout->addWidget(new QLabel(tr("Choose KToon destination path...")));
	
	QHBoxLayout *destLayout = new QHBoxLayout;
	
	m_destinationPath = new QLineEdit();
	m_destinationPath->setReadOnly(true);
	
	destLayout->addWidget(m_destinationPath);
	
	KTImageButton *explorer = new KTImageButton(QIcon(fileopen), 16, 0, true);
	connect(explorer, SIGNAL(clicked()), this, SLOT(chooseDestination()));
	destLayout->addWidget(explorer);
	
	layout->addLayout(destLayout);
	
	m_status = new QProgressBar(container);
	layout->addWidget(m_status);
	
	
	QPushButton *installButton = new QPushButton(tr("Install it!"));
	connect(installButton, SIGNAL(clicked()), this, SLOT(install()));
	
	layout->addWidget(installButton);
	
	container->setLayout(layout);
	
	setPixmap(QPixmap(install_xpm));
	setWidget( container);
	
	m_launchApp = new QCheckBox(tr("Launch application"));
	layout->addWidget(m_launchApp);
	
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


InstallPage::~InstallPage()
{
}

bool InstallPage::isComplete()
{
	return m_isComplete;
}

void InstallPage::reset()
{
}

void InstallPage::chooseDestination()
{
	QFileDialog fileDialog;
	fileDialog.setFileMode(QFileDialog::AnyFile);
	
	if ( fileDialog.exec() != QDialog::Rejected )
	{
		QStringList selected = fileDialog.selectedFiles ();
		if  (selected.count() > 0 )
		{
			QString destination = selected[0];
			
			//QFileDialog::getSaveFileName (this, tr("Choose... ;)"), QDir::homePath(), QString(), new QString(), QFileDialog::ShowDirsOnly );
			
			QFileInfo fileInfo(destination);
			
			if ( QFile::exists(destination) && ! fileInfo.isDir() )
			{
				destination = fileInfo.absolutePath () ;
			}
			
			m_destinationPath->setText(destination);
		}
	}
}

void InstallPage::install()
{	
	QDir toCopy(static_cast<Application *>(qApp)->ktoonHome() );
	
	if ( !static_cast<Application *>(qApp)->ktoonHome().isEmpty() && !m_destinationPath->text().isEmpty() )
	{
		installDir(toCopy);
		m_isComplete = true;
		
		generateLauncher();
		
		emit completed();
	}
	else
	{
		QMessageBox::critical(0, tr("Error"), tr("Error while trying install"), QMessageBox::Ok, 0);
	}
}

void InstallPage::installDir(const QDir &toCopy)
{
	QFileInfoList files = toCopy.entryInfoList ();
	
	QDir destination = (m_destinationPath->text() +"/"+ toCopy.path().remove(static_cast<Application *>(qApp)->ktoonHome()));
	
	if ( ! destination.exists() )
	{
		destination.mkpath( destination.path() );
	}
	
	foreach(QFileInfo fileInfo, files)
	{
		if ( ! fileInfo.isHidden() )
		{
			m_status->setValue( m_status->value()+1 );
			
			if ( fileInfo.isDir() )
			{
				installDir(fileInfo.absoluteFilePath());
			}
			else
			{
				QFile file(fileInfo.absoluteFilePath());
				file.copy(destination.path()+"/"+fileInfo.fileName());
			}
		}
	}
}

void InstallPage::generateLauncher()
{
	QFile launcher(m_destinationPath->text()+"/ktoon");
	
	if ( launcher.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ts(&launcher);
		// Write header
		ts << "#!/bin/bash" << endl;
		
		ts << "KTOON_HOME=" << m_destinationPath->text() << endl;
		ts << "LD_LIBRARY_PATH=$KTOON_HOME/lib" << endl;
		ts << "exec $KTOON_HOME/bin/ktoon" << endl;
		
		launcher.close();
		
		launcher.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser | QFile::ExeUser | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther );
	}
}

void InstallPage::launchApplication()
{
	ktDebug() << "Launching application!";
	QProcess::startDetached(m_destinationPath->text()+"/ktoon");
}

void InstallPage::aboutToFinish()
{
	KT_FUNCINFO;
	if (m_launchApp->isChecked() )
	{
		launchApplication();
	}
}


