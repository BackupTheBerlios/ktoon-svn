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

#ifndef SPROJECT_H
#define SPROJECT_H

#include <ktproject.h>
#include <QTimer>
#include "error.h"
#include <QMultiHash>
#include "user.h"

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
		QDomElement toXml(QDomDocument &doc) const;
		void addUser( const QString& login, UserType type );
		QString fileName();
		
		bool isOwner(const Users::User* user);
		
		
	private:
		QTimer *m_saver;
		QString m_filename;
		QMultiHash<UserType, QString> m_users;
		
	public slots:
		void save();
		
	signals:
		void requestSendErrorMessage(const QString &message, Packages::Error::Level level);
		
	protected:
		void timerEvent(QTimerEvent * event );
};

#endif