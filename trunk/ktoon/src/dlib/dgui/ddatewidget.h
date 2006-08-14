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

#ifndef DDATEWIDGET_H
#define DDATEWIDGET_H

#include <qwidget.h>
#include <QDate>
#include "dcore/dglobal.h"

class QComboBox;
class QSpinBox;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DDateWidget : public QWidget
{
	Q_OBJECT;
	public:
		DDateWidget(QWidget *parent = 0);
		DDateWidget(const QDate &date, QWidget *parent = 0);
		~DDateWidget();
		
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
