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

#ifndef DCONFIGURATIONAREA_H
#define DCONFIGURATIONAREA_H

#include <QDockWidget>
#include <QTimer>

class QPushButton;

/**
 * @author David Cuadrado \<krawek@toonka.com\>
*/
class KTConfigurationArea : public QDockWidget
{
	Q_OBJECT
	public:
		KTConfigurationArea(QWidget *parent = 0);
		~KTConfigurationArea();
		
		void setConfigurator(QWidget *widget);
		
		QSize sizeHint() const;
		
	public slots:
		void hideConfigurator();
		void showConfigurator();
		
	private:
		void shrink();
		
	private slots:
		void toggleLock();
		
	protected:
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		void paintEvent (QPaintEvent *e);
		
	private:
		QTimer m_locker;
		QTimer m_shower;
		bool m_toolTipShowed;
		
		QPoint m_mousePos;
};

#endif






