/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTIP_H
#define KTIP_H

#include <QString>
#include <QList>
#include <QDialog>
#include <kcore/kglobal.h>

#include "kcore/ktipdatabase.h"

class QCheckBox;
class QTextBrowser;

/**
 * @class KTipDialog
 * @author David Cuadrado <krawek@gmail.com>
 */

class K_GUI_EXPORT KTipDialog : public QDialog
{
	Q_OBJECT;
	public:
		KTipDialog(const QString &file, QWidget *parent = 0);
		KTipDialog(KTipDatabase *database, QWidget *parent = 0);
		~KTipDialog();
		
	private:
		void setupGUI();
		
	private slots:
		void showPrevTip();
		void showNextTip();
		
		void setShowOnStart();
		
	private:
		QTextBrowser *m_textArea;
		QCheckBox *m_showOnStart;
		KTipDatabase *m_database;
};

#endif
