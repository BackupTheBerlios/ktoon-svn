/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef KTFILEDIALOG_H
#define KTFILEDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTFileDialog : public QDialog
{
	Q_OBJECT
	public:
		enum Type
		{
			Repository = 0,
			Themes
		};
		
		KTFileDialog(Type t, QWidget *parent = 0);
		~KTFileDialog();
		QString fileName() const;
		void readFiles();
		
	private slots:
		void accept();
		void select(Q3ListViewItem *);
// 		void reject();
		
	private:
		QLineEdit *m_fileNameLE;
		Q3ListView *m_listView;
		QPushButton *m_accept, *m_cancel;
		QString m_fileName;
		Type m_type;
		
};

#endif