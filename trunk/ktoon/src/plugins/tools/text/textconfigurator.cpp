/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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
#include <QCheckBox>

#include <kgui/kfontchooser.h>
#include <kcore/kspellhighlighter.h>

// #include <kcore/kdebug.h>

TextConfigurator::TextConfigurator(QWidget *parent) : QWidget(parent)
{
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this );
	setLayout(layout);
	
	m_fontChooser = new KFontChooser;
	layout->addWidget(m_fontChooser);

	m_text = new QTextEdit(this);
	layout->addWidget(m_text);
	
	m_isHtml = new QCheckBox(tr("html"));
	layout->addWidget(m_isHtml);
	
	layout->addStretch(1);
	
	connect(m_fontChooser, SIGNAL(fontChanged()), this, SLOT(changeFont()));
	
	new KSpellHighlighter(m_text->document());
}


TextConfigurator::~TextConfigurator()
{
}

QString TextConfigurator::text() const
{
	return m_text->toPlainText();
}

QFont TextConfigurator::textFont() const
{
	return m_fontChooser->font();
}

void TextConfigurator::changeFont()
{
	QFont font = m_fontChooser->font();
	m_text->setFont(font);
	
	adjustSize();
}

void TextConfigurator::setDocument(QTextDocument *doc)
{
	m_text->setDocument( doc );
}

bool TextConfigurator::isHtml() const
{
	return m_isHtml->isChecked();
}
