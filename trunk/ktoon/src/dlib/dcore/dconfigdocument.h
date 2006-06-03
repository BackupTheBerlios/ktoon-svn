/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#ifndef DCONFIGDOCUMENT_H
#define DCONFIGDOCUMENT_H

#include <QDomDocument>
#include <QDomElement>
#include <QStringList>
#include <QVariant>
#include <QHash>

/**
 * This class represents the doon configuration xml document
 * @author David Cuadrado
*/
class DConfigDocument : public QDomDocument
{
	public:
    		DConfigDocument(const QString &path);
    		~DConfigDocument();
		
		void beginGroup(const QString & prefix );
		void setValue ( const QString & key, const QVariant & value );
		QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() ) const;
		
		void addRecentFiles(const QStringList &names);
		
		QString path();
		
		void saveConfig(const QString &file = QString::null);
		bool exists(const QString &key);
		
		bool isOk();

		void setup();
		
	private:
		QDomElement find(const QDomElement &element, const QString &key) const;
		
	private:
		QHash<QString, QDomElement> m_groups;
		
		QDomElement m_currentGroup;
		
		QString m_path;
		bool m_isOk;
};

#endif


