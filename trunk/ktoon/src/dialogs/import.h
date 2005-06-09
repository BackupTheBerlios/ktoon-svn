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

#ifndef IMPORT_H
#define IMPORT_H

/**
 * @file import.h
 * @brief Include this file if you need the class Import
 */

#include <qdialog.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>

class KToon;

/**
 * @brief Class that handles the dialog box to import files of known types
 *
 * It is possible to import images, sequences of images, color palettes, brushes or graphic lybraries.
 * <b>Date of Creation: September 22 - 2004.</b>\n
 */
class Import : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an Import dialog box.
     * @param parent The parent widget
     */
    Import( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the Import dialog box.
     */
    ~Import();

private:
    //General Purpose Variables
    QWidget *parent_widget;
    KToon *k_toon;

    //Buttons
    QPushButton *accept, *cancel;

    //Static Text
    QLabel *main_text;

    //Radio buttons
    QButtonGroup *group;
    QRadioButton *rb_image, *rb_image_sequence, *rb_library, *rb_palette, *rb_brushes, *rb_sound;

protected slots:
    /**
     * @brief Shows a file chooser dialog box according to the selected kind of import option
     *
     * This function is called when the user presses the Accept button.
     */
    void slotAccept();
    /**
     * @brief Discards an import operation
     *
     * This function is called when the user presses the Cancel button.
     */
    void slotCancel();

protected:

};

#endif
