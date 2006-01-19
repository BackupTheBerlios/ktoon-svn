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
 
#ifndef KTCONFIGURATIONDIALOG_H
#define KTCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QMap>

class QStackedWidget;
class QTreeWidget;
class QTreeWidgetItem;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTConfigurationDialog : public QDialog
{
	Q_OBJECT
	public:
		KTConfigurationDialog(QWidget *parent = 0);
		~KTConfigurationDialog();
		void addSection(QWidget *info, const QString &title);
		void addSection(const QString &title);
		void addPage(QWidget *page, const QString &title, const QString &section);
		void addPage(QWidget *page, const QString &title, const QIcon &icon, const QString &section);
		QWidget *currentPage();
		
	public slots:
		virtual void ok();
		virtual void cancel();
		virtual void apply();
		
	private slots:
		void showPageForItem(QTreeWidgetItem *, int );
		
	private:
		QTreeWidget *m_list;
		QStackedWidget *m_container;
		QMap<QTreeWidgetItem *, QWidget *> m_pages;
		QMap<QString, QTreeWidgetItem *> m_sections;
		
};

#endif