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

#include "dtipdatabase.h"

#include "dalgorithm.h"

#include <QDomDocument>
#include <QFile>

#include "ddebug.h"

DTipDatabase::DTipDatabase(const QString &file)
{
	loadTips( file );
	
	if ( !m_tips.isEmpty() )
	{
		m_currentTipIndex = DAlgorithm::random() % m_tips.count();
	}
}


DTipDatabase::~DTipDatabase()
{
}

DTip DTipDatabase::tip() const
{
	if (m_currentTipIndex >= 0 && m_currentTipIndex < m_tips.count() )
		return m_tips[m_currentTipIndex];
	return DTip();
}

void DTipDatabase::nextTip()
{
	if (m_tips.isEmpty())
		return ;
	m_currentTipIndex += 1;
	if (m_currentTipIndex >= (int) m_tips.count())
	{
		m_currentTipIndex = 0;
	}
}

void DTipDatabase::prevTip()
{
	if (m_tips.isEmpty())
		return ;
	m_currentTipIndex -= 1;
	if (m_currentTipIndex < 0)
	{
		m_currentTipIndex = m_tips.count() - 1;
	}
}

void DTipDatabase::loadTips(const QString &filePath)
{
	QDomDocument doc;
	QFile file(filePath);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement();
		if(!e.isNull())
		{
			if ( e.tagName() == "tip" )
			{
				DTip tip;
				tip.text = e.text();
				m_tips << tip;
			}
		}
		n = n.nextSibling();
	}
}


