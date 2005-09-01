/***************************************************************************
 *   Copyright (C) 2005 by jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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
#ifndef KTMDIWINDOW_H
#define KTMDIWINDOW_H

#include <qmainwindow.h>

/**
@author Jorge Cuadrado
*/
class KTMdiWindow : public QMainWindow
{
	Q_OBJECT
	public:
		KTMdiWindow(QWidget* parent = 0, const char* name = 0, WFlags f  = 0);
		~KTMdiWindow();
	
	protected:
		virtual bool event( QEvent * e );
	
	signals:
		void sendMessage(const QString &);
		void activate(bool);
};

#endif
