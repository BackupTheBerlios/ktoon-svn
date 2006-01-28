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

#ifndef KTMODULEWIDGETTITLE_H
#define KTMODULEWIDGETTITLE_H

#include <QFrame>

/**
 * @author David Cuadrado
*/
class KTModuleWidgetTitle : public QFrame
{
	Q_OBJECT
	public:
		KTModuleWidgetTitle(const QString &title, QWidget *parent = 0);
		~KTModuleWidgetTitle();
		QSize sizeHint() const;
		
		
	public slots:
		void setText(const QString &text);
		void setFont(const QFont &font);
		
	signals:
		void doubleClicked();
		
	protected:
		void mouseDoubleClickEvent(QMouseEvent * e );
		void paintEvent(QPaintEvent *e);
		
	private:
		QString m_text;
		QFont m_font;
};

#endif
