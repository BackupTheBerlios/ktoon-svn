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

#include <qmessagebox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "preferences.h"
#include "ktapplication.h"

//--------------- CONSTRUCTOR --------------------

Preferences::Preferences( QWidget *parent ) : Q3TabDialog( parent, "Application Preferences", true )
{
	setCaption( tr( "Application Preferences" ) );
	resize( 220, 230 );

	setCancelButton();
	connect( this, SIGNAL( applyButtonPressed() ), SLOT( applyChanges() ) );
	
	m_themeSelector = new KTThemeSelector(this);
	addTab(m_themeSelector, tr("Theme preferences"));
// 	m_colorPref = new ColorSchemePref(this);
// 	addTab(m_colorPref, tr("Color Preferences"));
	
	m_fontWidget = new KTFontWidget(this);
	m_fontWidget->setFont( QFont("helvetica", 10));
	m_fontWidget->showXLFDArea(false);
	addTab(m_fontWidget, tr("Font"));
}

//-------------- DESTRUCTOR -----------------

Preferences::~Preferences()
{
}

//------------------- SLOTS -------------------------

void Preferences::applyChanges()
{
	if ( static_cast<KTThemeSelector *>(currentPage()) ==  m_themeSelector)
	{
		if(m_themeSelector->iWantApplyColors() )
		{
			ktapp->applyTheme(m_themeSelector->document());
			QString filepath = m_themeSelector->lastFile();
			
			if ( ! filepath.isEmpty() )
			{
				KTCONFIG->configDocument()->setThemePath(filepath);
			}
		}
	}
	else if ( static_cast<KTFontWidget *>( currentPage ()) == m_fontWidget )
	{
		ktapp->changeFont(m_fontWidget->font());
	}
}
