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

#ifndef EXPORT_H
#define EXPORT_H

/**
 * @file export.h
 * @brief Include this file if you need the class Export
 */

#include <qdialog.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>

class QStringList;
class SWFDisplayItem;
class SWFMovie;
class SWFBitmap;
class SWFFill;

/**
 * @brief Class that handles the dialog box to export the animation
 *
 * It is possible to export the whole animation to a sequence of PNGs or export a single frame to a PNG.
 * <b>Date of Creation: November 24 - 2004.</b>\n
 */
class Export : public QDialog
{
	Q_OBJECT

	public:
		/**
		* @brief Default Constructor
		*
		* Constructs an Export dialog box.
		* @param parent The parent widget
		*/
		Export( QWidget *parent );
		/**
		* @brief Default Destructor
		*
		* Destroys the Export dialog box.
		*/
		~Export();
		
		/**
		* @brief Generates an SWF image animation
		*
		* @param movie The movie where is going to be saved the animation
		* @param images A list of strings, each one of them containing the path of an image
		* @return A pointer to a display item generated by the movie
		*/
		//     SWFDisplayItem *doAnimation( SWFMovie *movie, const QStringList &images );
					// TODO: Include ming and decomment this
		/**
		* @brief Constructs an SWF Movie
		*
		* @param width The movie width
		* @param height The movie height
		*/
		//     SWFMovie *doMovie( int width, int height );
	
	private:
		//General Purpose Variables
// 		QWidget *parent_widget;
// 		KToon *k_toon;
		
		//Buttons
		QPushButton *accept, *cancel;
		
		//Static Text
		QLabel *main_text;
		
		//Radio buttons
		QButtonGroup *group;
		QRadioButton *rb_animation, *rb_single_frame, *rb_swf, *rb_svg;
	
	protected slots:
		/**
		* @brief Shows a file chooser dialog box according to the selected kind of export option
		*
		* This function is called when the user presses the Accept button.
		*/
		void slotAccept();
		/**
		* @brief Discards an export operation
		*
		* This function is called when the user presses the Cancel button.
		*/
		void slotCancel();
	
	signals:
		void sendMessage(const QString &message);
		void selectToExport(int);
			 
	protected:

};

#endif
