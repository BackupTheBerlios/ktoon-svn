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

#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

#include "preferences.h"
#include "ktapplication.h"

#include "ktdebug.h"

#include <QLabel>

//--------------- CONSTRUCTOR --------------------

Preferences::Preferences( QWidget *parent ) : KTConfigurationDialog(parent )
{
	setWindowTitle( tr( "Application Preferences" ) );
	
// 	addSection( new QLabel("<h3>You can configure general settings<br> in this section</h3>"), tr("General") );
	
// 	addSection( new QLabel("<h3>You can configure user interface settings <br>in this section</h3>"),tr("User interface") );
	m_themeSelector = new KTThemeSelector;
	addPage(m_themeSelector, tr("Theme preferences"), QPixmap(KTOON_THEME_DIR+"/icons/play.png"));

	m_fontWidget = new KTFontWidget;
// 	m_fontWidget->setFont( QFont("helvetica", 10));
// 	m_fontWidget->showXLFDArea(false);
	addPage(m_fontWidget, tr("Font"), QPixmap(KTOON_THEME_DIR+"/icons/ff.png"));
}

//-------------- DESTRUCTOR -----------------

Preferences::~Preferences()
{
}

void Preferences::ok()
{
	apply();
	KTConfigurationDialog::ok();
}

void Preferences::apply()
{
	if ( static_cast<KTThemeSelector *>(currentPage()) ==  m_themeSelector)
	{
		if(m_themeSelector->iWantApplyColors() )
		{
			ktapp->applyTheme(m_themeSelector->document());
			QString filepath = m_themeSelector->lastFile();
			
			if ( ! filepath.isEmpty() )
			{
				KTCONFIG->setValue("General", filepath);
			}
		}
	}
// 	else if ( static_cast<KTFontWidget *>( currentTab ()) == m_fontWidget )
// 	{
// 		ktapp->changeFont(m_fontWidget->font());
// 	}
}

