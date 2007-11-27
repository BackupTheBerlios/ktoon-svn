/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KCONFIGURATIONDIALOG_H
#define KCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QButtonGroup>

#include "kgui/kwidgetlistview.h"
#include "kgui/kflatbutton.h"

#include "kcore/kglobal.h"

class QStackedWidget;
class QListWidget;
class QListWidgetItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KConfigurationDialog : public QDialog
{
	Q_OBJECT
	public:
		KConfigurationDialog(QWidget *parent = 0);
		~KConfigurationDialog();
		
		
		void addPage(QWidget *page, const QString &label, const QIcon &icon);
		QWidget *currentPage() const;
		
	public slots:
		virtual void ok();
		virtual void cancel();
		virtual void apply();
		
	private slots:
		void changePage(QListWidgetItem *, QListWidgetItem*);
		
	private:
		struct Private;
		Private *const k;
};

#endif
