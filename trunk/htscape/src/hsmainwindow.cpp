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

// Qt
#include <QHBoxLayout>
#include <QtDebug>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>


// libKtoon
#include "spellhighlighter.h"

HSMainWindow::HSMainWindow() : QMainWindow()
{
	QWidget *container = new QWidget;
	
	QMenu *file = menuBar()->addMenu(tr("File"));
	file->addAction(tr("Quit"), this, SLOT(close()));
	 
	QMenu *help = menuBar()->addMenu(tr("Help"));
	help->addAction(tr("About Qt..."), qApp, SLOT(aboutQt()));
	
	
	QVBoxLayout *mainLayout = new QVBoxLayout(container);
	
	QHBoxLayout *layout = new QHBoxLayout();
	
	m_source = new QTextEdit;
	layout->addWidget( m_source );
	
	new SpellHighlighter(m_source->document());
	
	m_destiny = new QTextBrowser;
	layout->addWidget( m_destiny );
	
	mainLayout->addLayout( layout );
	
	m_result = new QTextBrowser;
	mainLayout->addWidget( m_result );
	
	
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
	
	m_destiny->setPlainText( text );
	
	QString result = m_source->toPlainText();
	result.replace('\n', "<br>");
	
	m_result->setHtml(result);
}


