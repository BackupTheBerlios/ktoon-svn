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

#ifndef KTCOLORPALETTE_H
#define KTCOLORPALETTE_H

#include "ktmodulewidgetbase.h"

#include "ktvaluecolor.h"
#include "ktviewcolorcells.h"

#include <QComboBox>

#include <QGroupBox>
#include <QToolBox>
#include <QSplitter>

#include "ktvhbox.h"
#include "ktcolorpicker.h"
#include "ktluminancepicker.h"
#include "ktdualcolorbutton.h"
#include "ktgradientmanager.h"

class KTColorPalette;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
					  
class KTColorPalette : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTColorPalette(QWidget *parent = 0);
		~KTColorPalette();
		QPair<QColor, QColor> color();
		
	private:
		void createIcon();
		
	private slots:
		void changeIcon(int);
		
	private:
		QToolBox *m_centralWidget;
		
		KTViewColorCells *m_containerPalette;
		
		KTValueColor *m_displayValueColor;
		KTColorPicker *m_colorPicker;
		KTLuminancePicker *m_luminancePicker;
		KTGradientManager *m_gradientManager;
		QLineEdit *m_nameColor;
		KTDualColorButton *m_outlineAndFillColors;
		QColor m_currentOutlineColor, m_currentFillColor;
		
		QSplitter *m_splitter;
		
		QIcon m_icon;
		int m_lastIndex;
		bool m_flagGradient;
		
		
	private:
		void setupButtons();
		void setupChooserTypeColor();
		void setupGradienManager();
		void setupDisplayColor();
				
	public slots:
		void setColor(const QColor &color);
		void updateColor();
		void changeTypeColor(KTDualColorButton::DualColor s);
		void syncHsv(int h , int s , int v);
		void setHS(int h, int s);
		void changeGradient(const QGradient & gradient);
		
		void addColor();
		void removeColor();
		
	signals:
		void colorChanged(const QBrush &foreground, const QBrush &background); // TODO: cambiar nombre
};

#endif
