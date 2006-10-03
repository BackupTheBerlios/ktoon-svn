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

#include "ktcamerawidget.h"
#include "ktviewcamera.h"
#include "ktproject.h"

#include <dgui/dformfactory.h>

#include <QSpinBox>
#include <QVBoxLayout>

KTCameraWidget::KTCameraWidget(KTProject *project, QWidget *parent) : KTModuleWidgetBase(parent)
{
	setWindowIcon(QPixmap(THEME_DIR+"/icons/camera_preview.png") );
	
	m_viewCamera = new KTViewCamera(project);
	
	m_fps = new QSpinBox;
	boxLayout()->addLayout(DFormFactory::makeGrid( QStringList() << "FPS", QWidgetList() << m_fps ));
	connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(setFPS(int)));
	
	m_fps->setValue(14);
}


KTCameraWidget::~KTCameraWidget()
{
}

KTViewCamera *KTCameraWidget::viewCamera()
{
	return m_viewCamera;
}

void KTCameraWidget::setFPS(int fps)
{
	m_viewCamera->setFPS( fps );
}

