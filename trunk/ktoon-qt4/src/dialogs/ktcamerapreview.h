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

#ifndef KTCAMERAPREVIEW_H
#define KTCAMERAPREVIEW_H

#include "ktmdiwindow.h"
//Added by qt3to4:
#include <QResizeEvent>

#ifndef NO_OPENGL
#include "glrendercamerapreview.h"
#else
#include "rendercamerapreviewqt.h"
#endif

/**
@author David Cuadrado
*/
class KTCameraPreview : public KTMdiWindow
{
	Q_OBJECT
	public:
		KTCameraPreview(QWidget* parent, const char* name = 0);
		~KTCameraPreview();
#ifndef NO_OPENGL
		GLRenderCameraPreview *cameraPreview();
#else
		RenderCameraPreviewQt *cameraPreview();
#endif

	public slots:
		void grabImage(const QString &filename);
		
	private:
		#ifndef NO_OPENGL
		GLRenderCameraPreview *m_cameraPreview;
		#else
		RenderCameraPreviewQt *m_cameraPreview;
		#endif
		
		
	protected:
		void resizeEvent(QResizeEvent *e);


};

#endif
