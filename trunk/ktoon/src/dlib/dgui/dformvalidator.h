/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef DVALIDATES_H
#define DVALIDATES_H

#include <qobject.h>

#define D_GIVE_NAME(obj) obj->setObjectName( #obj );

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class DFormValidator : public QObject
{
	public:
		DFormValidator(QWidget *parent);
		~DFormValidator();
		
		bool validatesNumerically(bool real);
		bool validatesRange(int i, int e);
		bool validatesRegExp( const QString &regexp);
		
		
		bool validatesNumericallyOf(bool real, const QString &name);
		bool validatesRangeOf(int i, int e, const QString &name);
		bool validatesRegExpOf( const QString &regexp, const QString &name);
		
	private:
		
	private:
		QWidget *m_parent;
};

#endif

