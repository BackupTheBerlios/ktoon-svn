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

#ifndef DTIPDATABASE_H
#define DTIPDATABASE_H

#include <QString>
#include <QList>
#include "dcore/dglobal.h"

/**
 * @struct DTip
 * @author David Cuadrado <krawek@gmail.com>
 */

struct DTip
{
	QString text;
};

/**
 * @class DTipDatabase
 * 
 * Esta clase esta encargada de manejar la base de datos de mensajes de ayuda, utiles o deseados que se quieran mostrar en la aplicación.
 * 
 * El archivo de base de datos tiene la siguiente forma:
 * 
 * @verbatim
 * \<TipOfDay\>
 * 	\<tip>Tip1.\</tip\>
 * 	\<tip>Tipn\</tip\>
 * \</TipOfDay\>
 * @endverbatim
 * 
 * @author David Cuadrado \<krawek@gmail.com\>
 */

class D_CORE_EXPORT DTipDatabase
{
	public:
		DTipDatabase(const QString &file);
		~DTipDatabase();
		DTip tip() const;
		void nextTip();
		void prevTip();
		
	private:
		void loadTips(const QString &file);
		
	private:
		QList<DTip> m_tips;
		int m_currentTipIndex;
};


#endif
