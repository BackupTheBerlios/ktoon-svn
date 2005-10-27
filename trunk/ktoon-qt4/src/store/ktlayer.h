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

#ifndef KTLAYER_H
#define KTLAYER_H

#include <QObject>
#include "ktkeyframe.h"

typedef QList<KTKeyFrame *> Frames;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLayer : public QObject
{
	Q_OBJECT
	public:
		KTLayer(QObject *parent = 0);
		KTLayer(const QString &layerName, QObject * parent = 0);
		
		~KTLayer();
		Frames frames();
		void setFrames(const Frames &frames);
		
		KTKeyFrame *createFrame();
		
		KTKeyFrame *currentFrame();
		void setCurrentFrame(int index);
		int indexCurrentFrame();
		
		void setLayerName(const QString &name);
		void setVisible(bool isVisible);
		
		QString layerName() const;
		bool isVisible(); 
	
	signals: // TODO: add more signals
		//FIXME:
		void frameCreated( const QString &name);
	
	private:
		Frames m_frames;
		bool m_isVisible;
		QString m_name;
		
		KTKeyFrame *m_currentFrame;
		
		int m_framesCount;

};

#endif
