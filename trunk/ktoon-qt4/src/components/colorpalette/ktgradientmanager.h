/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 


#ifndef KTGRADIENTMANAGER_H
#define KTGRADIENTMANAGER_H

#include "ktgradientselector.h"
#include "ktgradientviewer.h"
#include "kteditspinbox.h"

#include <QFrame>
#include <QComboBox>

#include "ktxyspinbox.h"

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTGradientManager : public QFrame
{
	Q_OBJECT
	public:
		KTGradientManager(QWidget *parent = 0);
		~KTGradientManager();
		void setColor(const QColor &);
		int gradientType();
		void updateGradient();
	private:
		KTGradientSelector *m_selector;
		KTGradientViewer *m_viewer;
		QComboBox *m_type ;
		
// 		KTXYSpinBox *m_center, *m_focal;
		
	public slots:
		void changeType(int type);
		void changeGradient( const QGradientStops& );
		
	signals:
		void gradientChanged(const QGradient &);
		
};

#endif
