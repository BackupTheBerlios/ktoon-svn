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

#ifndef KTPAINTAREACONTAINER_H
#define KTPAINTAREACONTAINER_H

#include <ktvhbox.h>

#include "ktdocumentruler.h"
#include "apaintarea.h"

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTPaintAreaContainer : public QWidget
{
	Q_OBJECT
	public:
		KTPaintAreaContainer(QWidget *parent = 0);
		~KTPaintAreaContainer();
		APaintArea *drawArea() const;
		
	public slots:
		void moveRulerPointers(const QPoint &);
		
	protected:
// 		void resizeEvent ( QResizeEvent * event );
// 		void mouseMoveEvent(QMouseEvent *e);
// 		QSize sizeHint() const;
		
	private:
		APaintArea *m_drawArea;
		KTDocumentRuler *m_HRuler;
		KTDocumentRuler *m_VRuler;
		
		int m_drawAreaDelta;
};

#endif
