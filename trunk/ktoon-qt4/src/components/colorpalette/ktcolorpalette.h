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

#include "ktvhbox.h"
#include "ktcolorpicker.h"
#include "ktluminancepicker.h"
#include "ktdualcolorbutton.h"

//FIXME: unificar estas clases
#include "fillcolor.h"
#include "outlinecolor.h"

//FIXME: portar estas Clases
// #include "valueselector.h"
// #include "colorgradientselector.h"
#include "ktgradientselector.h"
#include "gradientviewer.h"

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
		QGroupBox *containerButtons;
		
		KTValueColor *m_displayValueColor;
		KTColorPicker *m_colorPicker;
		KTLuminancePicker *m_luminancePicker;
// 		QColor m_currentLineColor, m_currentFillColor;
// 		ColorMixer *m_colorPicker;
		GradientViewer *m_gradientViewer;
		KTGradientSelector *m_gradient;
		QComboBox *m_gradientTypes;
		QLineEdit *m_nameColor;
// 		FillColor *m_outlineColor, *m_fillColor;
		KTDualColorButton *m_outlineAndFillColors;
// 		ValueSelector *m_valueSelector;
		QColor m_currentOutlineColor, m_currentFillColor;
		
		QIcon m_icon;
		int m_lastIndex;
		
	private:
		void setupButtons();
		void setupChooserTypeColor();
		void setupChooserGradient();
		
	public slots:
		void setColor(const QColor &color);
		void updateColor();
		void changeTypeColor(KTDualColorButton::DualColor s);
		void syncHsv(int h , int s , int v);
		void setHS(int h, int s);
		
	signals:
		void colorChanged(const QColor &foreground, const QColor &background);
};

#endif
