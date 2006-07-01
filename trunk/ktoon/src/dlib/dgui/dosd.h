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
#include <QTimer>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class DOsd : public QWidget
{
	Q_OBJECT
	public:
		enum Level
		{
			None = -1,
			Info,
			Warning,
			Error,
			Fatal
		};
		DOsd(QWidget *parent = 0);
		~DOsd();
		
		void display( const QString & message, Level level = Info, int ms = -1 );
		
	private slots:
		void animate();

	protected:
		void paintEvent( QPaintEvent * e );
		void mousePressEvent( QMouseEvent * e );
		
	private:
		void drawPixmap(const QString &message, const QBrush &background, const QBrush &foreground);

	private:
		QPixmap m_pixmap;
		QTimer *m_timer;
		
		QPalette m_palette; 
		
		QString m_lastMessage;
		
		struct Animation
		{
			QTimer timer;
			Level level;
			bool on;
		} *m_animator;

};

#endif
