/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kthelpwidget.h"
#include <qlayout.h>
#include <qtabwidget.h>
#include <qdom.h>
#include <qfile.h>
#include <qmap.h>

#include <kcore/kdebug.h>

#include <QLocale>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

#include <kcore/kglobal.h>

KTHelpWidget::KTHelpWidget(const QString &path, QWidget *parent) : KTModuleWidgetBase(parent)
{
	setWindowTitle( tr("Help"));
	setWindowIcon(QPixmap(THEME_DIR+"/icons/help.png"));
	
	if (QString(QLocale::system().name()).length() > 1 )
	{
		m_helpPath = path+"/"+QString(QLocale::system().name()).left(2);
	}
	else
	{
		m_helpPath = path+"/en";
	}
	
	
	QTreeWidget *contentsListView = new QTreeWidget(this);
	contentsListView->setHeaderLabels ( QStringList() << tr("") );
	contentsListView->header()->hide();
	
	connect(contentsListView, SIGNAL(itemClicked ( QTreeWidgetItem *, int )), this, SLOT(tryToLoadPage(QTreeWidgetItem *, int)));
	
// 	contentsListView->setRootIsDecorated(true);
// 	contentsListView->addColumn(tr("Topics"));
	
	addChild( contentsListView);
	
	QDomDocument document;
	QFile file( m_helpPath.path()+"/help.xml" );
	
// 	dDebug() << "Help path: " << m_helpPath.path();
	if ( file.open( QIODevice::ReadOnly ) )
	{
		if ( document.setContent(&file) )
		{
			QDomElement root = document.documentElement();
			
			QDomNode section = root.firstChild();
			while( !section.isNull() )
			{
				QDomElement element = section.toElement();
				if( !element.isNull() )
				{
					if ( element.tagName() == "Section" )
					{
						QTreeWidgetItem *item = new QTreeWidgetItem(contentsListView);
						item->setText(0, element.attribute("title"));
						
						QDomNode subSection = element.firstChild();
						while( ! subSection.isNull())
						{
							QDomElement element2 = subSection.toElement();
							if ( !element2.isNull() )
							{
								if ( element2.tagName() == "SubSection" )
								{
									QTreeWidgetItem *subitem = new QTreeWidgetItem(item);
									subitem->setText(0, element2.attribute("title"));
									
									m_files.insert(subitem, element2.attribute("file"));
								}
							}
							subSection = subSection.nextSibling();
						}
					}
				}
				section = section.nextSibling();
			}
		}
		else
		{
			qDebug("KTHelpWidget::Can't set contents");
		}
		file.close();
	}
	else
	{
		qDebug("KTHelpWidget::Can't open");
	}
	
	contentsListView->show();
}

KTHelpWidget::~KTHelpWidget()
{
}

void KTHelpWidget::tryToLoadPage(QTreeWidgetItem *item, int)
{
	if ( item )
	{
		QString fileName = m_files[item];
		if ( ! fileName.isNull() )
		{
			loadPage(item->text(0), m_helpPath.path()+"/"+ fileName);
		}
	}
}

void KTHelpWidget::loadPage(const QString &title, const QString &filePath)
{
// 	dDebug() << "Loading: "+filePath;
/*	
	QFile file(filePath);
	QFileInfo finfo(file);
	
	if ( finfo.exists() && !finfo.isDir() && file.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		QTextStream ts(&file);
		
		QString document = "";
		while( ! ts.atEnd() )
		{
			document += ts.readLine();
		}
		
		file.close();
		
		if ( document != "" )*/
	emit pageLoaded(title, filePath);
// 	}
	
}

QString KTHelpWidget::helpPath () const
{
	return m_helpPath.path();
}
