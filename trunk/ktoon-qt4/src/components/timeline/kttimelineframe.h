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

#ifndef KTTIMELINEFRAME_H
#define KTTIMELINEFRAME_H

#include <QWidget>
#include <QMouseEvent>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTTimeLineFrame : public QWidget
{
	Q_OBJECT
	public:
		KTTimeLineFrame(int id, QWidget *parent = 0);
		~KTTimeLineFrame();
		void setSelected(bool opt);
		void setSpecial(bool opt);
		void setKey(bool opt);
		void setUsed(bool opt);
		void setOffset(bool opt);
		void setId(int id);
		
	protected:
		void mousePressEvent( QMouseEvent *mEv );
		void paintEvent( QPaintEvent *pEv );
		virtual void paintSelected(QPainter *p);
		virtual void paintNotSelected(QPainter *p);
		
	private:
		bool m_isUsed, m_isSpecial, m_isSelected, m_isOffset, m_isKey;
		int m_ID;
		
	signals:
		void selected(int);
};

#endif
