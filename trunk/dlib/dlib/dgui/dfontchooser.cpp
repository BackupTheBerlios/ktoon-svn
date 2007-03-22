/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dfontchooser.h"
#include <QComboBox>

#include <QHBoxLayout>
#include <QFontDatabase>
#include <QFontComboBox>

#include "dformfactory.h"

DFontChooser::DFontChooser(QWidget *parent) : QFrame(parent)
{
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	m_families = new QFontComboBox;
	
	connect(m_families, SIGNAL(currentFontChanged(const QFont & )), this, SLOT(loadFontInfo(const QFont &)));
	
	mainLayout->addLayout(DFormFactory::makeLine(tr("Family"), m_families));
	
	m_fontStyle = new QComboBox;
	connect(m_fontStyle, SIGNAL(activated (int)), this, SLOT(emitFontChanged( int)));
	mainLayout->addLayout(DFormFactory::makeLine(tr("Style"), m_fontStyle));
	
	m_fontSize = new QComboBox;
	connect(m_fontSize, SIGNAL(activated (int)), this, SLOT(emitFontChanged( int)));
	mainLayout->addLayout(DFormFactory::makeLine(tr("Size"), m_fontSize));
	
	setCurrentFont(m_families->currentFont());
}


DFontChooser::~DFontChooser()
{
}

void DFontChooser::loadFontInfo(const QFont &newFont)
{
	QString currentSize = m_fontSize->currentText();
	QString currentStyle = m_fontStyle->currentText();
	
	QString family = newFont.family();
	
	QFontDatabase fdb;
	
	m_fontStyle->clear();
	
	m_fontStyle->addItem(tr("Normal"), QFont::StyleNormal );
	m_fontStyle->addItem(tr("Italic"), QFont::StyleItalic );
	m_fontStyle->addItem(tr("Oblique"), QFont::StyleOblique );
	
// 	m_fontStyle->addItems();
	
	
	m_fontSize->clear();
	
	QList<int> points = fdb.pointSizes(family);
	
	foreach(int point, points)
	{
		m_fontSize->addItem(QString::number(point));
	}
	
	int sizeIndex = m_fontSize->findText(currentSize);
	int styleIndex = m_fontStyle->findText(currentStyle);
	if ( sizeIndex >= 0 )
	{
		m_fontSize->setCurrentIndex(sizeIndex);
	}
	
	if(styleIndex >= 0)
	{
		m_fontStyle->setCurrentIndex(styleIndex);
	}
	
	m_families->blockSignals(true);
	m_currentFont = newFont;
	m_currentFont.setPointSize( m_fontSize->currentText().toInt() );
	m_currentFont.setStyle( QFont::Style(m_fontStyle->itemData( m_fontStyle->currentIndex() ).toInt()) );
	m_families->blockSignals(false);
	
	emit fontChanged();
}

void DFontChooser::emitFontChanged(int )
{
	emit fontChanged();
}

void DFontChooser::setCurrentFont(const QFont &font)
{
	QFontDatabase fdb;
	
	m_families->setCurrentIndex(m_families->findText(font.family()));
	m_fontStyle->setCurrentIndex(m_fontStyle->findText(fdb.styleString(font.family())));
	m_fontSize->setCurrentIndex(m_fontSize->findText(QString::number(font.pointSize())));
}

QFont DFontChooser::currentFont() const
{
	return m_currentFont;
}


