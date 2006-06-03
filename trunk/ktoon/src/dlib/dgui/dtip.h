/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef DTIP_H
#define DTIP_H

#include <QString>
#include <QList>
#include <QDialog>

class QCheckBox;
class QTextBrowser;

/**
 * @struct DTip
 * @author David Cuadrado <krawek@toonka.com>
*/

struct DTip
{
	QString text;
};

/**
 * @class DTipDatabase
 * @author David Cuadrado <krawek@toonka.com>
 */

class DTipDatabase
{
	public:
		DTipDatabase(const QString &file);
		~DTipDatabase();
		DTip tip() const;
		void nextTip();
		void prevTip();
		
	private:
		void loadTips(const QString &file);
		
	private:
		
		
		QList<DTip> m_tips;
		int m_currentTipIndex;
};

/**
 * @class DTipDialog
 * @author David Cuadrado <krawek@toonka.com>
 */

class DTipDialog : public QDialog
{
	Q_OBJECT;
	public:
		DTipDialog(const QString &file, QWidget *parent = 0);
		DTipDialog(DTipDatabase *database, QWidget *parent = 0);
		~DTipDialog();
		
	private:
		void setupGUI();
		
	private slots:
		void showPrevTip();
		void showNextTip();
		
		void setShowOnStart();
		
	private:
		QTextBrowser *m_textArea;
		QCheckBox *m_showOnStart;
		DTipDatabase *m_database;
};

#endif
