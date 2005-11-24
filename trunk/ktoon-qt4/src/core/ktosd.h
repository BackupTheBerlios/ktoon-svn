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

#ifndef KTOSD_H
#define KTOSD_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTOsd : public QWidget
{
	Q_OBJECT
	public:
		enum Level
		{
			None,
			Info,
			Warning,
			Error,
			Fatal
		};
		KTOsd(QWidget *parent = 0);
		~KTOsd();
		
		void display( const QString & message, Level level = Info, int ms = 4000 );

	protected:
		void paintEvent( QPaintEvent * e );
		void mousePressEvent( QMouseEvent * e );

	private:
		QPixmap m_pixmap;
		QTimer *m_timer;

};

#endif
