/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#ifndef DTRANSFERMANAGER_H
#define DTRANSFERMANAGER_H

#include <QObject>

class QUrl;
class QString;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class DTransferManager : public QObject
{
	Q_OBJECT;
	public:
		enum Protocol
		{
			Http = 0,
			Ftp
		};
		
		DTransferManager();
		~DTransferManager();
		void get(const QUrl &url );
		void get(const QString &url);
		
		template<typename T> inline T provider() const
		{ 
			return qobject_cast<T>(m_provider);
		}
		
	private slots:
		void emitFinished(int id, bool error);
		
	signals:
		void error(const QString &msg);
		void readed(const QByteArray &);
		
	private:
		QObject *m_provider;
		Protocol m_protocol;
};

#endif
