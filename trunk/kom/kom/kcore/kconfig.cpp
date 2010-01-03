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

#include "kconfig.h"
#include <qdir.h>

#include <QCoreApplication>
#include <QTextDocument>
#include <QDomDocument>


#include "kcore/kdebug.h"


class KConfig::Private
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

KConfig* KConfig::m_instance = 0;

KConfig::KConfig() : QObject(), k(new Private)
{
    KINIT;
	
#ifdef Q_WS_X11
    k->configDirectory.setPath(QDir::homePath() + "/." + QCoreApplication::applicationName());
#elif defined(Q_WS_WIN)
    k->configDirectory.setPath(QDir::homePath() + "/" + QCoreApplication::applicationName());
#elif defined(Q_WS_MAC)
    k->configDirectory.setPath(QDir::homePath() + "/." + QCoreApplication::applicationName());
#endif

    if (!k->configDirectory.exists()) {
        kDebug() << tr("%1 not exists... creating...").arg(k->configDirectory.path()) << endl;

        if (!k->configDirectory.mkdir(k->configDirectory.path()))
            kError() << tr("I can't create %1").arg(k->configDirectory.path()) << endl;
    }

    k->path = k->configDirectory.path() + "/" + QCoreApplication::applicationName().toLower() + ".cfg";

    init();
}


KConfig::~KConfig()
{
    KEND;
    if (m_instance) 
        delete m_instance;
}

KConfig *KConfig::instance()
{
    if (! m_instance)
        m_instance = new KConfig;

    return m_instance;
}

void KConfig::init()
{
    QFile config(k->path);
    k->isOk = false;

    if (config.exists()) {
        QString errorMsg = "";
        int errorLine = 0;
        int errorColumn = 0;

        k->isOk = k->document.setContent(&config, &errorMsg, &errorLine, &errorColumn);

        if (!k->isOk)
            kDebug() << QObject::tr("Configuration file is corrupted %1:%2: %3").arg(errorLine).arg(errorColumn).arg(errorMsg);

        config.close();
   }

   if (!k->isOk) {
       QDomProcessingInstruction header = k->document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
       k->document.appendChild(header);

       QDomElement root = k->document.createElement("Config");
       k->document.appendChild(root);
   }
}

bool KConfig::isOk()
{
    return k->isOk;
}

QDomDocument KConfig::document()
{
    return k->document;
}

void KConfig::sync()
{
    QFile f(k->path);

    if (f.open(QIODevice::WriteOnly)) {
        kFatal() << "*** Saving config file...";
        QTextStream st(&f);
        st << k->document.toString() << endl;
        k->isOk = true;
        f.close();
    } else {
        kFatal() << "*** NO Saving config file...";
        k->isOk = false;
    }

    init();
}

void KConfig::beginGroup(const QString & prefix)
{
    QString stripped = Qt::escape(prefix);

    stripped.replace(' ', "_");
    stripped.replace('\n', "");

    k->lastGroup = k->currentGroup.tagName();

    if (k->groups.contains(stripped)) {
        k->currentGroup = k->groups[stripped];
    } else {
        k->currentGroup = find(k->document.documentElement(), stripped);

        if (k->currentGroup.isNull()) {
            k->currentGroup = k->document.createElement(stripped);
            k->document.documentElement().appendChild(k->currentGroup);
        }
    }
}

void KConfig::endGroup()
{
    if (!k->lastGroup.isEmpty())
        beginGroup(k->lastGroup);
}

void KConfig::setValue(const QString & key, const QVariant & value)
{
    QDomElement element = find(k->currentGroup, key);

    if (!element.isNull()) {
        if (value.canConvert(QVariant::StringList)) {
            QStringList list = value.toStringList();
            element.setAttribute("value", list.join(";"));
        } else {
            element.setAttribute("value", value.toString());
        }
    } else {
        element = k->document.createElement(key);

        if (value.canConvert(QVariant::StringList)) {
            QStringList list = value.toStringList();
            element.setAttribute("value", list.join(";"));
        } else {
            element.setAttribute("value", value.toString());
        }

        k->currentGroup.appendChild(element);
    }
}

QVariant KConfig::value(const QString & key, const QVariant & defaultValue) const
{
   QDomElement element = find(k->currentGroup, key); // Current group or root?
	
   if (element.isNull())
       return defaultValue;

   QVariant v = element.attribute("value");

   if (v.toString() == "false") {
       return false;
   } else if (v.toString() == "true") {
              return true;
   }

   return v;
}

QDomElement KConfig::find(const QDomElement &element, const QString &key) const 
{
   QDomElement recent;
   QDomNode n = element.firstChild();

   while (!n.isNull()) {
          QDomElement e = n.toElement();
          if (!e.isNull()) {
              if (e.tagName() == key) {
                  recent = e;
                  break;
              }
          }
          n = n.nextSibling();
   }

   return recent;
}

