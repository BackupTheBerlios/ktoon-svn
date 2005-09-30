/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#ifndef KTNEWPROYECT_H
#define KTNEWPROYECT_H

#include "ktwizard.h"

class NPFirstPage;
class QLineEdit;

/**
	@author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTNewProject : public KTWizard
{
	Q_OBJECT
	public:
		KTNewProject(QWidget *parent = 0);
		~KTNewProject();
		QString projectName() const;
		
	private:
		NPFirstPage *m_firstPage ;
};

class NPFirstPage : public KTWizardPage
{
	Q_OBJECT
	public:
		NPFirstPage(QWidget *parent = 0);
		~NPFirstPage();
		bool isComplete();
		void reset();
		QString projectName() const;
		
	private:
		QLineEdit *m_projectName;
		QLineEdit *m_authorName;
};

#endif
