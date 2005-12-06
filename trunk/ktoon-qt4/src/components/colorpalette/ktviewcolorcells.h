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

// #include "ktcellview.h"
#include "ktcellscolor.h"
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
		KTCellsColor *m_defaultPalette;
		KTCellsColor *m_qtColorPalette;
		KTCellsColor *m_customColorPalette;
		KTCellsColor *m_customGradientPalette;
		int m_numColorRecent;
		const int MAX_COLUMS;
		
	private:
		void setupForm();
		void fillDefaultColors();
		void addDefaultColor(int i , int j, const QColor &);
		void fillQtColor();
	protected:
		
		
	signals:
		void selectColor(const QColor &);
		void selectGradient(const QGradient &);
		
	public slots:
		virtual void addCustomColor(const QBrush& c);
		virtual void removeCurrentColor();
		virtual void addPalette(const QString & name);
		void changeColor(KTCellViewItem*);
		
};

#endif
