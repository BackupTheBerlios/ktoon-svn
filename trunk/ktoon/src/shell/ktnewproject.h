/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include <kgui/kosd.h>
#include <kgui/ktabdialog.h>
#include <kgui/kwizard.h>
#include <kgui/kxyspinbox.h>

class KTProjectManagerParams;

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

class KTNewProject : public KTabDialog
{
	Q_OBJECT
	public:
		KTNewProject(QWidget *parent = 0);
		~KTNewProject();
		// QString projectName() const;
		// QSize dimension() const;
		// int fps() const;
		
		KTProjectManagerParams *params();
		
		bool useNetwork() const;
		
	public slots:
		void ok();
		
	private:
		void setupNetOptions();
		
	public slots:
		void activateNetOptions(bool isVisible);
	
	private:
		struct Private;
		Private *const k;
		
};
#if 0
class KTNewProject : public KWizard
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

class NPFirstPage : public KWizardPage
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
		
		KXYSpinBox *m_size;
};
#endif
#endif
