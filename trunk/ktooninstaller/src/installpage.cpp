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

#include <unistd.h> // getuid

#include "dimagebutton.h"
#include "ddebug.h"

#include "fileopen.xpm"
#include "install.xpm"

#include "dglobal.h"

#include "kseparator.h"

InstallPage::InstallPage(QWidget *parent)
	: DWizardPage(tr("Install"), parent), m_isComplete(false)
{
	QWidget *container = new QWidget;
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	
	layout->addWidget(new QLabel(tr("Choose KToon destination path...")));
	
	QHBoxLayout *destLayout = new QHBoxLayout;
	
	m_destinationPath = new QLineEdit();
// 	m_destinationPath->setReadOnly(true);
	
	destLayout->addWidget(m_destinationPath);
	
	DImageButton *explorer = new DImageButton(QIcon(fileopen), 16, 0, true);
	connect(explorer, SIGNAL(clicked()), this, SLOT(chooseDestination()));
	destLayout->addWidget(explorer);
	
	layout->addLayout(destLayout);
	
	layout->addWidget(new KSeparator());
	m_status = new QProgressBar(container);
	layout->addWidget(m_status);
	
	
	QPushButton *installButton = new QPushButton(tr("Install it!"));
	connect(installButton, SIGNAL(clicked()), this, SLOT(install()));
	
	layout->addWidget(installButton);
	
	container->setLayout(layout);
	
	layout->addStretch(4);
	
	setPixmap(QPixmap(install_xpm));
	setWidget( container);
	
	layout->addWidget(new KSeparator);
	
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
	fileDialog.setFileMode(QFileDialog::Directory );
	
	fileDialog.setLabelText ( QFileDialog::Accept, tr("Choose") );
	
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
	QDir toCopy( HOME );
	
	if ( !HOME.isEmpty() && !m_destinationPath->text().isEmpty() )
	{
		installDir(toCopy);
		m_isComplete = true;
		
		generateLauncher();
		
		if ( getuid() == 0 )
		{
			generateMenuEntry("/usr/share/applications/Graphics");
		}
		else 
		{
			generateMenuEntry(QDir::homePath()+"/.local/share/applications/Graphics/");
		}
		
		emit completed();
	}
	else
	{
		QMessageBox::critical(0, tr("Error"), tr("Error while trying to install"), QMessageBox::Ok, 0);
	}
}

void InstallPage::installDir(const QDir &toCopy)
{
	QFileInfoList files = toCopy.entryInfoList ();
	
	QDir destination = (m_destinationPath->text() +"/"+ toCopy.path().remove(HOME));
	
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
		
		ts << "export KTOON_HOME=" << m_destinationPath->text() << endl;
		ts << "export LD_LIBRARY_PATH=$KTOON_HOME/lib" << endl;
		ts << "exec $KTOON_HOME/bin/ktoon $*" << endl;
		
		launcher.close();
		
		launcher.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser | QFile::ExeUser | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther );
	}
}

/**
 * [Desktop Entry]
 * Encoding=UTF-8
 * Name=KToon: A 2D Animation toolkit
 * Name[es]=KToon: Herramienta para animacion 2D
 * Exec=ktoon
 * Icon=ktoon.png
 * Type=Application
 * #MimeType=
 * #DocPath=
 * Categories=Application;Graphics;2DGraphics;RasterGraphics;
 * Comment=A 2D animation toolkit
 * Comment[es]=Herramienta para animacion 2D
 * Terminal=false
 */

void InstallPage::generateMenuEntry(const QString &path)
{
	QDir desktopDir(path);
	
	if ( ! desktopDir.exists() )
	{
		desktopDir.mkpath(desktopDir.path());
	}
	
	QFile desktop(desktopDir.path()+"/ktoon.desktop");
	
	if ( desktop.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ts(&desktop);
		// Write header
		ts << "[Desktop Entry]" << endl;
		
		ts << "Encoding=UTF-8" << endl;
		ts << "Name=KToon: A 2D Animation toolkit" << endl;
		ts << "Name[es]=KToon: Herramienta para animacion 2D" << endl;
		
		ts << "Exec="<< m_destinationPath->text()+"/ktoon" << endl;
		ts << "Icon=ktoon.png" << endl;
		ts << "Type=Application" << endl;
		ts << "Categories=Application;Graphics;2DGraphics;RasterGraphics;" << endl;
		ts << "Comment=A 2D animation toolkit" << endl;
		
		ts << "Comment[es]=Herramienta para animacion 2D" << endl;
		ts << "Terminal=false" << endl;
		
		desktop.close();
	}
	
	QFile configure(desktopDir.path()+"/ktoon_configure.desktop");
	
	if ( configure.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ts(&configure);
		// Write header
		ts << "[Desktop Entry]" << endl;
		
		ts << "Encoding=UTF-8" << endl;
		ts << "Name=KToon: Reconfigure" << endl;
		ts << "Name[es]=KToon: Reconfigurar" << endl;
		
		ts << "Exec="<< m_destinationPath->text()+"/ktoon --reconfigure" << endl;
		ts << "Icon=ktoon.png" << endl;
		ts << "Type=Application" << endl;
		ts << "Categories=Application;Graphics;2DGraphics;RasterGraphics;" << endl;
		ts << "Comment=Reconfigure KToon" << endl;
		ts << "Comment[es]=Reconfigura KToon" << endl;
		ts << "Terminal=false" << endl;
		
		configure.close();
	}
	
}

void InstallPage::launchApplication()
{
	dDebug() << "Launching application!";
	
	if ( QFile::exists( QDir::homePath()+"/.ktoon") )
	{
		QProcess::startDetached(m_destinationPath->text()+"/ktoon", QStringList() << "--reconfigure");
	}
	else
	{
		QProcess::startDetached(m_destinationPath->text()+"/ktoon");
	}
}

void InstallPage::aboutToFinish()
{
	D_FUNCINFO;
	if (m_launchApp->isChecked() )
	{
		launchApplication();
	}
}


