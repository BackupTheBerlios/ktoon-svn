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

#ifndef KDATEWIDGET_H
#define KDATEWIDGET_H

#include <qwidget.h>
#include <QDate>
#include "kcore/kglobal.h"

class QComboBox;
class QSpinBox;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KDateWidget : public QWidget
{
	Q_OBJECT;
	public:
		KDateWidget(QWidget *parent = 0);
		KDateWidget(const QDate &date, QWidget *parent = 0);
		~KDateWidget();
		
		void setDate(const QDate &date);
		QDate date() const;
		
	private:
		void init(const QDate &date);
		
	private slots:
		void updateDateValues(int);
		
	private:
		QDate m_date;
		
		QSpinBox *m_day, *m_year;
		QComboBox *m_mounth;
};

#endif
