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

#ifndef KTPREFERENCES_H
#define KTPREFERENCES_H

/**
 * @file ktpreferences.h
 * @brief Include this file if you need the class KTPreferences
 */

#include <qlineedit.h>
#include <qlabel.h>
#include <qradiobutton.h>

#include <dgui/dfontchooser.h>
#include <dgui/dtabdialog.h>
#include <dgui/dconfigurationdialog.h>
#include <dgui/dflatbutton.h>

#include "ktthemeselector.h"
#include "ktglobal.h"

class ColorSchemePref;
class KTPaintAreaConfig;

class KTPreferences : public DConfigurationDialog
{
	Q_OBJECT;
	
	public:
		KTPreferences( QWidget *parent );
		~KTPreferences();
		
		
	private:
		class GeneralPage;
		class FontPage;
		
		KTThemeSelector *m_themeSelector;
		FontPage *m_fontChooser;
		GeneralPage *m_generalPage;
		KTPaintAreaConfig *m_drawingAreaProperties;
		
	public slots:
		void apply();
		void ok();
		
	protected:
};

#endif

