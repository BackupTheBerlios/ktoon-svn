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

#ifndef PROPERTIES_H
#define PROPERTIES_H

/*!
 * \file properties.h
 * \brief Include this file if you need the class Properties
 */

#include <qdialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

class Ktoon;

/*!
 * \brief Class that handles the dialog box to set animation's properties
 *
 * It provides a way to set the animation frame rate and the camera size.
 * <b>Date of Creation: June 1 - 2004.</b>\n
 */
class Properties : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a Properties dialog box.
     * \param parent The parent widget
     */
    Properties( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the Properties dialog box.
     */
    ~Properties();

private:
    //General Purpose Variables
    QWidget *parent_widget;
    Ktoon *k_toon;
    QColor c_grid, c_background, c_ntsc;

    //Buttons
    QPushButton *accept, *cancel, *background_color, *grid_color, *ntsc_color;

    //Static Text
    QLabel *text_frame_rate, *text_camera_size, *text_background_color, *text_grid_color, *text_ntsc_color;

    //Textfields for insert the values of Frame Rate, Camera Width and Camera Height
    QLineEdit *value_frame_rate, *value_camera_size_width, *value_camera_size_height, *value_background_color,
    	      *value_grid_color, *value_ntsc_color;
    
public slots:
    /*!
     * \brief Opens a dialog box where the user may choose the drawing area's background color
     */
    void slotBackgroundColor();
    /*!
     * \brief Opens a dialog box where the user may choose the drawing area grid's color
     */
    void slotGridColor();
    /*!
     * \brief Opens a dialog box where the user may choose the NTSC border's color
     */
    void slotNTSCColor();
    /*!
     * \brief Changes the background color if the argument string is a valid color name
     *
     * A valid color has the #XXXXXX format, within the range #000000 to #FFFFFF.
     * It is valid also to pass a string with a color name such as "blue", "red", etc. See the Qt
     * documentation for QColor and the function setColorName() for a list of valid values.
     * \param rgb The color name
     */
    void slotChangeBackgroundColor( const QString &rgb );
    /*!
     * \brief Changes the grid color if the argument string is a valid color name
     *
     * A valid color has the #XXXXXX format, within the range #000000 to #FFFFFF.
     * It is valid also to pass a string with a color name such as "blue", "red", etc. See the Qt
     * documentation for QColor and the function setColorName() for a list of valid values.
     * \param rgb The color name
     */
    void slotChangeGridColor( const QString &rgb );
    /*!
     * \brief Changes the NTSC color if the argument string is a valid color name
     *
     * A valid color has the #XXXXXX format, within the range #000000 to #FFFFFF.
     * It is valid also to pass a string with a color name such as "blue", "red", etc. See the Qt
     * documentation for QColor and the function setColorName() for a list of valid values.
     * \param rgb The color name
     */
    void slotChangeNTSCColor( const QString &rgb );
    /*!
     * \brief Confirms all properties settings
     *
     * This function is called when the user presses the Accept button.
     */
    void slotAccept();
    /*!
     * \brief Discards all changes to properties settings
     *
     * This function is called when the user presses the Cancel button.
     */
    void slotCancel();

protected:

};

#endif
