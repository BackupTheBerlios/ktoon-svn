/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                           	   *
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


#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <QString>
#include <QMap>
#include <QColor>
#include <QPair>
#include <QImage>

#include <csignal>

class CrashHandler;

class CrashHandler
{
	protected:
		CrashHandler();
		
	public:
		~CrashHandler ();
		void setConfig(const QString &filePath);
		
		void setTrapper (void (*trapper)(int));
		static CrashHandler *instance();
		static void init();
		
	public:
		void setProgram (const QString &prog);
		QString program () const;
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
		struct CrashHandlerConfig
		{
			QString title;
			QString message;
			QColor messageColor;
			QString buttonText;
			
			QString defaultText;
			QString defaultImage;
			QMap<int, QPair<QString, QString> > signalEntry;
			
		} m_config;
		
		static CrashHandler *m_instance; // Singleton
		QString m_program;
		QString m_imagePath;
		bool m_verbose;
};

#define CHANDLER CrashHandler::instance()

#endif
