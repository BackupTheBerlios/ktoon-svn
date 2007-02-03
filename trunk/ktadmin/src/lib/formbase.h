/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
#ifndef BASEForm_H
#define BASEForm_H

#include <QFrame>
#include <QDialogButtonBox>
#include <dformvalidator.h>

class QTreeWidgetItem;

namespace Base {


/**
 * @author Jorge Cuadrado <kuadrosxx@gmail.com>
*/
class Form : public QFrame, public DFormValidator
{
	Q_OBJECT
	public:
		Form(const QString &title,  QWidget *parent=0);
		virtual ~Form();
		
		void setCentralWidget(QWidget *widget);
		
		void setButtons(QDialogButtonBox::StandardButtons buttons);
		void setTitle( const QString &title);
		
		virtual void itemDropped(QTreeWidgetItem *item);
		
	public slots:
		virtual void resetForm() = 0;
		virtual void acceptForm() = 0;
		virtual void showHelp();
		
	private slots:
		void _acceptForm();
		
	protected:
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		
	signals:
		void sendPackage(const QString &pkg);
		
	private:
		struct Private;
		Private  * const d;
};

}

#endif
