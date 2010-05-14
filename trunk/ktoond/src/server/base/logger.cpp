/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "logger.h"

#include <QFile>
#include <QDateTime>

namespace Base {

struct Logger::Private
{
    QFile file;
};

Logger *Logger::s_self = 0;

Logger::Logger() : k(new Private)
{
    k->file.setFileName("/tmp/server.log");
}

Logger::~Logger()
{
    delete k;
}

Logger *Logger::self()
{
    if (! s_self)
        s_self = new Logger;
    
    return s_self;
}

void Logger::setLogFile(const QString &logfile)
{
    k->file.setFileName(logfile);
}

QString Logger::logFile() const
{
    return k->file.fileName();
}

void Logger::warn(const QString &log)
{
    write(QString(QDateTime::currentDateTime().toString(Qt::ISODate) + " WARNING:  " + log + "\n").toLocal8Bit());
}

void Logger::error(const QString &log)
{
    write(QString(QDateTime::currentDateTime().toString(Qt::ISODate) + " ERROR:  " + log + "\n").toLocal8Bit());
}

void Logger::info(const QString &log)
{
    write(QString(QDateTime::currentDateTime().toString(Qt::ISODate) + " INFO:  " + log + "\n").toLocal8Bit());
}

void Logger::fatal(const QString &log)
{
    write(QString(QDateTime::currentDateTime().toString(Qt::ISODate) + " FATAL:  " + log + "\n").toLocal8Bit());
}

void Logger::write(const QByteArray &msg)
{
    if (k->file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        k->file.write(msg.data(), msg.size());
        k->file.close();
    }
}

}
