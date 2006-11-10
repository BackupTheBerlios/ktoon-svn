/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTHELPWIDGET_H
#define KTHELPWIDGET_H

#include <QWidget>
#include <QMap>

#include <QTreeWidget>
#include <qdir.h>

#include "ktmodulewidgetbase.h"

class DHelpWidgetManager;

/**
 * @author David Alejandro Cuadrado Cabrera <krawek@gmail.com>
*/
class KTHelpWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTHelpWidget(const QString &path, QWidget *parent = 0);
		~KTHelpWidget();
		QString helpPath () const;
		
	private slots:
		void tryToLoadPage(QTreeWidgetItem *, int);
		void loadPage(const QString &title,const QString &path);
		
	signals:
		void pageLoaded(const QString &title, const QString &content);
		
	private:
		QDir m_helpPath;
		QMap<QTreeWidgetItem *, QString> m_files;
};

#endif
