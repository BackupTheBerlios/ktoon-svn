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

#ifndef DRADIOBUTTONGROUP_H
#define DRADIOBUTTONGROUP_H

#include <QGroupBox>
#include <QRadioButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <dcore/dglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DRadioButtonGroup : public QGroupBox
{
	Q_OBJECT
	public:
		DRadioButtonGroup(const QString &title, Qt::Orientation orientation, QWidget *parent = 0);
		~DRadioButtonGroup();
		
		void addItem ( const QString & text, int index = -1 );
		void addItems(const QStringList &texts);
		
		int currentIndex() const;
		void setCurrentIndex ( int index );
		
	private slots:
		void emitButtonId(QAbstractButton *button);
		
	signals:
		void clicked(int index);
		
	private:
		Qt::Orientation m_orientation;
		QBoxLayout *m_layout;
		QButtonGroup *m_buttonGroup;
};

#endif
