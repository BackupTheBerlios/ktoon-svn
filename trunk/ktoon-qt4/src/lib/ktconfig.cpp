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

#include "ktconfig.h"
#include <qdir.h>

#include "ktdebug.h"

KTConfig* KTConfig::m_instance = 0;

KTConfig::KTConfig() : QObject()
{
	KTINIT;
#ifdef Q_WS_X11
	configDirectory.setPath(QDir::homeDirPath()+"/.ktoon");
#elif defined(Q_WS_WIN)
	configDirectory.setPath(QDir::homeDirPath()+"/KToon");
#elif defined(Q_WS_MAC)
	configDirectory.setPath(QDir::homeDirPath()+"/.ktoon");
#endif

	if ( !configDirectory.exists() )
	{
		ktDebug() << tr("%1 not exists... creating...").arg(configDirectory.path()) << endl;
		if(!configDirectory.mkdir(configDirectory.path()))
		{
			ktError() << tr("I can't create %1").arg(configDirectory.path()) << endl;
		}
	}

	m_ktconfig = new KTConfigDocument( configDirectory.path() + "/ktoon.cfg" );
}


KTConfig::~KTConfig()
{
	KTEND;
	if ( m_instance )
		delete m_instance;
}

KTConfig *KTConfig::instance()
{
	if ( ! m_instance )
	{
		m_instance = new KTConfig;
	}
	return m_instance;
}

void KTConfig::init()
{
	ktDebug() << "*********Init configuration file : " << m_ktconfig->path() << endl;
	m_ktconfig->setup();
	m_isOk = m_ktconfig->isOk();
	
	KTXmlReader configreader;
	QFile f(m_ktconfig->path());
	QXmlInputSource xmlsource(&f);
	if ( configreader.parse(&xmlsource))
	{
		m_isOk = m_isOk && true;
		m_configKeys = configreader.getResult();
	}
	else
	{
		qDebug("Error parsing config document");
		m_isOk = m_isOk && false;
	}
}

QString KTConfig::read(const QString &sec)
{
	return m_configKeys[sec];
}

bool KTConfig::isOk()
{
	return m_isOk;
}

KTConfigDocument *KTConfig::configDocument()
{
	return m_ktconfig;
}

void KTConfig::sync()
{
	m_ktconfig->saveConfig();
	init();
	m_isOk = m_isOk && m_ktconfig->isOk();
}

void KTConfig::beginGroup(const QString & prefix )
{
	m_ktconfig->beginGroup( prefix);
}

void KTConfig::setValue ( const QString & key, const QVariant & value )
{
	m_ktconfig->setValue(key, value);
}

// void KTConfig::setValue ( const QString &group, const QString & key, const QVariant & value )
// {
// 	beginGroup( group );
// 	m_ktconfig->setValue(key, value);
// }

QVariant KTConfig::value ( const QString & key, const QVariant & defaultValue) const
{
	return m_ktconfig->value(key, defaultValue);
}

// QVariant KTConfig::value ( const QString &group, const QString & key, const QVariant & defaultValue )
// {
// 	beginGroup( group );
// 	return m_ktconfig->value(key, defaultValue);
// }



