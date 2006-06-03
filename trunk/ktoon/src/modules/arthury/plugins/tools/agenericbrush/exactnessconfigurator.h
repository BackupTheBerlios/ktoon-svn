/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#ifndef EXACTNESSCONFIGURATOR_H
#define EXACTNESSCONFIGURATOR_H

#include <QDoubleSpinBox>
#include <QLabel>

class QTableWidget;
class QTableWidgetItem;

/**
 * @author Jorge Cuadrado <krawek@toonka.com>
*/
class ExactnessConfigurator : public QWidget
{
	Q_OBJECT;
	public:
		ExactnessConfigurator(QWidget *parent = 0);
		~ExactnessConfigurator();
		double exactness() const;
		
	protected:
		void resizeEvent(QResizeEvent *e);
		
	private slots:
		void addCurrentValue();
		void removeCurrentValue();
		
		void updateValueFromItem(QTableWidgetItem *item);
		
	private:
		QDoubleSpinBox *m_exactness;
		QTableWidget *m_table;

};

#endif
