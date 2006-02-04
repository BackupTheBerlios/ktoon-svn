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

#ifndef TEXTCONFIGURATOR_H
#define TEXTCONFIGURATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class TextConfigurator : public QWidget
{
	Q_OBJECT
	public:
		TextConfigurator(QWidget *parent = 0);
		~TextConfigurator();
		QString text() const;
		QString family() const;
		int size() const;
		QFont textFont() const;
		
	private:
		QLineEdit *m_text;
		QComboBox *m_families;
		QSpinBox *m_size;
		QFont m_font;
		
	public slots:
		void setFamily(const QString & family);
		void setSize(int size);
		
};

#endif
