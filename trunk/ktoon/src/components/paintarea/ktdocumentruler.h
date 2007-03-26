/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#ifndef KTDOCUMENTRULER_H
#define KTDOCUMENTRULER_H

#include <dgui/drulerbase.h>

/**
 * @author Jorge Cuadrado
*/

class KTDocumentRuler : public DRulerBase
{
	Q_OBJECT
	public:
		KTDocumentRuler(Qt::Orientation orientation=Qt::Horizontal, QWidget *parent = 0, const char *name = 0);
		~KTDocumentRuler();
		
		
	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	
	public slots:
		void movePointers(const QPointF &pos);
		
	private:
		struct Private;
		Private * const d;
};

#endif
