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

#include "dwizard.h"
#include "dxyspinbox.h"
#include <QComboBox>
#include <QSpinBox>
#include "dtabdialog.h"
#include <QLineEdit>


/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTNewProject : public DTabDialog
{
	Q_OBJECT
	public:
		KTNewProject(QWidget *parent = 0);
		~KTNewProject();
		QString projectName() const;
		QSize dimension() const;
		int fps() const;
		
	signals:
		void sendToStatus(const QString &);
		void sendToOSD(const QString &, int);
	
	private:
		QLineEdit *m_projectName;
		QLineEdit *m_authorName;
		QSpinBox *m_fps;
		
		DXYSpinBox *m_size;
		
};
#if 0
class KTNewProject : public DWizard
{
	Q_OBJECT
	public:
		KTNewProject(QWidget *parent = 0);
		~KTNewProject();
		QString projectName() const;
		QSize dimension() const;
		int fps() const;
// 		QString renderType() const;
	signals:
		void sendToStatus(const QString &);
		void sendToOSD(const QString &, int);
		
	private:
		NPFirstPage *m_firstPage ;
};

class NPFirstPage : public DWizardPage
{
	Q_OBJECT
	public:
		NPFirstPage(QWidget *parent = 0);
		~NPFirstPage();
		bool isComplete();
		void reset();
		QString projectName() const;
		QSize dimension() const;
		int fps() const;
		QString renderType() const;
		
	signals:
		void sendToStatus(const QString &);
		void sendToOSD(const QString &, int level);
		
	private:
		QLineEdit *m_projectName;
		QLineEdit *m_authorName;
		QComboBox *m_renderType;
		QSpinBox *m_fps;
		
		DXYSpinBox *m_size;
};
#endif
#endif
