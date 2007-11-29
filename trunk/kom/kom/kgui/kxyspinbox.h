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

#ifndef KXYSPINBOX_H
#define KXYSPINBOX_H

#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_GUI_EXPORT KXYSpinBox : public QGroupBox
{
	Q_OBJECT
	public:
		KXYSpinBox(const QString &title, QWidget *parent = 0);
		~KXYSpinBox();
		void setSingleStep(double step);
		void setMinimum ( double min);
		void setMaximum ( double max);
		void setX(double x);
		void setY(double y);
		double x();
		double y();
		void setModifyTogether(bool enable);
		
	private slots:
		void updateXValue(double v);
		void updateYValue(double v);
		void toggleModify();
		
	private:
		QLabel *m_textX, *m_textY;
	
		QDoubleSpinBox *m_x, *m_y;
		QPushButton *m_separator;
		
		bool m_modifyTogether;
		
	signals:
		void valueXChanged(double );
		void valueYChanged(double );
		void valueXYChanged(double, double);
};

#endif
