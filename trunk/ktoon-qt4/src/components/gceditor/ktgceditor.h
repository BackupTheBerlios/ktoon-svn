/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTGCEDITOR_H
#define KTGCEDITOR_H

#include <ktmodulewidgetbase.h>

#include <QLabel>
#include <QPushButton>

#include <deditspinbox.h>
#include "dxyspinbox.h"

#include "dtreelistwidget.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTGCEditor : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTGCEditor(QWidget *parent = 0);
		~KTGCEditor();
		
	public:
		struct SGCItem
		{
			QString name;
			QList<SGCItem> childs;
		};
		
		void addItem(const SGCItem &item);

		
	signals:
		void requestRotate(int angle);
		void requestScale(double x, double y);
		void requestTranslate(double x, double y);
		void requestShear(double x, double y);
		
	private:
		DTreeListWidget *m_componentTree;
		
		DEditSpinBox *m_angle;
		DXYSpinBox *m_scale;
		DXYSpinBox *m_shear;
		DXYSpinBox *m_translate;
};

#endif
