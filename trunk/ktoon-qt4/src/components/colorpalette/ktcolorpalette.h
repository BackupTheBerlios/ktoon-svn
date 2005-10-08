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
#include <QStackedWidget>

#include <QGroupBox>

#include "ktvhbox.h"
#include "ktcolorpicker.h"
#include "ktluminancepicker.h"

//FIXME: unificar estas clases
#include "fillcolor.h"
#include "outlinecolor.h"

//FIXME: portar estas Clases
#include "valueselector.h"
#include "colorgradientselector.h"
#include "gradientviewer.h"
// #include <QColor>
/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTColorPalette : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTColorPalette(QWidget *parent = 0);
		~KTColorPalette();
		
		
	private:
		KTViewColorCells *m_containerPalette;
		QGroupBox *containerButtons;
		
		KTValueColor *m_displayValueColor;
		KTColorPicker *m_colorPicker;
		KTLuminancePicker *m_luminancePicker;
// 		QColor m_currentLineColor, m_currentFillColor;
// 		ColorMixer *m_colorPicker;
		GradientViewer *m_gradientViewer;
		ColorGradientSelector *m_gradient;
		QComboBox *m_gradientTypes;
		QLineEdit *m_nameColor;
		FillColor *m_outlineColor, *m_fillColor;
// 		ValueSelector *m_valueSelector;
		QColor m_currentOutlineColor, m_currentFillColor;
		
	private:
		void setupButtons();
		void setupChooserTypeColor();
		void setupChooserGradient();
		
	public slots:
		void setColor(const QColor &color);
		void updateColor();
		void changeTypeColor();
		void syncHsv(int h , int s , int v);
		void setHS(int h, int s);
		
	signals:
		void colorChanged(const QColor &color);
};

#endif
