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

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

#include <QTimer>

// #include "error.h"


class SProject : public KTProject
{
	Q_OBJECT;
	
	public:
		SProject(const QString & filename , QObject *parent = 0);
		~SProject();
		void resetTimer();
		QDomElement info(QDomDocument &doc) const;
		
	private:
		QTimer *m_saver;
		QString m_filename;
		
	public slots:
		void save();
	
	signals:
// 		void requestSendMessage(const QString &message, Packages::Error::Level level);
		
		
	protected:
		void timerEvent ( QTimerEvent * event );
};

#endif
