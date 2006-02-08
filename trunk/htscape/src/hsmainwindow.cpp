/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "hsmainwindow.h"
#include <QHBoxLayout>
#include <QtDebug>

HSMainWindow::HSMainWindow() : QMainWindow()
{
	QWidget *container = new QWidget;
	
	QHBoxLayout *layout = new QHBoxLayout(container);
	
	m_source = new QTextEdit;
	layout->addWidget( m_source );
	m_destiny = new QTextBrowser;
	layout->addWidget( m_destiny );
	
	setCentralWidget( container );
	
	// connections
	connect(m_source, SIGNAL(textChanged()), this, SLOT(convertSource()) );
}


HSMainWindow::~HSMainWindow()
{
}

void HSMainWindow::convertSource()
{
	QString text = Qt::escape(m_source->toPlainText());
	text.replace( '\n', Qt::escape("<br>") );
	
	qDebug() << text;
	
	m_destiny->setPlainText( text );
}


