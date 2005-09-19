/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@toonka.com   						   *
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

#ifndef KTANIMWIDGET_H
#define KTANIMWIDGET_H

#include <q3canvas.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QHideEvent>
#include <QShowEvent>
#include <Q3ValueList>

typedef Q3ValueList<QPixmap> ListOfPixmaps;

/**
 * @Author David Cuadrado
 */
 
class KTAnimWidget : public Q3CanvasView
{
	public:
		enum Type { AnimText = 0, AnimPixmap };
		KTAnimWidget(const QPixmap &px, const QString &text, QWidget *parent = 0);
		KTAnimWidget(ListOfPixmaps lop, QWidget *parent = 0);
		
		~KTAnimWidget();
		
		void setBackgroundPixmap(const QPixmap &px);
		
		bool isItemVisible();
		
		void stop();
		void follow();
		void start();
		
		void showEvent ( QShowEvent * e);
		void hideEvent ( QHideEvent * e);
		
	private:
		Type m_type;
		Q3Canvas *m_canvas;
		Q3CanvasItem *m_item;
};

#endif
