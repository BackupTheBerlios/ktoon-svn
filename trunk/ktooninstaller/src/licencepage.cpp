/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                *
 *   krawek@toonka.com                                                     *
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

#include "licencepage.h"
#include <QTextBrowser>
#include <QFile>

#include "ddebug.h"

#include "dglobal.h"

LicencePage::LicencePage(QWidget *parent)
	: DWizardPage(tr("Licence"), parent), m_isComplete(false)
{
	QTextBrowser *licence = new QTextBrowser;
	
	QFile licenceFile( DATA_DIR+"/COPYING" );
	if ( licenceFile.open( QIODevice::ReadOnly  | QIODevice::Text) )
	{
		QTextStream stream( &licenceFile );
		while ( ! stream.atEnd() )
		{
			QString line = stream.readLine();
			licence->append(line);
		}
		licenceFile.close();
	}
	
	m_agree = new QCheckBox(tr("I agree"), this);
	connect(m_agree, SIGNAL(clicked()), this, SLOT(toggleAgree()));
	
	setWidget( licence );
}


LicencePage::~LicencePage()
{
}

void LicencePage::toggleAgree()
{
	m_isComplete = m_agree->isChecked();
	emit completed();
}

bool LicencePage::isComplete()
{
	return m_isComplete;
}

void LicencePage::reset()
{
}
