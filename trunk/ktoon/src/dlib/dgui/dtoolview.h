/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                *
 *   krawek@gmail.com                                                      *
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

#ifndef DTOOLVIEW_H
#define DTOOLVIEW_H

#include <QDockWidget>
#include <QIcon>

class DViewButton;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_GUI_EXPORT DToolView : public QDockWidget
{
	Q_OBJECT;
	
	public:
		DToolView(const QString &title, const QIcon &icon = QIcon(), QWidget * parent = 0);
		~DToolView();
		
		void setDescription(const QString &description);
		DViewButton *button() const;
		QSize sizeHint() const;
		
	private:
		void setup();
		
		
	private slots:
		void saveSize(bool checked);
		
	private:
		DViewButton *m_button;
		int m_size;
};

#endif
