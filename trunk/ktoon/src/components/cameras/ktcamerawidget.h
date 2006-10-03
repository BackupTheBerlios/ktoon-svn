/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTCAMERAWIDGET_H
#define KTCAMERAWIDGET_H

#include <ktmodulewidgetbase.h>

class KTProject;
class KTViewCamera;
class QSpinBox;

/**
 * @author David Cuadrado <krawek@gmail.com>
 * @todo:
 * @li
 * - range of frames
 * - aspect ratio
*/

class KTCameraWidget : public KTModuleWidgetBase
{
	Q_OBJECT;
	
	public:
		KTCameraWidget(KTProject *project, QWidget *parent = 0);
		~KTCameraWidget();
		
		KTViewCamera *viewCamera();
		
	public slots:
		void setFPS(int fps);
		
	private:
		KTViewCamera *m_viewCamera;
		
		QSpinBox *m_fps;
};

#endif