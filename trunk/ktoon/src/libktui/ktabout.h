/***************************************************************************
 *   Copyright (C) 2004 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#ifndef KTABOUT_H
#define KTABOUT_H

#include <qlabel.h>
#include <qpixmap.h>

#include <dgui/dtabdialog.h>
#include <dgui/danimwidget.h>

/**
 * @brief Class that handles the about dialog box
 *
 * <b>Date of Creation: July 28 - 2004.</b>\n
 * <b>Rewritten in May 30 - 2005</b>\n
 * This is a dialog box that contains info about the authors of the application,
 * the application itself and another related stuff. It's a subclass of QTabDialog,
 * and provides 6 Tabs:
 * - Copyrights
 * - Credits
 * - Acknowledgements
 * - SVN Log
 * - KTAbout Toonka Films
 * - GNU Public License
 *
 * Moreover, it provides an OK Button and a Cancel Button.
 */

class KTAbout : public DTabDialog
{
	Q_OBJECT

	public:
		/**
		* @brief Default Constructor
		*
		* Constructs an KTAbout Dialog.
		* @param parent The parent widget
		*/
		KTAbout( QWidget *parent );
		/**
		* @brief Default Destructor
		*
		* Destroys the KTAbout Dialog.
		*/
		~KTAbout();
	
	private:
		DAnimWidget *m_credits;
};

#endif
