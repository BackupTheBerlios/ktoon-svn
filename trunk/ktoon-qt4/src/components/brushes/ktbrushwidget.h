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
#include "kteditspinbox.h"
#include "ktimagebutton.h"
#include "ktbrusheslist.h"

#include <QBoxLayout>
#include <QTabWidget>
#include <QLineEdit>

#include "ktbrush.h"



/**
*	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTBrushWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTBrushWidget(QWidget *parent = 0);
		~KTBrushWidget();
		//FIXME:
		enum BrushConstants
		{
			THICKNESS_MIN_MIN = 1, /**< Constant that defines the lowest value of the brush minimum thickness */
			THICKNESS_MIN_MAX = 99, /**< Constant that defines the highest value of the brush minimum thickness */
			THICKNESS_MAX_MIN = 1, /**< Constant that defines the lowest value of the brush maximum thickness */
			THICKNESS_MAX_MAX = 99, /**< Constant that defines the highest value of the brush maximum thickness */
			SMOOTHNESS_MIN = 0, /**< Constant that defines the lowest value of the brush smoothness */
			SMOOTHNESS_MAX = 9 /**< Constant that defines the highest value of the brush smoothness */
		};
		
	private:
		KTDisplayBrush *m_displayBrush;
		QTabWidget *m_brushManager;
		KTEditSpinBox *m_displayThickness, *m_displaySmoothness;
		QLineEdit *m_nameBrush;
		KTImageButton *m_addBrush, *m_removeBrush;
		KTBrushesList *m_defaultBrushesList;
		KTBrushesList *m_customBrushesList;
		
	private:
		void setupDisplay();
		void setupBrushManager();
		void setupButtons();
		void createDefaultBrushes();
		
		
		
	public slots:
		void changeValueMinThickness(int value);
		void changeValueSmoothness(int value);
		void selectBrush(QListWidgetItem * item);
		
	signals:
		void brushSelected(KTBrush *);
		/**
		* This signal is emitted when the minimum thickness has changed.
		*/
// 		void minThicknessChanged(int index , int value);
		/**
		 * This signal is emitted when the maximum thickness has changed.
		*/
// 		void maxThicknessChanged(int index , int value);
		/**
		 * This signal is emitted when the smoothness has changed.
		*/
// 		void smoothnessChanged(int index , int value);
};

#endif
