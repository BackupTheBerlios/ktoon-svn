/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
 
#ifndef KTNOTICE_H
#define KTNOTICE_H

#include <QDialog>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class KTNotice: public QWidget
{
	Q_OBJECT
	public:
		KTNotice(QWidget *parent = 0);
		~KTNotice();
		void addMessage(const QString &from, const QString &message);
		
	signals:
		void requestSendMessage(const QString &message);
		
	protected slots:
		void sendMessage();
		
	private:
		struct Private;
		Private *const d;
};

#endif
