/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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
#include <QTextDocument>
#include <QDomDocument>


#include "dcore/ddebug.h"


class DConfig::Private
{
	public:
		QDomDocument document;
		QString path;
		
		bool isOk;
		QDir configDirectory;
		
		
		QHash<QString, QDomElement> groups;
		QDomElement currentGroup;
		QString lastGroup;
};


DConfig* DConfig::m_instance = 0;

DConfig::DConfig() : QObject(), d(new Private)
{
	DINIT;
	
#ifdef Q_WS_X11
	d->configDirectory.setPath(QDir::homePath()+"/."+QCoreApplication::applicationName ());
#elif defined(Q_WS_WIN)
	d->configDirectory.setPath(QDir::homePath()+"/"+QCoreApplication::applicationName ());
#elif defined(Q_WS_MAC)
	d->configDirectory.setPath(QDir::homePath()+"/."+QCoreApplication::applicationName ());
#endif

	if ( !d->configDirectory.exists() )
	{
		dDebug() << tr("%1 not exists... creating...").arg(d->configDirectory.path()) << endl;
		if(!d->configDirectory.mkdir(d->configDirectory.path()))
		{
			dError() << tr("I can't create %1").arg(d->configDirectory.path()) << endl;
		}
	}
	
	d->path = d->configDirectory.path() + "/"+QCoreApplication::applicationName().toLower()+".cfg";
	
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
	QFile config( d->path );
	d->isOk = false;
	
	if ( config.exists() )
	{
		QString errorMsg = "";
		int errorLine = 0;
		int errorColumn = 0;
		
		d->isOk = d->document.setContent(&config, &errorMsg, &errorLine, &errorColumn);
		
		if ( !d->isOk )
		{
			dDebug() << QObject::tr("Configuration file is corrupted %1:%2: %3").arg(errorLine).arg(errorColumn).arg(errorMsg);
		}
		config.close();
	}
	
	if ( !d->isOk )
	{
		QDomProcessingInstruction header = d->document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
		d->document.appendChild(header);
		
		QDomElement root = d->document.createElement( "DConfig" );
		d->document.appendChild( root );
	}
}

bool DConfig::isOk()
{
	return d->isOk;
}

QDomDocument DConfig::document()
{
	return d->document;
}

void DConfig::sync()
{
	QFile f(d->path);
	
	if ( f.open(QIODevice::WriteOnly) )
	{
		QTextStream st( &f );
		st << d->document.toString() << endl;
		
		d->isOk = true;
		
		f.close();
	}
	else
	{
		d->isOk = false;
	}
	
	init();
}

void DConfig::beginGroup(const QString & prefix )
{
	QString stripped = Qt::escape(prefix);
	
	stripped.replace(' ', "_");
	stripped.replace('\n', "");
	
	d->lastGroup = d->currentGroup.tagName();
	
	if ( d->groups.contains(stripped) )
	{
		d->currentGroup = d->groups[stripped];
	}
	else
	{
		d->currentGroup = find(d->document.documentElement(), stripped);
		
		if ( d->currentGroup.isNull() )
		{
			d->currentGroup = d->document.createElement(stripped);
			d->document.documentElement().appendChild(d->currentGroup);
		}
		
	}
}

void DConfig::endGroup()
{
	if ( !d->lastGroup.isEmpty() )
	{
		beginGroup( d->lastGroup );
	}
}

void DConfig::setValue ( const QString & key, const QVariant & value )
{
	QDomElement element = find(d->currentGroup, key);
	
	if ( !element.isNull () )
	{
		if ( value.canConvert( QVariant::StringList ) )
		{
			QStringList list = value.toStringList();
			
			element.setAttribute("value", list.join(";"));
		}
		else
		{
			element.setAttribute("value", value.toString());
		}
	}
	else
	{
		element = d->document.createElement(key);
		
		if ( value.canConvert( QVariant::StringList ) )
		{
			QStringList list = value.toStringList();
			
			element.setAttribute("value", list.join(";"));
		}
		else
		{
			element.setAttribute("value", value.toString());
		}
		
		d->currentGroup.appendChild(element);
	}
}

QVariant DConfig::value ( const QString & key, const QVariant & defaultValue) const
{
	QDomElement element = find(d->currentGroup, key); // Current group or root?
	
	if ( element.isNull() )
	{
		return defaultValue;
	}
	
	QVariant v = element.attribute("value");
	
	if ( v.toString() == "false" )
	{
		return false;
	}
	else if ( v.toString() == "true" )
	{
		return true;
	}
	
	return v;
}



QDomElement DConfig::find(const QDomElement &element, const QString &key) const 
{
	QDomElement recent;
	QDomNode n = element.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if ( e.tagName() == key )
			{
				recent = e;
				break;
			}
		}
		n = n.nextSibling();
	}
	
	return recent;
}

