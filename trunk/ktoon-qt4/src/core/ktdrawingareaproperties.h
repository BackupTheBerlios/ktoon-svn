/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef KTDRAWINGAREAPROPERTIES_H
#define KTDRAWINGAREAPROPERTIES_H

#include <QDialog>


class DColorButton;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTDrawingAreaProperties : public QDialog
{
	Q_OBJECT;
	public:
		KTDrawingAreaProperties(QWidget *parent = 0);
		~KTDrawingAreaProperties();
		
		QColor gridColor() const;
		QColor backgroundColor() const;
		QColor onionSkinColor() const;
		QColor onionSkinBackground() const;
		
	private:
		void setupPage();
		
	private:
		DColorButton *m_gridColor,*m_backgroundColor,*m_onionSkinColor,*m_onionSkinBackground;
};

#endif
