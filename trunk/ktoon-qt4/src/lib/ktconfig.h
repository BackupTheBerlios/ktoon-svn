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
#ifndef KTCONFIG_H
#define KTCONFIG_H

#include <QObject>
#include <QDir>
#include <QHash>

#include "ktconfigdocument.h"
#include "ktxmlreader.h"

class KTConfig;

/**
 * @author David Cuadrado
 * this is a ktoon config handler
*/

class KTConfig : public QObject
{
	public:
		KTConfig();
		~KTConfig();
		void init();
		QString read(const QString &sec);
		static KTConfig *instance();
		
		bool isOk();
		KTConfigDocument *configDocument();
		
		void sync();
		
	private:
		KTConfigDocument *m_ktconfig;
		XMLSingleResult m_configKeys;
		bool m_isOk;
		QDir configDirectory;

};

#define KTCONFIG static_cast<KTConfig*>(KTConfig::instance())

#endif
