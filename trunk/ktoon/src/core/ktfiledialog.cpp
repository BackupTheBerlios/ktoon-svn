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

#include <qhbox.h>
#include <qfileinfo.h>
#include <qdir.h>

KTFileDialog::KTFileDialog(QWidget *parent) : QDialog(parent, "KTFileDialog")
{
	KTINIT;
	setCaption("Choose your file...");
	setModal(true);
	
	(new QVBoxLayout(this))->setAutoAdd(true);
	
	QHBox *title = new QHBox(this);
	new QLabel("<br>"+tr("Repository ")+KTOON_REPOSITORY+"<br>", this);
	
	new KSeparator(this);
	
	new QLabel(tr("File name: "), this);
	m_fileNameLE = new QLineEdit(this);
	
	m_listView = new QListView(this);
	m_listView->addColumn(tr("Filename"));
	m_listView->addColumn(tr("Owner"));
	m_listView->addColumn(tr("Date"));
	
	connect(m_listView, SIGNAL(clicked(QListViewItem *)), this, SLOT(select(QListViewItem *)));
	
	QHBox *buttonBox= new QHBox(this);
	m_accept = new QPushButton(tr("Accept"), buttonBox);
	connect(m_accept, SIGNAL(clicked()), this, SLOT(accept()));
	m_cancel = new QPushButton(tr("Cancel"), buttonBox);
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	readFiles();
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
		
		if( ! m_fileName.endsWith(".ktn") )
		{
			m_fileName+=".ktn";
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
	QDir m_dir(KTOON_REPOSITORY);
	
	QFileInfo *iterator;
	QFileInfoList *files = new QFileInfoList(*m_dir.entryInfoList("*.ktn"));
	
	for(iterator = files->first(); iterator; iterator = files->next() )
	{
		QListViewItem *item = new QListViewItem(m_listView);
		item->setText(0, iterator->fileName());
		item->setText(1, iterator->owner());
		item->setText(2, iterator->created().toString());
	}
}

void KTFileDialog::select(QListViewItem *item)
{
	if ( ! item )
	{
		return;
	}
	
	m_fileNameLE->setText(item->text(0));
}
