/***************************************************************************
 *   Copyright (C) 2004 by David Cuadrado                                  *
 *   krawek@toonka.com                                                    *
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

/**
 * @file preferences.h
 * @brief Include this file if you need the class Preferences
 */

#include <qlineedit.h>
#include <qlabel.h>
#include <qradiobutton.h>

#include "ktfontwidget.h"
#include "ktthemeselector.h"

#include "kttabdialog.h"

#include "ktconfigurationdialog.h"

#include "ktflatbutton.h"

class ColorSchemePref;

class Preferences : public KTConfigurationDialog
{
	Q_OBJECT
			
	
	public:
		Preferences( QWidget *parent );
		~Preferences();
		
		
	private:
		KTThemeSelector *m_themeSelector;
		KTFontWidget *m_fontWidget;

	public slots:
		void apply();
		void ok();
    
	protected:
};

#endif
