/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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
#include <QBrush>

#include "ktcellscolor.h"
#include "ktpaletteparser.h"


/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTViewColorCells : public QFrame
{
	Q_OBJECT
	public:
		KTViewColorCells(QWidget *parent = 0);
		virtual ~KTViewColorCells();
		void readPaletteFile(const QString &file);
		void setColor(const QBrush & b);
		
	private:
		struct Private;
		Private *const k;
		
		
	private:
		void setupForm();
		void setupButtons();
		void fillDefaultColors();
		void addDefaultColor(int i , int j, const QColor &);
		void fillNamedColor();
		void readPalettes(const QString &paletteDir);
		void addPalette(KTCellsColor *palette);
		
		
	protected:
		
		
	signals:
		void selectColor(const QBrush &);
		
	public slots:
		virtual void addCurrentColor();
		virtual void removeCurrentColor();
		virtual void addPalette(const QString & name, const QList<QBrush> & brushes, bool editable );
		void changeColor(QTableWidgetItem*/*, QTableWidgetItem **/);
		
};

#endif
