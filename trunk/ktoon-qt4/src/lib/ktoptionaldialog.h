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

#ifndef KTOPTIONALDIALOG_H
#define KTOPTIONALDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOptionalDialog : public QDialog
{
	Q_OBJECT
	public:
		KTOptionalDialog(const QString &text, const QString &title = 0, QWidget *parent = 0);
		~KTOptionalDialog();
		bool checked();
		
	private:
		QBoxLayout *m_layout;
		QCheckBox *m_checkBox;
};

#endif
