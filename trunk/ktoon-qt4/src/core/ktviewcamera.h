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

#ifndef KTVIEWCAMERA_H
#define KTVIEWCAMERA_H

#include "ktmdiwindow.h"

#include "ktvhbox.h"
#include "aanimationarea.h"
#include "ktcamerabar.h"

#include "ccbar.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTViewCamera : public KTMdiWindow
{
	Q_OBJECT
	public:
		KTViewCamera(QWorkspace *parent = 0);
		~KTViewCamera();
		
		AAnimationArea *animationArea();
		
// 		QSize sizeHint() const;
		
	private slots:

	private:
		QFrame *m_container;
		AAnimationArea *m_animationArea;
// 		KTCameraBar *m_bar;
// 		CCBar *m_bar;
};

#endif
