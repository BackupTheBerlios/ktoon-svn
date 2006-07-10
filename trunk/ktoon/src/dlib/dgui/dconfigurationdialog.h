/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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
 
#ifndef DCONFIGURATIONDIALOG_H
#define DCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QButtonGroup>

#include "dgui/dwidgetlistview.h"
#include "dgui/dflatbutton.h"

#include "dgui/dpagedialog.h"

class QStackedWidget;
class QTreeWidget;
class QTableWidgetItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class DConfigurationDialog : public DPageDialog
{
	Q_OBJECT
	public:
		DConfigurationDialog(QWidget *parent = 0);
		~DConfigurationDialog();
		
	public slots:
		virtual void ok();
		virtual void cancel();
		virtual void apply();
};

#endif
