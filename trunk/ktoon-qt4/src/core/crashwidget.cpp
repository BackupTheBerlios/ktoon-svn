/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                           	   *
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


#include <QPainter>
#include <QDialog>
#include <QPushButton>
#include <QApplication>
#include <QFile>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>

#include "crashwidget.h"
#include "crashhandler.h"

CrashWidget::CrashWidget (int sig) : QDialog(0), m_sig(sig)
{
	setModal(true);
	
	setWindowTitle( CHANDLER->title() );
	
	m_layout = new QVBoxLayout(this);
	
	QLabel *message = new QLabel("<font color="+CHANDLER->messageColor().name()+">"+ CHANDLER->message()+"</color>", this);
	
	m_layout->addWidget(message);
	
	
	QHBoxLayout *hbox = new QHBoxLayout;
	
	QString text = CHANDLER->defaultText();
	QImage img(CHANDLER->defaultImage());

	if ( CHANDLER->containsSignalEntry(sig) )
	{
		text = CHANDLER->signalText(sig);
		img = QImage(CHANDLER->signalImage(sig));
	}
	
	QLabel *sigImg = new QLabel;
	sigImg->setPixmap(QPixmap::fromImage(img));
	
	hbox->addWidget(sigImg);
	
	QTextBrowser *sigText = new QTextBrowser();
	sigText->setHtml(text);
	hbox->addWidget(sigText);
	
	m_layout->addLayout(hbox);
	
	QPushButton *end = new QPushButton( CHANDLER->buttonText(),this );
	connect(end,SIGNAL(clicked()),SLOT(accept()));
	
	
	m_layout->addWidget(end);
	setLayout(m_layout);
}


CrashWidget::~CrashWidget ()
{
}

