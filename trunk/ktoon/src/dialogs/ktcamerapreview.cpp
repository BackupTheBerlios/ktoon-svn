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

#include "ktcamerapreview.h"
#include "status.h"

KTCameraPreview::KTCameraPreview(QWidget* parent, const char* name): KTMdiWindow(parent, name, 0)
{
#ifndef NO_OPENGL
		m_cameraPreview = new GLRenderCameraPreview(this, KTStatus->currentDrawingArea());
#else
// 		m_cameraPreview = new QtRenderCameraPreview(this); // FIXME
#endif

	setCentralWidget(m_cameraPreview);
	
	move( 20, 220 );
	
	show();
}


KTCameraPreview::~KTCameraPreview()
{
}

#ifndef NO_OPENGL
GLRenderCameraPreview
#else
RenderCameraPreviewQt 
#endif	
*KTCameraPreview::cameraPreview()
{
	return m_cameraPreview;
}

void KTCameraPreview::resizeEvent(QResizeEvent *e)
{
#ifndef NO_OPENGL
	m_cameraPreview->resize(e->size().width(), e->size().height());
#else
	m_cameraPreview->resize(e->size().width(), e->size().height());
#endif
	QMainWindow::resizeEvent(e);
}

void KTCameraPreview::grabImage(const QString &filename)
{
#ifndef NO_OPENGL
	m_cameraPreview->saveImage(filename);
#else
	m_cameraPreview->saveImage(filename);
#endif
}




