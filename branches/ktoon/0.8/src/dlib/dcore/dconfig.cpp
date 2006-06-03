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

#include "dconfig.h"
#include <qdir.h>

#include <QCoreApplication>

#include "ddebug.h"

DConfig* DConfig::m_instance = 0;

DConfig::DConfig() : QObject()
{
	DINIT;
#ifdef Q_WS_X11
	configDirectory.setPath(QDir::homePath()+"/."+QCoreApplication::applicationName ());
#elif defined(Q_WS_WIN)
	configDirectory.setPath(QDir::homePath()+"/"+QCoreApplication::applicationName ());
#elif defined(Q_WS_MAC)
	configDirectory.setPath(QDir::homePath()+"/."+QCoreApplication::applicationName ());
#endif

	if ( !configDirectory.exists() )
	{
		dDebug() << tr("%1 not exists... creating...").arg(configDirectory.path()) << endl;
		if(!configDirectory.mkdir(configDirectory.path()))
		{
			dError() << tr("I can't create %1").arg(configDirectory.path()) << endl;
		}
	}

	m_dconfig = new DConfigDocument( configDirectory.path() + "/"+QCoreApplication::applicationName().toLower()+".cfg" );
	
	init();
}


DConfig::~DConfig()
{
	DEND;
	if ( m_instance ) delete m_instance;
}

DConfig *DConfig::instance()
{
	if ( ! m_instance )
	{
		m_instance = new DConfig;
	}
	return m_instance;
}

void DConfig::init()
{
	dDebug() << "*********Init configuration file : " << m_dconfig->path() << endl;
	m_dconfig->setup();
	m_isOk = m_dconfig->isOk();

}

bool DConfig::isOk()
{
	return m_isOk;
}

DConfigDocument *DConfig::configDocument()
{
	return m_dconfig;
}

void DConfig::sync()
{
	m_dconfig->saveConfig();
	init();
	m_isOk = m_isOk && m_dconfig->isOk();
}

void DConfig::beginGroup(const QString & prefix )
{
	m_dconfig->beginGroup( prefix);
}

void DConfig::setValue ( const QString & key, const QVariant & value )
{
	m_dconfig->setValue(key, value);
}

QVariant DConfig::value ( const QString & key, const QVariant & defaultValue) const
{
	return m_dconfig->value(key, defaultValue);
}



