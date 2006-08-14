/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef DAUDIOENGINEIFACE_H
#define DAUDIOENGINEIFACE_H

#include <QString>
#include <QPluginLoader>
#include <QUrl>
#include <qplugin.h>
#include <dcore/dglobal.h>


/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_CORE_EXPORT DAudioEngineIface
{
	public:
		virtual ~DAudioEngineIface() {}
		virtual QString key() const = 0;
		
		/**
		 * If can't load this function returns -1, else returns the player id
		 * @param url 
		 * @param id 
		 * @return 
		 */
		virtual int load( const QUrl &url, int id = -1 ) = 0;
		
		virtual bool init() = 0;
		virtual bool play(int offset = 0) = 0;
		
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void seek( uint ms ) = 0;
		
		virtual bool setCurrentPlayer(int id) = 0;
		
		virtual void setVolume(int percent) = 0;
};

Q_DECLARE_INTERFACE(DAudioEngineIface, "org.dlib.DAudioEngineIface/0.1");

#endif

