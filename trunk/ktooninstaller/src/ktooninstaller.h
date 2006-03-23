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



#ifndef KTOONINSTALLER_H
#define KTOONINSTALLER_H

#include <QMainWindow>
#include <dgui/dwizard.h>

#include "welcomepage.h"
#include "licencepage.h"
#include "installpage.h"

class KToonInstaller: public DWizard
{
	Q_OBJECT

	public:
		KToonInstaller();
		~KToonInstaller();

	protected:
		void closeEvent( QCloseEvent* );

	private:
		WelcomePage *m_welcomePage;
		LicencePage *m_licencePage;
		InstallPage *m_installPage;
};


#endif
