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

#ifndef COLORPALETTE_H
#define COLORPALETTE_H

/**
 * @file colorpalette.h
 * @brief Include this file if you need the class ColorPalette
 */

#include <qlayout.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qtoolbutton.h>

#include <qvbox.h>
#include <qhbox.h>

#include "ktdialogbase.h"
#include "colorcells.h"
#include "colormixer.h"
#include "outlinecolor.h"
#include "fillcolor.h"
#include "colordisplay.h"
#include "valueselector.h"
#include "colorgradientselector.h"
#include "custompalette.h"
#include "gradientviewer.h"

#include "ktcolor.h"

class KTColor;

/**
 * @brief Class that handles the dialog box with the color palette
 *
 * <b>Date of Creation: June 3 - 2004.</b>\n
 * This is a class that contains several widgets related to color handling such a ColorCells (that is a standard
 * palette), widgets to see the current outline and fill color, RGBA and HSV spin boxes, a ColorDisplay (to see
 * the current color), a textfield with the color #RRGGBB representation, a ColorMixer, a ValueSelector, a gradient
 * manipulation zone and finally a palette where the user can add custom colors.
 * See each widget documentation for more info.
 */
class ColorPalette : public KTDialogBase
{
    Q_OBJECT
    
	public:
	/**
	* @brief Default Constructor
	*
	* Constructs a ColorPalette dialog box.
	* @param parent The parent widget
	* @param style Flags passed to the QDialog constructor
	* @param in_assigned_menu The assigned menu bar popup menu
	* @param id_assigned_item The assigned menu item from the assigned menu
	* @param assig_tb_button The assigned tool bar button
	*/
	ColorPalette( QWidget *parent);
	/**
	* @brief Default Destructor
	*
	* Destroys the ColorPalette dialog box.
	*/
	~ColorPalette();
	
	/**
	* @brief Enables or disables the custom palette block
	*
	* @param state The boolean value of the enable property
	*/
	void enableCustomPalette( bool state );
	/**
	* @brief Loads colors into the custom palette
	*
	* @param colors The colors
	*/
	void loadCustomColors( QPtrList<KTColor> colors );
	/**
	* @brief Deletes every color from the custom palette
	*/
	void clearCustomPalette();
	
	private:
	//General Purpose Variables
// 	QWidget *parent_widget;
// 	QPopupMenu *assigned_menu;
// 	int assigned_item;
// 	QToolButton *assigned_tb_button;
	KTColor *new_outline_color;
	KTColor *new_fill_color;
	void setupBlockStandardPalette();
	void setupBlockColorMixer();
	void setupBlockGradient();
	void setupBlockCustomPalette();
	//Icons
	QPixmap i_add_color, i_remove_color;
	
	//Buttons
	QPushButton *add_color, *remove_color;
	
	//KTColor palette components
	ColorCells *color_cells;
	
	ColorMixer *color_mixer;
	
	ColorDisplay *color_display;
	
	ValueSelector *value_selector;
	
	OutlineColor *outline_color;
	
	FillColor *fill_color;
	
	ColorGradientSelector *gradient;
	
	GradientViewer *grad_viewer;
	
	CustomPalette *custom_palette;
	
	//Combo box for gradient types
	QComboBox *gradient_types;
	
	//Static Text
	QLabel *text_std_palette, *text_gradient, *text_custom_palette, *text_color_name, *text_color_mixer;
	
	//Textfields
	QLineEdit *value_color_name, *value_rgb;
	
	//Spin Boxes
	QSpinBox *value_r, *value_g, *value_b, *value_h, *value_s, *value_v, *value_alpha;
	
	signals:
		/**
		* This signal is emitted when the outline color has changed.
		*/
		void outlineColorChanged();
		/**
		* This signal is emitted when the fill color has changed.
		*/
		void fillColorChanged();
		void documentModified(bool);
	
	public slots:
	/**
	* @brief Adds a the current displayed color to the custom palette
	*
	* @sa slotRemoveFromCustomColors()
	*/
	void slotAddToCustomColors();
	/**
	* @brief Removes the current selected color from the custom palette
	*
	* @sa slotAddToCustomColors()
	*/
	void slotRemoveFromCustomColors();
	/**
	* @brief Sets a new color and synchronizes all its widgets with it
	*
	* @param new_color The new color
	*/
	void slotSetColor( const QColor &new_color );
	/**
	* @brief Sets a new color and synchronizes all its widgets with it
	*
	* This is as an overloaded slot provided only for convenience. It behaves essentially like the above slot. The difference
	* is that the argument of this slot contains the alpha component inside.
	* @param new_color The new color
	*/
		void slotSetColor( KTColor *new_color );
	/**
	* @brief Sets the outline color widget to be active
	*
	* It updates the current color displayed to be the outline color as well as the RBGA and HSV spin boxes.
	* Any color operations from now on will affect the outline color.
	* @sa slotActivateFillColor()
	*/
	void slotActivateOutlineColor();
	/**
	* @brief Sets the fill color widget to be active
	*
	* It updates the current color displayed to be the fill color as well as the RBGA and HSV spin boxes.
	* Any color operations from now on will affect the fill color.
	* @sa slotActivateOutlineColor()
	*/
	void slotActivateFillColor();
	/**
	* @brief Synchronizes the values of the RGB spin boxes when an HSV spin box has changed
	*
	* @sa slotSyncHSV()
	*/
	void slotSyncRGB();
	/**
	* @brief Synchronizes the values of the HSV spin boxes when an RGB spin box has changed
	*
	* @sa slotSyncRGB()
	*/
	void slotSyncHSV();
	/**
	* @brief Synchronizes the values of the RGB spin boxes when a HSV spin box has changed
	*
	* @param value One of the following strings: "None", "Linear", "Radial"
	*/
	void slotActivateGradientType( const QString &value );
	/**
	* @brief Modifies the #RRGGBB representation textfield with the \a rgb parameter.
	*
	* It is valid also to pass a string with a color name such as "blue", "red", etc. See the Qt
	* documentation for QColor and the function setColorName() for a list of valid values.
	* @param rgb The string with the new #RRGGBB representation or color name
	*/
	void slotChangeRGB( const QString &rgb );
	/**
	* @brief Sets a new value for the current color alpha and updates the visualization
	*
	* @param new_alpha The new alpha value
	*/
	void slotUpdateCurrentAlpha( int new_alpha );
	
	protected:
	/**
	* @brief Event for dialog box closing control
	*
	* It updates some main window features as the assigned menu item from the assigned menu.
	* Reimplemented from QWidget.
	* @param close_event The input event
	*/
// 	void closeEvent( QCloseEvent *close_event );
	/**
	* @brief Connects all members with all receivers
	*/
	void makeConnections();
	/**
	* @brief Disconnects all members from all receivers
	*/
	void disconnectMembers();

};

#endif
