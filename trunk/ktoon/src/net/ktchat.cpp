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

#include "ktchat.h"

#include <QTextBrowser>
#include <QLineEdit>

#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>

class KTChat::Private
{
	public:
		Private()
		{
		}
		
		~Private()
		{
			delete lineEdit;
			delete browser;
		}
		
		QLineEdit *lineEdit;
		QTextBrowser *browser;
		QPushButton *send;
		
};

KTChat::KTChat(QWidget *parent) : QDialog(parent), d(new Private)
{
	setAttribute(Qt::WA_DeleteOnClose);
	QGridLayout *layout = new QGridLayout(this);
	
	setWindowTitle("chat");
	
	d->browser = new QTextBrowser;
	layout->addWidget(d->browser, 0, 0 );
	
	
	QHBoxLayout *box = new QHBoxLayout;
	
	d->lineEdit = new QLineEdit;
	box->addWidget(d->lineEdit);
	
	d->send = new QPushButton(tr("Send"));
	box->addWidget(d->send);
	
	layout->addLayout( box, 1, 0);
	
	connect(d->lineEdit, SIGNAL(returnPressed()), d->send, SLOT(animateClick()));
	connect(d->send, SIGNAL(clicked()), this, SLOT(sendMessage()));
}


KTChat::~KTChat()
{
	delete d;
}


void KTChat::addMessage(const QString &from, const QString &message)
{
	d->browser->append(QString("<%1> %2").arg(from).arg(message));
}

void KTChat::sendMessage()
{
	QString text = d->lineEdit->text();
	d->lineEdit->clear();
	if(!text.isEmpty())
	{
		emit requestSendMessage(text);
	}
}

