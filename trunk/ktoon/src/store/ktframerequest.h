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

#ifndef KTFRAMEREQUEST_H
#define KTFRAMEREQUEST_H

#include <ktlayerrequest.h>
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTFrameRequest : public KTLayerRequest
{
	public:
		KTFrameRequest(Action action, int sceneIndex, int layerIndex, int frameIndex, const QVariant &data = 0);
		virtual ~KTFrameRequest();
		
		virtual int id() const;
		int frameIndex() const;
		virtual bool isValid() const;
		virtual KTProjectRequest *clone() const;
		
	private:
		int m_frameIndex;
		
		Q_DISABLE_COPY(KTFrameRequest);
};


#endif

