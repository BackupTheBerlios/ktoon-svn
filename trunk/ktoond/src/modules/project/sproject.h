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

#ifndef SPROJECT_H
#define SPROJECT_H

#include <ktproject.h>
#include "packages/error.h"
#include "users/user.h"

#include <QTimer>
#include <QMultiHash>

/**
 * @author David Cuadrado <krawek@toonka.com>
 */
class SProject : public KTProject
{
    Q_OBJECT;
    
    public:
        enum UserType{Owner = 0, Desiger};
        SProject(const QString & filename, QObject *parent = 0);
        ~SProject();
        void resetTimer();
        QDomElement infoToXml(QDomDocument &doc) const;
        
        bool addUser(const QString& login, UserType type);
        void setUsers(const QMultiHash<SProject::UserType, QString> & users);
        QString fileName();
        
        bool isOwner(const Users::User* user);
        
    private:
        struct Private;
        Private *const k;
    
    public slots:
        bool save();
        
    signals:
        void requestSendErrorMessage(const QString &message, Packages::Error::Level level);
        
    protected:
        void timerEvent(QTimerEvent * event);
};

#endif
