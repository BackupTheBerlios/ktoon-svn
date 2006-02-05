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

#include "ktpreferences.h"
#include "ktapplication.h"

#include "ktdebug.h"

#include <QLabel>
#include <QLineEdit>

#include "ktformfactory.h"

class KTPreferences::GeneralPage : public QWidget
{
	public:
		GeneralPage();
		~GeneralPage();
		void saveValues();
		
	private:
		QLineEdit *m_home, *m_repository, *m_browser;
};

KTPreferences::GeneralPage::GeneralPage()
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	KTCONFIG->beginGroup("General");
	
	m_home = new QLineEdit;
	layout->addLayout( KTFormFactory::makeLine(tr("KToon Home"), m_home, Qt::Horizontal ) );
	
	QString str = KTCONFIG->value("Home").toString();
	if ( !str.isEmpty())
	{
		m_home->setText(str);
	}
	
	m_repository = new QLineEdit;
	layout->addLayout( KTFormFactory::makeLine(tr("Repository"), m_repository, Qt::Horizontal ) );
	str = KTCONFIG->value("Repository").toString();
	if ( !str.isEmpty())
	{
		m_repository->setText(str);
	}
	
	m_browser = new QLineEdit;
	layout->addLayout( KTFormFactory::makeLine(tr("Browser"), m_browser, Qt::Horizontal ) );
	str = KTCONFIG->value("Browser").toString();
	if ( !str.isEmpty())
	{
		m_browser->setText(str);
	}
	
	layout->addStretch(3);
}

KTPreferences::GeneralPage::~GeneralPage()
{
}

void KTPreferences::GeneralPage::saveValues()
{
	KTCONFIG->beginGroup("General");
	
	QString str = m_home->text();
	if ( !str.isEmpty() && m_home->isModified ())
	{
		KTCONFIG->setValue("Home", str);
	}
	
	str = m_repository->text();
	if ( !str.isEmpty() && m_repository->isModified () )
	{
		KTCONFIG->setValue("Repository", str);
	}
	
	str = m_browser->text();
	if ( !str.isEmpty() && m_browser->isModified () )
	{
		KTCONFIG->setValue("Browser", str);
	}
	
	KTCONFIG->sync();
}

//--------------- CONSTRUCTOR --------------------

KTPreferences::KTPreferences( QWidget *parent ) : KTConfigurationDialog(parent )
{
	setWindowTitle( tr( "Application KTPreferences" ) );
	
	m_generalPage = new GeneralPage;
	addPage(m_generalPage, tr("General"), QPixmap(KTOON_THEME_DIR+"/icons/ff.png"));
	
	m_themeSelector = new KTThemeSelector;
	addPage(m_themeSelector, tr("Theme preferences"), QPixmap(KTOON_THEME_DIR+"/icons/play.png"));

	m_fontChooser = new KTFontChooser;
	addPage(m_fontChooser, tr("Font"), QPixmap(KTOON_THEME_DIR+"/icons/rw.png"));
}

//-------------- DESTRUCTOR -----------------

KTPreferences::~KTPreferences()
{
}

void KTPreferences::ok()
{
	apply();
	KTConfigurationDialog::ok();
}

void KTPreferences::apply()
{
	if ( static_cast<KTThemeSelector *>(currentPage()) ==  m_themeSelector)
	{
		if(m_themeSelector->iWantApplyColors() )
		{
			ktapp->applyTheme(m_themeSelector->document());
		}
	}
	else if ( static_cast<GeneralPage *>( currentPage()) == m_generalPage )
	{
		m_generalPage->saveValues();
	}
	else if ( qobject_cast<KTFontChooser *>(currentPage() ) == m_fontChooser )
	{
		ktapp->setFont(m_fontChooser->font());
	}
}

