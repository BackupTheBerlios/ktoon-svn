/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#ifndef KTTABDIALOG_H
#define KTTABDIALOG_H

#include <QDialog>
#include <QTabWidget>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTTabDialog : public QDialog
{
	Q_OBJECT
	public:
		enum Button
		{
			Help    = 0x00000001,
			Ok      = 0x00000004,
			Apply   = 0x00000008,
			Cancel  = 0x00000020,
			Custom1 = 0x00000080,
			Custom2 = 0x00000100,
			Custom3 = 0x00000200
		};
		KTTabDialog(QWidget *parent = 0, bool modal = true);
		KTTabDialog(int buttons = Ok|Cancel, QWidget *parent = 0, bool modal = true);
		
		~KTTabDialog();
		
		void addTab ( QWidget * child, const QString & label );
		void addTab ( QWidget * child, const QIcon & iconset, const QString & label );
		
		QWidget *currentTab();
		
	private:
		void setupButtons(int buttons);
		
	public slots:
		virtual void ok();
		virtual void cancel();
		virtual void apply();
		virtual void help(){};
		virtual void custom1() {};
		virtual void custom2() {};
		virtual void custom3() {};
		
	private:
		QTabWidget *m_tabWidget;
};

#endif
