/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef KTCRASHHANDLER_H
#define KTCRASHHANDLER_H

#include <QString>
#include <QMap>
#include <QColor>
#include <QPair>
#include <QImage>
#include <csignal>

class KTCrashHandler;

#ifdef Q_OS_UNIX

class KTCrashHandler
{
    protected:
        KTCrashHandler();

    public:
        ~KTCrashHandler();
        void setConfig(const QString &filePath);
        void setTrapper(void (*trapper)(int));
        static KTCrashHandler *instance();
        static void init();

    public:
        void setProgram(const QString &prog);
        QString program() const;
        void setImagePath(const QString &imagePath);
        QString imagePath() const;

        QString title() const;
        QString message() const;
        QColor messageColor() const;
        QString buttonText() const;
        QString defaultText() const;
        QString defaultImage() const;
        QString signalText(int signal);
        QString signalImage(int signal);
        bool containsSignalEntry(int signal);

    private:
        struct CrashHandlerConfig {
               QString title;
               QString message;
               QColor  messageColor;
               QString buttonText;
               QString defaultText;
               QString defaultImage;
               QMap<int, QPair<QString, QString> > signalEntry;
        } m_config;

        static KTCrashHandler *m_instance; // Singleton
        QString m_program;
        QString m_imagePath;
        bool m_verbose;
};

#define CHANDLER KTCrashHandler::instance()

#endif // Q_OS_UNIX

#endif
