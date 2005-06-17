/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#ifndef BRUSHES_H
#define BRUSHES_H

/**
 * @file brushes.h
 * @brief Include this file if you need the class Brushes
 */

#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qlistview.h>


#include <qlineedit.h>
#include <qslider.h>
#include <qlabel.h>


#include <qcanvas.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qvbox.h>
#include <qhbox.h>
#include "ktdialogbase.h"

#include "kteditspinbox.h"

class Brush;

/**
 * @brief Class that handles the dialog box for creating and editing brushes
 *
 * <b>Date of Creation: May 27 - 2004.</b>\n
 * This is a class that contains a QListView that shows the available brushes and provides
 * mechanisms for adding, editing, removing and previewing the brushes or their attributes.
 */
class Brushes : public KTDialogBase
{
	Q_OBJECT
	
	public:
	//! Constants for defining brush attribute's ranges
	enum BrushConstants
	{
		THICKNESS_MIN_MIN = 1, /**< Constant that defines the lowest value of the brush minimum thickness */
		THICKNESS_MIN_MAX = 99, /**< Constant that defines the highest value of the brush minimum thickness */
		THICKNESS_MAX_MIN = 1, /**< Constant that defines the lowest value of the brush maximum thickness */
		THICKNESS_MAX_MAX = 99, /**< Constant that defines the highest value of the brush maximum thickness */
		SMOOTHNESS_MIN = 0, /**< Constant that defines the lowest value of the brush smoothness */
		SMOOTHNESS_MAX = 9 /**< Constant that defines the highest value of the brush smoothness */
	};
	
	/**
	* @brief Default Constructor
	*
	* Constructs a Brushes dialog box.
	* @param parent The parent widget
	* @param style Flags passed to the QDialog constructor
	* @param in_assigned_menu The assigned menu bar popup menu
	* @param id_assigned_item The assigned menu item from the assigned menu
	* @param assig_tb_button The assigned tool bar button
	*/
	Brushes( QWidget *parent=0, WFlags style=0/*, QPopupMenu *in_assigned_menu, int id_assigned_item, QToolButton *assig_tb_button */);
	/**
	* @brief Default Destructor
	*
	* Destroys the Brushes dialog box.
	*/
	~Brushes();
	
	/**
	* @brief Loads the specified brushes
	*
	* @param brushes The list of brushes
	*/
	void loadBrushes( QPtrList<Brush> brushes );
	
	private:
		//General Purpose Variables
// 		QPopupMenu *assigned_menu;
// 		int assigned_item;
// 		QToolButton *assigned_tb_button;
		int brush_max_value;
		int number_of_brushes;
		QString current_name;
		
		//Icons
		QPixmap i_add_brush, i_remove_brush;
		
		//Buttons
		void setupButtons();
		QPushButton *add_brush, *remove_brush;
		
		//Table that shows the available brushes with their attributes
		void setupTableBruches();
		QListView *table_brushes;
		QListViewItem *default_brush;
		
		void setupStaticText();
		//Static Text name
		QLabel *text_name;
		
		//Textfields for insert the values of name
		QLineEdit  *value_name;
		
		
		KTEditSpinBox *m_displayMinThickness, *m_displayMaxThickness, *m_displaySmoothness;
		
		//Previsualization area
		void setupPrevisualizationArea();
		QCanvasView *previsualization_container;
		QCanvas *previsualization;
		QCanvasEllipse *circle_min_thickness;
		QCanvasEllipse *circle_max_thickness;
	
	signals:
	/**
	* This signal is emitted when the minimum thickness has changed.
	*/
	void minThicknessChanged();
	/**
	* This signal is emitted when the maximum thickness has changed.
	*/
	void maxThicknessChanged();
	/**
	* This signal is emitted when the smoothness has changed.
	*/
	void smoothnessChanged();
	
	public slots:
	/**
	* @brief Adds a new default Brush
	*
	* The default brush has a minimum thickness of 2, a maximum thickness of 5, and a smoothness of 2.
	* @sa slotRemoveBrush(), slotSelectBrush()
	*/
	void slotAddBrush();
	/**
	* @brief Removes the selected Brush
	*
	* @sa slotAddBrush(), slotSelectBrush()
	*/
	void slotRemoveBrush();
	/**
	* @brief Performs a selection on the clicked Brush
	*
	* This slot displays all the properties values of the selected brush into the dialog box.
	* @sa slotAddBrush(), slotRemoveBrush()
	*/
	void slotSelectBrush();
	/**
	* @brief Changes the minimimum thickness textfield value
	*
	* The value is modified after the user moves the corresponding slider.
	* It also modifies the current brush minimum thickness.
	* @sa slotChangeValueMaxThickness(), slotChangeValueSmoothness()
	*/
	void slotChangeValueMinThickness();
	/**
	* @brief Changes the maximimum thickness textfield value
	*
	* The value is modified after the user moves the corresponding slider.
	* It also modifies the current brush maximum thickness.
	* @sa slotChangeValueMinThickness(), slotChangeValueSmoothness()
	*/
	void slotChangeValueMaxThickness();
	/**
	* @brief Changes the smoothness textfield value
	*
	* The value is modified after the user moves the corresponding slider.
	* It also modifies the current brush maximum smoothness.
	* @sa slotChangeValueMinThickness(), slotChangeValueMaxThickness()
	*/
	void slotChangeValueSmoothness();
	/**
	* @brief Changes the current brush name
	*
	* The value is modified after the user presses enter or clicks outside the corresponding textfield.
	*/
	void slotChangeValueName();

	
	protected:
	/**
	* @brief Event for dialog box closing control
	*
	* It updates some main window features as the assigned menu item from the assigned menu.
	* Reimplemented from QWidget.
	* @param close_event The input event
	*/
// 	void closeEvent( QCloseEvent *close_event );
		 
};

#endif
