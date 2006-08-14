/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DOPTIONALDIALOG_H
#define DOPTIONALDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <dcore/dglobal.h>

/**
 * @if english
 * This class represents a dialog that is optional
 * @elseif spanish
 * Esta clase representa un dialogo que es opcional
 * @endif
 * 
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DOptionalDialog : public QDialog
{
	Q_OBJECT
	public:
		/**
		 * @if english
		 * Default constructor
		 * @elseif spanish
		 * Constructor por defecto
		 * @endif
		 * @param text 
		 * @param title 
		 * @param parent 
		 * @return 
		 */
		DOptionalDialog(const QString &text, const QString &title = 0, QWidget *parent = 0);
		/**
		 * Destructor
		 * @return 
		 */
		~DOptionalDialog();
		
		/**
		 * @if english
		 * This function returns true when the user wants to see the dialog again
		 * @elseif spanish
		 * Esta funcion retorna true cuando el usuario quiere volver a ver el dialogo
		 * @endif
		 * @return 
		 */
		bool shownAgain();
		
	private:
		QBoxLayout *m_layout;
		QCheckBox *m_checkBox;
};

#endif
