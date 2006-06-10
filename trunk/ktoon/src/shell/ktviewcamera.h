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

#include "dmdiwindow.h"

#include "dvhbox.h"
#include "ktanimationarea.h"
#include "ktcamerabar.h"

#include "ccbar.h"


class QCheckBox;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTViewCamera : public DMdiWindow
{
	Q_OBJECT
	public:
		KTViewCamera(const QSize& size, QWorkspace *parent = 0);
		~KTViewCamera();
		
		KTAnimationArea *animationArea();
		
		QSize sizeHint() const;

		void updateSceneInfo();
		
	private slots:
		void showSceneInfo(const KTScene *scene);
		void setLoop();
		void doPlay();
		
	private:
		QFrame *m_container;
		KTAnimationArea *m_animationArea;
		
		class Status;
		
		Status *m_status;
		
		QCheckBox *m_loop;
};

#endif
