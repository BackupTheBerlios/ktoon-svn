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

#ifndef PEN_PROPERTIES_H
#define PEN_PROPERTIES_H

/*!
 * \file penproperties.h
 * \brief Include this file if you need the class PenProperties
 */

#include <qdialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>

class GLPen;
class DrawingArea;

/*!
 * \brief Class that handles the dialog box to set the pen's properties
 *
 * <b>Date of Creation: September 13 - 2004.</b>\n
 */
class PenProperties : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a PenProperties dialog box.
     * \param parent The parent widget
     */
    PenProperties( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the PenProperties dialog box.
     */
    ~PenProperties();

    /*!
     * \brief Loads data from a pencil
     */
     void loadFromGraphic( GLPen *pen );

private:
    //General Purpose Variables
    QWidget *parent_widget;

    //Buttons
    QPushButton *accept, *cancel;

    //Static Text
    QLabel *text_origin, *text_origin_x, *text_origin_y, *text_pattern, *text_factor, *text_angle;

    //Textfields
    QLineEdit *value_origin_x, *value_origin_y, *value_factor, *value_angle;

    //Other
    QComboBox *value_pattern;

    DrawingArea *drawing_area;

public slots:
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
