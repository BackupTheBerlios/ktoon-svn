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

#include "ktfiledialog.h"
#include "ktapplication.h"
#include "ktdebug.h"
#include "kseparator.h"

#include <QFileInfo>
#include <QDir>

#include <QLabel>
#include <QVBoxLayout>
#include <QDateTime>
#include <QHeaderView>

#include "ktvhbox.h"

KTFileDialog::KTFileDialog(Type t, QWidget *parent) : QDialog(parent), m_type(t)
{
	KTINIT;
	setCaption("Choose your file...");
	setModal(true);
	
	QVBoxLayout *m_mainLayout = new QVBoxLayout;
	
	KTVHBox *title = new KTVHBox(this, Qt::Horizontal);
	title->show();
	m_mainLayout->addWidget(title);

	if ( t == Repository )
	{
		QLabel *explain = new QLabel("<br>"+tr("Repository ")+KTOON_REPOSITORY+"<br>", this);
		explain->show();
		m_mainLayout->addWidget(explain);
	}
	else if ( t == Themes )
	{
		QLabel *explain = new QLabel("<br>"+tr("Theme dir ")+ktapp->configDir()+"/themes/<br>", this);
		explain->show();
		m_mainLayout->addWidget(explain);
	}
	
	KSeparator *sep1 = new KSeparator(this);
	m_mainLayout->addWidget(sep1);
	
	m_mainLayout->addWidget(new QLabel(tr("File name: "), this));
	m_fileNameLE = new QLineEdit(this);
	m_fileNameLE->show();
	m_mainLayout->addWidget(m_fileNameLE);
	
	m_treeWidget = new QTreeWidget(this);
	m_treeWidget->show();
	
	m_mainLayout->addWidget(m_treeWidget);
	m_treeWidget->setHeaderLabels ( QStringList() << tr("Filename") << tr( "Owner" ) << tr( "Date" ) );
	m_treeWidget->header()->setResizeMode(QHeaderView::Stretch);
	
	connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(select(QTreeWidgetItem *, int)));
	
	KTVHBox *buttonBox= new KTVHBox(this, Qt::Horizontal);
	buttonBox->show();
	m_mainLayout->addWidget(buttonBox);
	m_accept = new QPushButton(tr("Accept"), buttonBox);
	connect(m_accept, SIGNAL(clicked()), this, SLOT(accept()));
	m_cancel = new QPushButton(tr("Cancel"), buttonBox);
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	readFiles();
	
	setLayout(m_mainLayout);
	
	show();
}


KTFileDialog::~KTFileDialog()
{
	KTEND;
}

void KTFileDialog::accept()
{
	if ( ! m_fileNameLE->text().isEmpty() )
	{
		m_fileName = m_fileNameLE->text();
		
		if ( m_type == Repository )
		{
			if( ! m_fileName.endsWith(".ktn") )
			{
				m_fileName+=".ktn";
			}
		}
		else if ( m_type == Themes )
		{
			if( ! m_fileName.endsWith(".ktt") )
			{
				m_fileName+=".ktt";
			}
		}
		
		QDialog::accept();
	}
	else
	{
	}
}

QString KTFileDialog::fileName() const
{
	return m_fileName;
}

void KTFileDialog::readFiles()
{
	QDir m_dir;
	QString extension = "";
	
	if ( m_type == Repository )
	{
		m_dir.setPath(KTOON_REPOSITORY);
		extension = "ktn";
	}
	else if (m_type == Themes )
	{
		m_dir.setPath(ktapp->configDir()+"/themes/");
		extension = "ktt";
	}
	
	
	if ( m_dir.exists() )
	{
		QFileInfoList projects = m_dir.entryInfoList();
		
		foreach(QFileInfo dir, projects)
		{
			if ( dir.isDir() && ! (dir.fileName()[0] == '.') )
			{
				QDir projectDir(dir.filePath());
				QFileInfoList files = projectDir.entryInfoList(QStringList() << QString("*.")+extension);
		
				foreach(QFileInfo iterator, files)
				{
					QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
					item->setText(0, iterator.fileName());
					item->setText(1, iterator.owner());
					item->setText(2, iterator.created().toString());
				}
			}
		}
	}
}

void KTFileDialog::select(QTreeWidgetItem *item, int)
{
	if ( ! item )
	{
		return;
	}
	
	m_fileNameLE->setText(item->text(0));
}
