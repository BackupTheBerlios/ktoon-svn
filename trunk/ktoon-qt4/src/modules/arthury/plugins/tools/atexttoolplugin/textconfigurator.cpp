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

#include "textconfigurator.h"
#include <QBoxLayout>
#include <QFontDatabase>
TextConfigurator::TextConfigurator(QWidget *parent)
 : QWidget(parent)
{
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this );
	setLayout(layout);
	
	QBoxLayout *subLayout = new QBoxLayout(QBoxLayout::LeftToRight );
	layout->addLayout(subLayout);
	
	
	m_families = new QComboBox();

	QFontDatabase fdb;
	QStringList families = fdb.families();
	m_families->addItems ( families );
	
	m_size = new QSpinBox();
	m_size->setMinimum ( 5 );
	m_size->setValue(36);
	
	subLayout->addWidget(m_families);
	subLayout->addWidget(m_size);
// 	ktDebug() << families;
	
	
	m_text = new QLineEdit(this);
	layout->addWidget(m_text);
	
	connect(m_families, SIGNAL(currentIndexChanged ( const QString &  )), this , SLOT(setFamily( const QString &  ) ));
	
	connect(m_size, SIGNAL(valueChanged ( int  )), this , SLOT(setSize( int   ) ));
	setFamily( m_font.family() );
	setSize( m_font.pixelSize() );
}


TextConfigurator::~TextConfigurator()
{
}

QString TextConfigurator::text() const
{
	return m_text->text ();
}

QString TextConfigurator::family() const
{
	return m_families->currentText();
}

int TextConfigurator::size() const
{
	return m_size->value();
}

void TextConfigurator::setFamily(const QString & family)
{
	m_font.setFamily(family);
	QFont font;
	font.setFamily(family);
	m_text->setFont(font);
	if(family != m_families->currentText())
	{
		int index = m_families->findText ( family );
		if(index > 0)
		{
			m_families->setCurrentIndex ( index );
		}
	}
	
	adjustSize();
}
void TextConfigurator::setSize(int size)
{
	m_font.setPixelSize(size);
// 	m_text->setFont(m_font);
	if(size != m_size->value())
	{
		m_size->setValue ( size );
	}
	
	adjustSize();
}

QFont TextConfigurator::textFont() const
{
	return m_font;
}

