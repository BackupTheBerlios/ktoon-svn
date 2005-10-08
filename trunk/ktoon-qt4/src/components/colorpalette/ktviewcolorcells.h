/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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
#ifndef KTVIEWCOLORCELLS_H
#define KTVIEWCOLORCELLS_H

#include <QFrame>
#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>

//reimplemtentar estas clases para porta a qt4 
#include "colorcells.h"
#include "custompalette.h"

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTViewColorCells : public QFrame
{
	Q_OBJECT
	public:
		KTViewColorCells(QWidget *parent = 0);
		virtual ~KTViewColorCells();
		
	private:
		QComboBox *m_chooserPalette;
		QStackedWidget *m_containerPalette;
		ColorCells *m_defaultPalette;
		
		
	private:
		void setupForm();
		
	public slots:
		virtual void addCustomColor(QColor c);
		virtual void addCustomPalette(QString name);
};

#endif