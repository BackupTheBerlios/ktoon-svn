/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#ifndef DDATEPICKER_H
#define DDATEPICKER_H

#include <QFrame>
#include <QToolButton>

#include "dgui/ddatetable.h"
#include "dcore/dglobal.h"

class QComboBox;
class QLabel;

/**
 * 
 * @author David Cuadrado <krawek@gmail.com>
 * 
*/

class D_GUI_EXPORT DDatePicker : public QFrame
{
	Q_OBJECT;
	public:
		DDatePicker(QWidget *parent = 0);
		~DDatePicker();
		void setDate(const QDate &date);
		QDate date() const;
		
	private:
		void fillWeeks(const QDate &date);
		
	public slots:
		void setWeek(int week);
		void setYear(int year);
		
	protected slots:
		void previousYear();
		void nextYear();
		
		void previousMounth();
		void nextMounth();
		
	private slots:
		void mounthFromAction(QAction *act);
		
	signals:
		void dateChanged(const QDate &date);
		
	private:
		QComboBox *m_week;
		DDateTable *m_dateTable;
		
		class EditableButton;
		
		 QToolButton *m_mounth;
		 EditableButton *m_year;
};

class DDatePicker::EditableButton : public QToolButton
{
	Q_OBJECT
	public:
		EditableButton();
		~EditableButton();
		
	public slots:
		void edit();
		
	private slots:
		void emitYearSelected();
		
	signals:
		void yearSelected(int year);
		
	private:
		QLineEdit *m_editor;
};


#endif
