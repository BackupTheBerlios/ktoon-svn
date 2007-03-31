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

#include "ktcolorpicker.h"
#include "ktluminancepicker.h"
#include "ktgradientcreator.h"

#include <QComboBox>
#include <QGroupBox>
#include <QSplitter>
#include <QMenu>

#include <dgui/dtoolbox.h>
#include <dgui/dvhbox.h>
#include <dgui/ddualcolorbutton.h>


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
		struct Private;
		Private *const d;
		
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
		void changeTypeColor(DDualColorButton::DualColor s);
		void syncHsv(int h , int s , int v);
		void setHS(int h, int s);
		void changeBrushType(const QString& );
		
	signals:
		void paintAreaEventTriggered(const KTPaintAreaEvent *event);
};

#endif
