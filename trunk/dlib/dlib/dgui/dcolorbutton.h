/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef DCOLORBUTTON_H
#define DCOLORBUTTON_H

#include <QAbstractButton>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "dcore/dglobal.h"

class QColor;

class D_GUI_EXPORT DColorButton : public QAbstractButton
{
	Q_OBJECT;
	Q_PROPERTY( QColor color READ color WRITE setColor );

	public:
		DColorButton( QWidget* parent = 0);
		~DColorButton();

		void setColor( const QColor& );
		QColor color() const;

		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		void setPalette ( const QPalette & );

	public slots:
		virtual void showEditor();

	signals:
		void colorChanged(const QColor &color);

	protected:
		void paintEvent(QPaintEvent *e);
		void mousePressEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void dragEnterEvent(QDragEnterEvent* e);
		void dragMoveEvent(QDragMoveEvent* e);
		void dropEvent(QDropEvent* e);

	private:
		QColor m_color;
		QPoint m_position;
};

#endif //DCOLORBUTTON_H
