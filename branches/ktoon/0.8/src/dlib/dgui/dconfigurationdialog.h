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
 
#ifndef DCONFIGURATIONDIALOG_H
#define DCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QButtonGroup>

#include "dwidgetlistview.h"
#include "dflatbutton.h"

class QStackedWidget;
class QTreeWidget;
class QTableWidgetItem;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class DConfigurationDialog : public QDialog
{
	Q_OBJECT
	public:
		DConfigurationDialog(QWidget *parent = 0);
		~DConfigurationDialog();
		void addSection(QWidget *info, const QString &title);
		void addSection(const QString &title);
		void addPageToSection(QWidget *page, const QString &title, const QString &section);
		void addPageToSection(QWidget *page, const QString &title, const QIcon &icon, const QString &section);
		
		void addPage(QWidget *page, const QString &title, const QIcon &icon);
		
		QWidget *currentPage();
		
	public slots:
		virtual void ok();
		virtual void cancel();
		virtual void apply();
		
	private slots:
		void showPageForItem(QTableWidgetItem *);
		void showPageForButton(QAbstractButton *);
		
	private:
		DWidgetListView *m_list;
		QStackedWidget *m_container;
		QMap<QTableWidgetItem *, QWidget *> m_pages;
		QMap<QString, QTableWidgetItem *> m_sections;
		
		QButtonGroup *m_buttonGroup;
};

#endif
