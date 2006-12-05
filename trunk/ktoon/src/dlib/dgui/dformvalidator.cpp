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

#include "dformvalidator.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include <QSpinBox>
#include <QDoubleSpinBox>

DFormValidator::DFormValidator(QWidget *parent) : QObject(parent), m_parent(parent)
{
}


DFormValidator::~DFormValidator()
{
}

bool DFormValidator::validatesNumerically(bool real)
{
	bool ok = false;
	foreach(QObject *child, m_parent->children())
	{
		if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
		{
			if ( real )
			{
				line->setValidator(new QDoubleValidator(line));
			}
			else
			{
				line->setValidator(new QIntValidator(line));
			}
			
			ok = true;
		}
	}
	
	return ok;
}

bool DFormValidator::validatesRange(int i, int e)
{
	bool ok = false;
	
	foreach(QObject *child, m_parent->children())
	{
		if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
		{
			line->setValidator(new QIntValidator(i, e, line));
			ok = true;
		}
		else if ( QSpinBox *spin = qobject_cast<QSpinBox *>(child) )
		{
			spin->setMaximum(e);
			spin->setMinimum(i);
			ok = true;
		}
	}
	
	return ok;
}

bool DFormValidator::validatesRegExp( const QString &regexp)
{
	bool ok = false;
	foreach(QObject *child, m_parent->children())
	{
		if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
		{
			line->setValidator(new QRegExpValidator(QRegExp(regexp), line));
			ok = false;
		}
	}
	
	return ok;
}



bool DFormValidator::validatesNumericallyOf(bool real, const QString &name)
{
	bool ok = false;
	foreach(QObject *child, m_parent->children())
	{
		if( child->objectName() == name)
		{
			if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
			{
				ok = true;
				if ( real )
				{
					line->setValidator(new QDoubleValidator(line));
				}
				else
				{
					line->setValidator(new QIntValidator(line));
				}
			}
		}
	}
	return ok;
}

bool DFormValidator::validatesRangeOf(int i, int e, const QString &name)
{
	bool ok = false;
	foreach(QObject *child, m_parent->children() )
	{
		if( child->objectName() == name)
		{
			if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
			{
				ok = true;
				line->setValidator(new QIntValidator(i, e, line));
			}
			else if ( QSpinBox *spin = qobject_cast<QSpinBox *>(child) )
			{
				spin->setMaximum(e);
				spin->setMinimum(i);
				ok = true;
			}
		}
	}
	return ok;
}

bool DFormValidator::validatesRegExpOf( const QString &regexp, const QString &name)
{
	bool ok = false;
	foreach(QObject *child, m_parent->children())
	{
		if( child->objectName() == name)
		{
			if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
			{
				line->setValidator(new QRegExpValidator(QRegExp(regexp), line));
				ok = true;
			}
		}
	}
	
	return ok;
}

