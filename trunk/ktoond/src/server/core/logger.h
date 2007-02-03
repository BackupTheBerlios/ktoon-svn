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

#ifndef SERVERLOGGER_H
#define SERVERLOGGER_H

#include <QString>
#include <QByteArray>

namespace Server {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Logger
{
	protected:
		Logger();
		
	public:
		~Logger();
		
		static Logger *self();
		
		
		void setLogFile(const QString &logfile);
		QString logFile() const;
		void warn(const QString &log);
		void error(const QString &err);
		void info(const QString &inf);
		void fatal(const QString &fatal);
		
	protected:
		void write(const QByteArray &msg);
		
	private:
		static Logger *s_self;
		struct Private;
		Private *const d;
};

}

#endif