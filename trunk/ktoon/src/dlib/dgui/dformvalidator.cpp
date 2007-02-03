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

#include <QtDebug>

DFormValidator::DFormValidator(QWidget *form) : m_parent(form)
{
}


DFormValidator::~DFormValidator()
{
}

void DFormValidator::setForm(QWidget *form)
{
	m_parent = form;
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
			ok = true;
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

bool DFormValidator::validatesLength(int max)
{
        bool ok = false;
        foreach(QObject *child, m_parent->children())
        {
                if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
                {
                        line->setMaxLength(max);
                        ok = true;
                }
        }

        return ok;
}

bool DFormValidator::validatesLengthOf(int max, const QString &name)
{
        bool ok = false;
        foreach(QObject *child, m_parent->children())
        {
                if( child->objectName() == name)
                {
                        if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
                        {
                                line->setMaxLength(max);
                                ok = true;
                        }

			break;
                }
        }

        return ok;
}

bool DFormValidator::validatesMask(const QString &mask)
{
        bool ok = false;
        foreach(QObject *child, m_parent->children())
        {
                if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
                {
                        line->setInputMask(mask);
                        ok = true;
                }
        }

        return ok;
}

bool DFormValidator::validatesMaskOf(const QString &mask, const QString &name)
{
        bool ok = false;
        foreach(QObject *child, m_parent->children())
        {
                if( child->objectName() == name)
                {
                        if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
                        {
                                line->setInputMask(mask);
                                ok = true;
                        }

                        break;
                }
        }
        return ok;
}


void DFormValidator::validatesMaskOf(const QString &mask, QLineEdit *line)
{
	line->setInputMask(mask);
	m_childs << line;
}

void DFormValidator::validatesLengthOf(int max, QLineEdit *line)
{
	line->setMaxLength(max);
	m_childs << line;
}

void DFormValidator::validatesNumericallyOf(bool real, QLineEdit *line)
{
	if ( real )
	{
		line->setValidator(new QDoubleValidator(line));
	}
	else
	{
		line->setValidator(new QIntValidator(line));
	}
	m_childs << line;
}

void DFormValidator::validatesRangeOf(int i, int e, QLineEdit *line)
{
	line->setValidator(new QIntValidator(i, e, line));
	m_childs << line;
}

void DFormValidator::validatesRegExpOf(const QString &regexp, QLineEdit *line)
{
	line->setValidator(new QRegExpValidator(QRegExp(regexp), line));
	m_childs << line;
}


bool DFormValidator::validate()
{
	bool ok = true;
	
	foreach(QLineEdit *child, m_childs)
	{
		ok = validate( child );
	}
	
	foreach(QObject *child, m_parent->children())
	{
		if ( QLineEdit *line = qobject_cast<QLineEdit *>(child) )
		{
			ok = validate(line);
		}
	}
	
	return ok;
}

bool DFormValidator::validate(QLineEdit *line)
{
	bool ok = true;
	if ( line->validator() != 0 )
	{
		int pos = 0;
		QString text = line->text();
				
		QValidator::State state = line->validator()->validate(text, pos);
				
		if ( text.isEmpty() )
		{
			state = QValidator::Invalid;
		}
				
		QPalette pal = line->palette();
				
		switch(state)
		{
			case QValidator::Acceptable:
			{
				pal.setBrush(QPalette::Base, Qt::green);
			}
			break;
			case QValidator::Intermediate:
			{
				pal.setBrush(QPalette::Base, Qt::yellow);
				ok = false;
			}
			break;
			case QValidator::Invalid:
			{
				pal.setBrush(QPalette::Base, Qt::red);
				ok = false;
			}
			break;
		}
		line->setPalette(pal);
	}
	
	return ok;
}
