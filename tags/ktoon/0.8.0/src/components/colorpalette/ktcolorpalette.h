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
#include <QSplitter>

#include "dtoolbox.h"
#include "dvhbox.h"
#include "ktcolorpicker.h"
#include "ktluminancepicker.h"
#include "ktdualcolorbutton.h"
#include "ktgradientcreator.h"
#include <QMenu>


class KTColorPalette;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTColorPalette : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		enum TypeBrush{ Solid = 0, Gradient };
		KTColorPalette(QWidget *parent = 0);
		~KTColorPalette();
		//FIXME: cambiar esto por brush
		QPair<QColor, QColor> color();
		void parsePaletteFile(const QString &file);

	private:
		DToolBox *m_centralWidget;
		
		KTViewColorCells *m_containerPalette;
		
		KTValueColor *m_displayValueColor;
		KTColorPicker *m_colorPicker;
		KTLuminancePicker *m_luminancePicker;
		KTGradientCreator *m_gradientManager;
		QLineEdit *m_nameColor;
		KTDualColorButton *m_outlineAndFillColors;
		QBrush m_currentOutlineColor, m_currentFillColor;
		QComboBox *m_labelType;
		QSplitter *m_splitter;
		bool m_flagGradient;
		TypeBrush m_type;
		
	private:
		void setupButtons();
		void setupChooserTypeColor();
		void setupGradienManager();
		void setupDisplayColor();
	
	protected:
		void mousePressEvent ( QMouseEvent * e );
	
	public slots:
		void setColor(const QBrush &brush);
		void setFG(const QBrush &brush);
		void setBG(const QBrush &brush);
		void updateColor();
		void changeTypeColor(KTDualColorButton::DualColor s);
		void syncHsv(int h , int s , int v);
		void setHS(int h, int s);
		void changeBrushType(const QString& );
		
	signals:
		void brushChanged(const QBrush &foreground, const QBrush &background); 
};

#endif
