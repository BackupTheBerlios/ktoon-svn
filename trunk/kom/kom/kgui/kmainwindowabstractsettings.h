/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KMAINWINDOWABSTRACTSETTINGS_H
#define KMAINWINDOWABSTRACTSETTINGS_H

#include <QObject>
#include <kgui/kideality.h>

class KToolView;
class KMainWindow;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_IDEAL_EXPORT KMainWindowAbstractSettings : public QObject
{
	public:
		KMainWindowAbstractSettings(QObject *parent = 0) : QObject(parent) {}
		~KMainWindowAbstractSettings() {};
		
		virtual void save(KMainWindow *w) = 0;
		virtual void restore(KMainWindow *w) = 0;
		
};

#endif
