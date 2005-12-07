/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
 
#ifndef KTBRUSHWIDGET_H
#define KTBRUSHWIDGET_H


#include "ktmodulewidgetbase.h"

#include "ktdisplaybrush.h"
#include "ktbrusheditor.h"

#include "kteditspinbox.h"
#include "ktimagebutton.h"
#include "ktbrusheslist.h"

#include <QBoxLayout>
#include <QLineEdit>

#include "kttabwidget.h"
#include "kttoolbox.h"

#include <QGridLayout>

/**
* @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTBrushWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTBrushWidget(QWidget *parent = 0);
		~KTBrushWidget();
		
	private:
		void setupDisplay();
		void setupBrushManager();
		void setupButtons();
		void createDefaultBrushes();
		
	private slots:
		void editBrush();
		
	public slots:
		void changeValueMinThickness(int value);
		void changeValueSmoothness(int value);
		void selectBrush(KTCellViewItem * item);
		
	signals:
		void brushSelected(const QPainterPath &form, int thickness);
		
	private:
		KTBrushEditor *m_brushEditor;
// 		KTTabWidget *m_brushManager;
		KTToolBox *m_brushManager;
		KTEditSpinBox *m_displayThickness, *m_displaySmoothness;
		QLineEdit *m_nameBrush;
		KTImageButton *m_addBrush, *m_removeBrush;
		KTBrushesList *m_defaultBrushesList;
		KTBrushesList *m_customBrushesList;
		
	private:
		QPushButton *m_editFormButton;
		
		QGridLayout *m_layout;
		int m_currentFormIndex;
};

#endif
