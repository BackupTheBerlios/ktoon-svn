/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef DTABDIALOG_H
#define DTABDIALOG_H

#include <QDialog>
#include "dgui/dtabwidget.h"

#include <QHash>

typedef QHash<int, QPushButton *> Buttons;


/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DTabDialog : public QDialog
{
	Q_OBJECT
	public:
		enum Button
		{
			Help    = 1<<2,
			Ok      = 1<<3,
			Apply   = 1<<4,
			Cancel  = 1<<5,
			Custom1 = 1<<6,
			Custom2 = 1<<7,
			Custom3 = 1<<8
		};
		DTabDialog(QWidget *parent = 0, bool modal = true);
		DTabDialog(int buttons = Ok|Cancel, QWidget *parent = 0, bool modal = true);
		
		~DTabDialog();
		
		void addTab ( QWidget * child, const QString & label );
		void addTab ( QWidget * child, const QIcon & iconset, const QString & label );
		
		QWidget *currentTab();
		
		void setButtonText(Button b, const QString &text);
		QPushButton *button(Button b);
		
		DTabWidget *tabWidget() const;
		
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
		DTabWidget *m_tabWidget;
		Buttons m_buttons;
};

#endif
