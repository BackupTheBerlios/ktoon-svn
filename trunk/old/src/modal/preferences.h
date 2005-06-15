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

#ifndef PREFERENCES_H
#define PREFERENCES_H

/*!
 * \file preferences.h
 * \brief Include this file if you need the class Preferences
 */

#include <qtabdialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

/*!
 * \brief Class that handles the dialog box to set application's preferences
 *
 * <b>Date of Creation: June 9 - 2004.</b>\n
 */
class Preferences : public QTabDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a Preferences dialog box.
     * \param parent The parent widget
     */
    Preferences( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the Preferences dialog box.
     */
    ~Preferences();

private:
    //General Purpose Variables
    QWidget *parent_widget;

    //Widgets for the tabs:
    //1:
    QFrame *language;
    QButtonGroup *language_group;
    QRadioButton *rb_system, *rb_english, *rb_spanish, *rb_french;

public slots:
    /*!
     * \brief Confirms all preferences settings
     *
     * This function is called when the user presses the OK button.
     */
    void slotOK();
    
protected:

};

#endif
