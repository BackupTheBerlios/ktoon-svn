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

#ifndef INSTALLPAGE_H
#define INSTALLPAGE_H

#include <ktwizard.h>
#include <qprogressbar.h>
#include <QDir>
#include <QList>
#include <QFileInfo>
#include <QFile>

#include <QLineEdit>

class QCheckBox;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class InstallPage : public KTWizardPage
{
	Q_OBJECT
	public:
		InstallPage(QWidget *parent = 0);
		~InstallPage();
		bool isComplete();
		void reset();
		
	public slots:
		void aboutToFinish();
		
	private:
		void generateLauncher();
		void generateMenuEntry(const QString &path);
		void launchApplication();
		
	private slots:
		void chooseDestination();
		void install();
		void installDir(const QDir &dir);
		
	private:
		QProgressBar *m_status;
		bool m_isComplete;
		QLineEdit *m_destinationPath;
		
		QCheckBox *m_launchApp;
};

#endif
